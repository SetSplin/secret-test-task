#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <string>
#include <vector>
#include <map>

#include <catch.h>
#include <hashtable.h>

TEST_CASE( "Constructors" ) {
    SECTION( "builtin primitives" ) {
        {
            Hashtable<int, int> table;
            REQUIRE( table.Size() == 0 );
        }
        {
            Hashtable<int, char> table;
            REQUIRE( table.Size() == 0 );
        }
        {
            Hashtable<int, bool> table;
            REQUIRE( table.Size() == 0 );
        }
        {
            Hashtable<char, int> table;
            REQUIRE( table.Size() == 0 );
        }
        {
            Hashtable<char*, int> table;
            REQUIRE( table.Size() == 0 );
        }
    }
    SECTION( "builtin class types" ) {
        {
            Hashtable<std::string, int> table;
            REQUIRE( table.Size() == 0 );
        }
        {
            Hashtable<int, std::string> table;
            REQUIRE( table.Size() == 0 );
        }
    }
}

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

        REQUIRE( table.Size() == 2 );
    }
}

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
        REQUIRE( table.Size() == size / 2 );
    }
}

TEST_CASE( "Find" ) {
    SECTION( "key exists" ) {
        Hashtable<std::string, int> table;
        table.Insert("asd", 42);
        table.Insert("qwe", 69);

        auto result = table.Find("asd");
        REQUIRE( result->second == 42 );

        result = table.Find("qwe");
        REQUIRE( result->second == 69 );
    }
}

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

TEST_CASE( "Iterators" ) {
    SECTION( "Mutating" ) {
        Hashtable<std::string, int> table;
        table.Insert("asd", 42);
        auto element = table.Begin();
        element->second = 123;
        REQUIRE( table.Begin()->second == 123 );
    }
}

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