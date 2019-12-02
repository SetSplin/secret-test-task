#include <string>

#include <catch.h>
#include <hashtable.h>

TEST_CASE( "Operator[]" ) {
    SECTION( "key exists" ) {
        Hashtable<std::string, int> table;
        table.Insert("asd", 42);
        table.Insert("qwe", 69);

        REQUIRE( table["asd"] == 42 );
        REQUIRE( table["qwe"] == 69 );
    }

    SECTION( "key does not exists", "[int]" ) {
        Hashtable<std::string, int> table;

        REQUIRE( table["asd"] == 0 );
        REQUIRE( table["qwe"] == 0 );

        REQUIRE( table.Size() == 2 );
    }

    SECTION( "key does not exists", "[string]" ) {
        Hashtable<std::string, std::string> table;

        REQUIRE( table["asd"] == "" );
        REQUIRE( table["qwe"] == "" );

        REQUIRE( table.Size() == 2 );
    }
}