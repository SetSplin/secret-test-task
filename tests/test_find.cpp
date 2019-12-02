#include <string>

#include <catch.h>
#include <hashtable.h>

TEST_CASE( "Find" ) {
    SECTION( "simple case" ) {
        Hashtable<std::string, int> table;
        table.Insert("asd", 42);
        table.Insert("qwe", 69);

        auto result = table.Find("asd");
        REQUIRE( result->second == 42 );

        result = table.Find("qwe");
        REQUIRE( result->second == 69 );
    }

    SECTION( "large table" ) {
        size_t size = 10000;
        Hashtable<size_t, size_t> table;
        for (size_t i = 0; i < size; ++i) {
            table.Insert(i, size - i);
        }
        for (size_t i = 0; i < size; ++i) {
            REQUIRE( table.Find(i)->second == size - i );
        }
    }
}
