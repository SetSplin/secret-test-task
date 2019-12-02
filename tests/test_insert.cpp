#include <string>

#include <catch.h>
#include <hashtable.h>

TEST_CASE( "Insert" ) {
    SECTION( "return value" ) {
        Hashtable<std::string, int> table;
        auto result = table.Insert("asd", 42);
        REQUIRE( (*(result.first)).first == "asd" );
        REQUIRE( (*(result.first)).second == 42 );
        REQUIRE( result.second );

        result = table.Insert("qwe", 69);
        REQUIRE( result.first->first == "qwe" );
        REQUIRE( result.first->second == 69 );
        REQUIRE( result.second );

        result = table.Insert("qwe", 42);
        REQUIRE( !result.second );
        REQUIRE( result.first == table.End() );

        REQUIRE( table.Size() == 2 );
    }
    SECTION( "large table" ) {
        size_t size = 10000;
        Hashtable<size_t, size_t> table;
        for (size_t i = 0; i < size; ++i) {
            table.Insert(i, i);
        }
        REQUIRE( table.Size() == size );
    }
}