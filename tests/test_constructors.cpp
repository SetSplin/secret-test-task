#include <string>
#include <vector>
#include <map>

#include <catch.h>
#include <hashtable.h>
#include <simple_int.h>

TEST_CASE( "Constructors no params" ) {
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
        {
            Hashtable<std::string, std::vector<std::string>> table;
            REQUIRE( table.Size() == 0 );
        }
    }
    SECTION( "user defined classes" ) {
        {
            Int::Reset();
            Hashtable<std::string, Int> table;
            REQUIRE( table.Size() == 0 );
            REQUIRE( Int::created == 0 );
        }
    }
}

TEST_CASE( "Constructors initializer list" ) {
    SECTION( "builtin primitives" ) {
        {
            Hashtable<int, int> table{ {1, 2}, {3, 4}, {5, 6} };
            REQUIRE( table.Size() == 3 );
            REQUIRE( table[1] == 2 );
            REQUIRE( table[3] == 4 );
            REQUIRE( table[5] == 6 );
        }
    }

    SECTION( "builtin class types" ) {
        {
            Hashtable<std::string, int> table{ {"qwe", 2}, {"asd", 4}, {"rty", 6} };
            REQUIRE( table.Size() == 3 );
            REQUIRE( table["qwe"] == 2 );
            REQUIRE( table["asd"] == 4 );
            REQUIRE( table["rty"] == 6 );
        }
    }
    SECTION( "user defined classes" ) {
        {
            Int::Reset();
            Hashtable<std::string, Int> table{ {"qwe", Int(2)}, {"asd", Int(4)}, {"rty", Int(6)} };
            REQUIRE( Int::created == 9 );
            REQUIRE( table.Size() == 3 );
            REQUIRE( table["qwe"] == Int(2) );
            REQUIRE( table["asd"] == Int(4) );
            REQUIRE( table["rty"] == Int(6) );
            REQUIRE( Int::created == 12 );
        }
        REQUIRE( Int::created == Int::destroyed );
    }
}

TEST_CASE( "Constructors copy constructor" ) {
    SECTION( "builtin primitives" ) {
        {
            Hashtable<int, int> table{ {1, 2}, {3, 4}, {5, 6} };
            Hashtable<int, int> copy_table(table);
            REQUIRE( copy_table.Size() == 3 );
            REQUIRE( copy_table[1] == 2 );
            REQUIRE( copy_table[3] == 4 );
            REQUIRE( copy_table[5] == 6 );

            REQUIRE( table.Size() == 3 );
            REQUIRE( table[1] == 2 );
            REQUIRE( table[3] == 4 );
            REQUIRE( table[5] == 6 );
        }
    }

    SECTION( "builtin class types" ) {
        {
            Hashtable<std::string, int> table{ {"qwe", 2}, {"asd", 4}, {"rty", 6} };
            Hashtable<std::string, int> copy_table(table);
            REQUIRE( copy_table.Size() == 3 );
            REQUIRE( copy_table["qwe"] == 2 );
            REQUIRE( copy_table["asd"] == 4 );
            REQUIRE( copy_table["rty"] == 6 );

            REQUIRE( table.Size() == 3 );
            REQUIRE( table["qwe"] == 2 );
            REQUIRE( table["asd"] == 4 );
            REQUIRE( table["rty"] == 6 );
        }
    }
    SECTION( "user defined classes" ) {
        {
            Int::Reset();
            Hashtable<std::string, Int> table{ {"qwe", Int(2)}, {"asd", Int(4)}, {"rty", Int(6)} };
            Hashtable<std::string, Int> copy_table = table;
            REQUIRE( copy_table.Size() == 3 );
            REQUIRE( Int::created == 12 );
            REQUIRE( copy_table["qwe"] == Int(2) );
            REQUIRE( copy_table["asd"] == Int(4) );
            REQUIRE( copy_table["rty"] == Int(6) );
            REQUIRE( Int::created == 15 );
        }
        REQUIRE( Int::created == Int::destroyed );
    }
}

TEST_CASE( "Constructors move constructor" ) {
    SECTION( "builtin primitives" ) {
        {
            Hashtable<int, int> table{ {1, 2}, {3, 4}, {5, 6} };
            Hashtable<int, int> copy_table = std::move(table);
            REQUIRE( copy_table.Size() == 3 );
            REQUIRE( copy_table[1] == 2 );
            REQUIRE( copy_table[3] == 4 );
            REQUIRE( copy_table[5] == 6 );
            REQUIRE( table.Size() == 0 );
        }
    }

    SECTION( "builtin class types" ) {
        {
            Hashtable<std::string, int> table{ {"qwe", 2}, {"asd", 4}, {"rty", 6} };
            Hashtable<std::string, int> copy_table = std::move(table);
            REQUIRE( copy_table.Size() == 3 );
            REQUIRE( copy_table["qwe"] == 2 );
            REQUIRE( copy_table["asd"] == 4 );
            REQUIRE( copy_table["rty"] == 6 );
            REQUIRE( table.Size() == 0 );
        }
    }

    SECTION( "user defined classes" ) {
        {
            Int::Reset();
            Hashtable<std::string, Int> table{ {"qwe", Int(2)}, {"asd", Int(4)}, {"rty", Int(6)} };
            Hashtable<std::string, Int> copy_table = std::move(table);
            REQUIRE( copy_table.Size() == 3 );
            REQUIRE( Int::created == 9 );
            REQUIRE( copy_table["qwe"] == Int(2) );
            REQUIRE( copy_table["asd"] == Int(4) );
            REQUIRE( copy_table["rty"] == Int(6) );
            REQUIRE( Int::created == 12 );
            REQUIRE( table.Size() == 0 );
        }
        REQUIRE( Int::created == Int::destroyed );
    }
}