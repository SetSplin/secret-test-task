# <R/Trading> test task
Welcome to perf branch! Here you can find my train of thought.

> Dont forget to install Flame Graph git submodule!

## First look
I wrote `perf_test.cpp` for testing my `Hashtable`'s performance.

> [Important]. I generate strings array *before* main perf test to get cleaner perfomance data

Lets go ahead and get first perf data...

```bash
make perf
```

> Check out for command in `Makefile`, it requires sudo.

After command ends you can open `dist/perf-kernel.svg` using any modern browser.

> I wont explane what is Flame Graph exactly. All you need to know - x axis is time, child functions called from parent function, order is meaningless

## Main reason
Its easy to see that `Hashtable::Insert` (in the very middle) performed +-6 times longer than `std::vector::_M_realloc_insert` (the last block above main).
Just for tests I added one more constructor. `Hashtable(size_t)` to set initial table size to get better statistics. Its important to note that this constructor cant be used in production because of next optimizations!!!

I added it cause `new` is main reason on slowness.

After initializing with initial size, `Insert` became work +-5 times longer than...


## Floating-point optimization
First of all lets remove floating-point arithmetic
```cpp
if ((table_size_ == 0) ||
    (static_cast<double>(table_fullness_) / table_size_ >
        fullness_threshold)) {
    RelocateTable();
}
```

turns to

```cpp

void RelocateTable() {
    ...
    next_goal_ = table_size_ * fullness_threshold;
    ...
}

if ((table_size_ == 0) || (table_fullness_ > next_goal_)) {
    RelocateTable();
}
```

It works! Now it +- 4 time slower than...

## Step optimization
Now lets set diffirent step size for each `key`.
```cpp
...
const size_t steps[] = {1, 3, 7, 11, 13, 17, 19}; // this brokes added constructor
                                                  // cause step must be prime with
                                                  // table size
const size_t steps_len = 7;
...

TableItem* FindPtr(const KeyType& key) const {
    ...
    size_t key_hash = hasher_(key);
    size_t index = key_hash % table_size_;
    size_t step = steps[index % steps_len]; // new line
    while (table_[index].key && (table_[index].hash != key_hash) &&
            !comparator_(key, table_[index].key.value())) {
        index += step; // global index_step to step
        index %= table_size_;
    }
    ...
}
```
Wow it makes `Insert` work +-3 times slower than...

> I use hashes in search cycles to increase search speed. Its optimization as well.

## About Find, Erase
All optimisations above affected `Find` and `Erase` as well. `Insert` was selected cause of slowness and its easy to see how optimization works on `Insert`.

## Separate chaining
According to task I implemented hashtable with *Open addressing* (to reduce `new` calls). First optimization (one more constructor) shows that *Separate chaining* could be slower.