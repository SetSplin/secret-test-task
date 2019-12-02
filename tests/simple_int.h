#pragma once

class Int {
private:
    int value_;

public:
    static size_t created;
    static size_t destroyed;

    static void Reset() {
        created = 0;
        destroyed = 0;
    }

    Int(int value) : value_(value) {
        ++created;
    }

    Int() : Int(0) {
    }

    Int(const Int& rhs) : Int(rhs.value_) {
    }

    bool operator==(const Int& rhs) const {
        return rhs.value_ == value_;
    }

    ~Int() {
        ++destroyed;
    }
};

size_t Int::created = 0;
size_t Int::destroyed = 0;