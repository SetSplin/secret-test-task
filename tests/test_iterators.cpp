#include <string>

#include <catch.h>
#include <hashtable.h>

TEST_CASE( "Iterators" ) {
    SECTION( "Mutating" ) {
        Hashtable<std::string, int> table;
        table.Insert("asd", 42);
        auto element = table.Begin();
        element->second = 123;
        REQUIRE( table.Begin()->second == 123 );
    }
}
