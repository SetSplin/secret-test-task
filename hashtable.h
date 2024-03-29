#pragma once

#include <functional>
#include <optional>
#include <memory>
#include <utility>
#include <iterator>
#include <type_traits>
#include <algorithm>

#include <iostream>

namespace {
const size_t initial_table_size = 16;
const double fullness_threshold = 0.3;
const size_t index_step = 1;
}  // namespace

template <class KeyType, class ValueType, class HasherType = std::hash<KeyType>,
          class ComparatorType = std::equal_to<KeyType>>
class Hashtable {
public:
    template <class ItemType>
    class Iterator : public std::forward_iterator_tag {
    private:
        friend class Hashtable;
        ItemType* item_;
        ItemType* end_;

    public:
        Iterator(ItemType* ptr, ItemType* end) : item_(ptr), end_(end) {
        }

        Iterator& operator++() {
            ++item_;
            while ((item_ != end_) && !(item_->key)) {
                ++item_;
            }
            return *this;
        }
        Iterator operator++(int) {
            Iterator result(item_, end_);
            ++item_;
            while ((item_ != end_) && !(item_->key)) {
                ++item_;
            }
            return result;
        }

        auto operator*() const {
            return std::make_pair<KeyType, decltype(item_->value.value())&>(
                static_cast<KeyType>(item_->key.value()), item_->value.value());
        }
        auto operator->() const {
            return std::make_shared<
                std::pair<KeyType, decltype(item_->value.value())&>>(
                item_->key.value(), item_->value.value());
        }

        bool operator==(const Iterator<ItemType>& rhs) const {
            return item_ == rhs.item_;
        }
        bool operator!=(const Iterator<ItemType>& rhs) const {
            return item_ != rhs.item_;
        }
    };

private:
    struct TableItem {
        std::optional<ValueType> value;
        std::optional<KeyType> key;
        size_t hash;

        TableItem() {
        }

        TableItem(const TableItem& rhs)
            : value(rhs.value.value()), key(rhs.key.value()), hash(rhs.hash) {
        }
    };

    TableItem* table_;
    size_t table_size_;
    size_t table_fullness_;

    HasherType hasher_;
    ComparatorType comparator_;

    void RelocateTable() {
        if (table_size_ == 0) {
            table_size_ = initial_table_size;
            table_ = new TableItem[table_size_];
            return;
        }

        TableItem* old_table = table_;
        size_t old_table_size = table_size_;
        table_size_ <<= 1;
        table_ = new TableItem[table_size_];
        table_fullness_ = 0;

        for (size_t i = 0; i < old_table_size; ++i) {
            if (old_table[i].key) {
                Insert(old_table[i].key.value(), old_table[i].value.value());
            }
        }

        delete[] old_table;
    }

    TableItem* FindPtr(const KeyType& key) const {
        if (table_size_ == 0) {
            return table_;
        }

        size_t key_hash = hasher_(key);
        size_t index = key_hash % table_size_;
        while (table_[index].key && (table_[index].hash != key_hash) &&
               !comparator_(key, table_[index].key.value())) {
            index += index_step;
            index %= table_size_;
        }

        if (table_[index].key && comparator_(key, table_[index].key.value())) {
            return table_ + index;
        }
        return table_ + table_size_;
    }

    TableItem* BeginPtr() const {
        for (size_t i = 0; i < table_size_; ++i) {
            if (table_[i].key) {
                return table_ + i;
            }
        }
        return table_ + table_size_;
    }

    TableItem* EndPtr() const {
        return table_ + table_size_;
    }

public:
    Hashtable()
        : table_(nullptr),
          table_size_(0),
          table_fullness_(0),
          hasher_(HasherType()),
          comparator_(ComparatorType()) {
    }

    Hashtable(size_t size)
        : table_size_(size),
          table_fullness_(0),
          hasher_(HasherType()),
          comparator_(ComparatorType()) {
        table_ = new TableItem[table_size_];
    }

    Hashtable(std::initializer_list<std::pair<KeyType, ValueType>> list)
        : table_(nullptr),
          table_size_(0),
          table_fullness_(0),
          hasher_(HasherType()),
          comparator_(ComparatorType()) {
        for (const auto& [key, value] : list) {
            Insert(key, value);
        }
    }

