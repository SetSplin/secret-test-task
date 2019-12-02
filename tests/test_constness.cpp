#include <string>

#include <catch.h>
#include <hashtable.h>

TEST_CASE( "Constness" ) {
    Hashtable<std::string, int> table;
    table.Insert("asd", 42);
    table.Insert("qwe", 69);
    SECTION( "Find" ) {
        const Hashtable<std::string, int>& const_table = table;
        auto result = const_table.Find("asd");
        REQUIRE( result->second == 42 );

        result = const_table.Find("qwe");
        REQUIRE( result->second == 69 );

        REQUIRE( const_table.Size() == 2 );
    }
}