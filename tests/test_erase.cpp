#include <string>

#include <catch.h>
#include <hashtable.h>

TEST_CASE( "Erase" ) {
    SECTION( "small test" ) {
        Hashtable<std::string, int> table;
        table.Insert("asd", 42);
        table.Insert("qwe", 69);
        auto first = table.Begin();
        auto second = table.Begin();
        std::string first_key = first->first;
        ++second;
        std::string second_key = second->first;
        int second_value = second->second;

        REQUIRE( table.Size() == 2 );
        REQUIRE( table.Erase(first) == second );
        REQUIRE( table.Size() == 1 );
        REQUIRE( table.Find(first_key) == table.end() );
        REQUIRE( table[second_key] == second_value );
        REQUIRE( table.Erase(second_key) == table.End() );
    }

    SECTION( "large test" ) {
        Hashtable<size_t, size_t> table;
        size_t size = 1000;
        for (size_t i = 0; i < size; ++i) {
            table.Insert(i, i);
        }
        REQUIRE( table.Size() == size );

        for (size_t i = 0; i < size; ++i) {
            table.Erase(table.Begin());
        }
        REQUIRE( table.Size() == 0 );
    }

    SECTION( "non linear test" ) {
        Hashtable<size_t, size_t> table;
        size_t size = 1000;
        for (size_t i = 0; i < size; ++i) {
            table.Insert(i, i);
        }
        REQUIRE( table.Size() == size );

        for (size_t i = 0; i < size; i += 2) {
            table.Erase(table.Begin());
        }
        REQUIRE( table.Size() == size >> 1 );
    }
}