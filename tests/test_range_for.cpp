#include <string>
#include <map>

#include <catch.h>
#include <hashtable.h>

TEST_CASE( "Range based for" ) {
    SECTION( "non const case" ) {
        Hashtable<size_t, size_t> table;
        std::map<size_t, size_t> check;
        size_t size = 1000;
        for (size_t i = 0; i < size; ++i) {
            table.Insert(i, i);
            check[i] = 0;
        }

        for (auto it : table) {
            check[it.first] += 1;
            REQUIRE( it.first == it.second );
        }

        for (size_t i = 0; i < size; ++i) {
            REQUIRE( check[i] == 1 );
        }
        REQUIRE( check.size() == size );
    }

    SECTION( "const case" ) {
        Hashtable<size_t, size_t> table;
        std::map<size_t, size_t> check;
        size_t size = 1000;
        for (size_t i = 0; i < size; ++i) {
            table.Insert(i, i);
            check[i] = 0;
        }

        const Hashtable<size_t, size_t>& const_table = table;

        for (const auto it : const_table) {
            check[it.first] += 1;
            REQUIRE( it.first == it.second );
        }

        for (size_t i = 0; i < size; ++i) {
            REQUIRE( check[i] == 1 );
        }
        REQUIRE( check.size() == size );
    }
}