    Hashtable(const Hashtable<KeyType, ValueType, HasherType, ComparatorType>&
                  table) {
        comparator_ = table.comparator_;
        hasher_ = table.hasher_;
        table_size_ = table.table_size_;
        table_fullness_ = table.table_fullness_;
        table_ = new TableItem[table_size_];
        for (size_t i = 0; i < table_size_; ++i) {
            if (table.table_[i].key) {
                table_[i] = table.table_[i];
            }
        }
    }

    Hashtable(Hashtable<KeyType, ValueType, HasherType, ComparatorType>&& table)
        : table_(nullptr),
          table_size_(0),
          table_fullness_(0),
          hasher_(std::hash<KeyType>()),
          comparator_(std::equal_to<KeyType>()) {
        std::swap(comparator_, table.comparator_);
        std::swap(hasher_, table.hasher_);
        std::swap(table_size_, table.table_size_);
        std::swap(table_fullness_, table.table_fullness_);
        std::swap(table_, table.table_);
    }

    ~Hashtable() {
        delete[] table_;
    }

    size_t Size() const {
        return table_fullness_;
    }

    std::pair<Iterator<TableItem>, bool> Insert(const KeyType& key,
                                                const ValueType& value) {
        if ((table_size_ == 0) ||
            (static_cast<double>(table_fullness_) / table_size_ >
             fullness_threshold)) {
            RelocateTable();
        }
        size_t key_hash = hasher_(key);
        size_t index = key_hash % table_size_;
        while (table_[index].key) {
            if ((table_[index].hash == key_hash) &&
                comparator_(key, table_[index].key.value())) {
                return {Iterator<TableItem>(table_ + table_size_,
                                            table_ + table_size_),
                        false};
            }
            index += index_step;
            index %= table_size_;
        }

        table_[index].value.emplace(value);
        table_[index].key.emplace(key);
        table_[index].hash = key_hash;
        ++table_fullness_;

        return {Iterator<TableItem>(table_ + index, table_ + table_size_),
                true};
    }

    Iterator<TableItem> Erase(Iterator<TableItem> iterator) {
        iterator.item_->key = std::optional<KeyType>();
        iterator.item_->value = std::optional<ValueType>();
        --table_fullness_;
        return ++iterator;
    }

    Iterator<TableItem> Erase(const KeyType& key) {
        TableItem* item = FindPtr(key);
        item->key = std::optional<KeyType>();
        item->value = std::optional<ValueType>();
        --table_fullness_;
        Iterator<TableItem> result{item, table_ + table_size_};
        return ++result;
    }

    Iterator<TableItem> Find(const KeyType& key) {
        return Iterator<TableItem>(FindPtr(key), table_ + table_size_);
    }

    Iterator<const TableItem> Find(const KeyType& key) const {
        return Iterator<const TableItem>(FindPtr(key), table_ + table_size_);
    }

    ValueType& operator[](const KeyType& key) {
        TableItem* found = FindPtr(key);
        if (found == table_ + table_size_) {
            std::pair<Iterator<TableItem>, bool> result =
                Insert(key, ValueType());
            found = result.first.item_;
        }

        return found->value.value();
    }

    Iterator<TableItem> Begin() {
        return Iterator<TableItem>(BeginPtr(), table_ + table_size_);
    }

    Iterator<const TableItem> Begin() const {
        return Iterator<const TableItem>(BeginPtr(), table_ + table_size_);
    }

    Iterator<TableItem> End() {
        return Iterator<TableItem>(EndPtr(), table_ + table_size_);
    }

    Iterator<const TableItem> End() const {
        return Iterator<const TableItem>(EndPtr(), table_ + table_size_);
    }

    Iterator<TableItem> begin() {  // NOLINT
        return Begin();
    }

    Iterator<const TableItem> begin() const {  // NOLINT
        return Begin();
    }

    Iterator<TableItem> end() {  // NOLINT
        return End();
    }

    Iterator<const TableItem> end() const {  // NOLINT
        return End();
    }
};
