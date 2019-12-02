#include <string>

#include <catch.h>
#include <hashtable.h>
#include <complex_int.h>

struct Hasher {
    size_t operator()(const Complex& number)  const {
        std::hash<int> hasher;
        return hasher(number.Real()) + hasher(number.Imaginary());
    }
};


namespace std {
    template <> struct equal_to<Complex> {
        bool operator()(const Complex& a, const Complex& b) const {
            return (a.Real() == b.Real()) && (a.Imaginary() == b.Imaginary());
        }
    };
}

TEST_CASE( "Custom hasher" ) {
    SECTION( "it compiles" ) {
        Hashtable<Complex, int, Hasher, std::equal_to<Complex>> table;
        table.Insert(Complex(), 42);
    }

    SECTION( "stress" ) {
        size_t size = 10000;
        Hashtable<Complex, int, Hasher, std::equal_to<Complex>> table;
        for (size_t i = 0; i < size; ++i) {
            table.Insert(Complex(i, i), i);
        }
        for (size_t i = 0; i < size; ++i) {
            REQUIRE( table[Complex(i, i)] == i );
        }
        REQUIRE( table.Size() == size );
        for (size_t i = 0; i < size; ++i) {
            table.Erase(Complex(i, i));
        }
        REQUIRE( table.Size() == 0 );
    }
}
