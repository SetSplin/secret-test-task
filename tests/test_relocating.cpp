#include <string>

#include <catch.h>
#include <hashtable.h>

TEST_CASE( "Table relocating" ) {
    Hashtable<size_t, size_t> table;
    size_t size = 10000;
    for (size_t i = 0; i < size; ++i) {
        table.Insert(i, i);
        REQUIRE( table.Size() == i + 1 );
    }

    for (size_t i = 0; i < size; ++i) {
        REQUIRE( table[i] == i );
    }

    REQUIRE( table.Size() == size );
}