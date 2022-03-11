#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <pprint.hpp>
#include "pystr.hpp"

using std::make_tuple;
using std::string;
using std::vector;

TEST_CASE("split strings from left to right", "[pystr::split]")
{
        REQUIRE(pystr::split<string>("a, b, c", ", ") ==
                vector<string>{"a", "b", "c"});
        REQUIRE(pystr::split<string>("", "/", 1) == vector<string>{""});
        REQUIRE(pystr::split<string>("/", "/", 1) == vector<string>{"", ""});
        REQUIRE(pystr::split<string>(" ", " ", 1) == vector<string>{"", ""});
        REQUIRE(pystr::split<string>(" /", "/", 1) == vector<string>{" ", ""});
        REQUIRE(pystr::split<string>(" //", "/", 1) == vector<string>{" ", "/"});
        REQUIRE(pystr::split<string>("a  ", " ", 1) == vector<string>{"a", " "});
        REQUIRE(pystr::split<string>("//as//rew//gdf", "//") == vector<string>{"", "as", "rew", "gdf"});
        REQUIRE(pystr::split<string>("/root/world", "/", 0) == vector<string>{"/root/world"});
        REQUIRE(pystr::split<string>("/root/world", "/", 1) == vector<string>{"", "root/world"});
        REQUIRE(pystr::split<string>("/root/world", "/", 2) == vector<string>{"", "root", "world"});
        REQUIRE(pystr::split<string>("/root/world", "/", -1) == vector<string>{"", "root", "world"});
        REQUIRE(pystr::split<string>("a\r\nb \n\n", "") == vector<string>{"a", "b"});
}

TEST_CASE("split strings from right to left", "[pystr::rsplit]")
{
        REQUIRE(pystr::rsplit<string>("", "/", 1) == vector<string>{""});
        REQUIRE(pystr::rsplit<string>("", "", 1) == vector<string>{""});
        REQUIRE(pystr::rsplit<string>(" ", "", 1) == vector<string>{});
        REQUIRE(pystr::rsplit<string>("  ", "", 1) == vector<string>{});
        REQUIRE(pystr::rsplit<string>("/", "/", 1) == vector<string>{"", ""});
        REQUIRE(pystr::rsplit<string>(" /", "/", 1) == vector<string>{" ", ""});
        REQUIRE(pystr::rsplit<string>(" //", "/", 1) == vector<string>{" /", ""});
        REQUIRE(pystr::rsplit<string>("/root", "/", 1) == vector<string>{"", "root"});
        REQUIRE(pystr::rsplit<string>("/root/world", "/", 0) == vector<string>{"/root/world"});
        REQUIRE(pystr::rsplit<string>("/root/world", "/", 1) == vector<string>{"/root", "world"});
        REQUIRE(pystr::rsplit<string>("/root/world", "/", 2) == vector<string>{"", "root", "world"});
        REQUIRE(pystr::rsplit<string>("/root/world", "/", -1) == vector<string>{"", "root", "world"});
        REQUIRE(pystr::rsplit<string>(" root world", "", 0) == vector<string>{" root world"});
        REQUIRE(pystr::rsplit<string>(" root world", "", 2) == vector<string>{"root", "world"});
        REQUIRE(pystr::rsplit<string>(" root world", " ", 0) == vector<string>{" root world"});
        REQUIRE(pystr::rsplit<string>(" root world", " ", 0) == vector<string>{" root world"});
        REQUIRE(pystr::rsplit<string>(" root world", " ", 1) == vector<string>{" root", "world"});
        REQUIRE(pystr::rsplit<string>(" root world", " ", 2) == vector<string>{"", "root", "world"});
}

TEST_CASE("strip", "[pystr::strip]")
{
        REQUIRE(pystr::strip<string>("") == "");
        REQUIRE(pystr::strip<string>("a") == "a");
        REQUIRE(pystr::strip<string>("a ") == "a");
        REQUIRE(pystr::strip<string>(" a") == "a");
        REQUIRE(pystr::strip<string>("\n a ") == "a");
        REQUIRE(pystr::strip<string>("\r\n a \r\n") == "a");
        REQUIRE(pystr::strip<string>("\r\n a \r\n\t") == "a");
}

TEST_CASE("partition", "[pystr::partition]")
{
        REQUIRE(pystr::partition<string>("aaabbbcccbbbddd", "bbb") == make_tuple("aaa", "bbb", "cccbbbddd"));
}

TEST_CASE("rpartition", "[pystr::rpartition]")
{
        REQUIRE(pystr::rpartition<string>("aaabbbcccbbbddd", "bbb") == make_tuple("aaabbbccc", "bbb", "ddd"));
}

TEST_CASE("startswith", "[pystr::startswith]")
{
        REQUIRE(pystr::startswith<string>("", "") == true);
        REQUIRE(pystr::startswith<string>("", "a") == false);
        REQUIRE(pystr::startswith<string>("a", "") == true);
        REQUIRE(pystr::startswith<string>("abc", "ab") == true);
        REQUIRE(pystr::startswith<string>("abc", "abc") == true);
        REQUIRE(pystr::startswith<string>("abc", "abcd") == false);
        REQUIRE(pystr::startswith<string>("abcdef", "abc") == true);
        REQUIRE(pystr::startswith<string>("abcdef", "abc", 1) == false);
        REQUIRE(pystr::startswith<string>("abcdef", "bc", 1) == true);
}

TEST_CASE("endswith", "[pystr::endswith]")
{
        REQUIRE(pystr::endswith<string>("", "") == true);
        REQUIRE(pystr::endswith<string>("", "a") == false);
        REQUIRE(pystr::endswith<string>("a", "") == true);
        REQUIRE(pystr::endswith<string>("", ".mesh") == false);
        REQUIRE(pystr::endswith<string>("help", ".mesh") == false);
        REQUIRE(pystr::endswith<string>("help", ".mesh", 0) == false);
        REQUIRE(pystr::endswith<string>("help", ".mesh", 1) == false);
        REQUIRE(pystr::endswith<string>("help", ".mesh", 1, 2) == false);
        REQUIRE(pystr::endswith<string>("help", ".mesh", 1, 1) == false);
        REQUIRE(pystr::endswith<string>("help", ".mesh", 1, -1) == false);
        REQUIRE(pystr::endswith<string>("help", ".mesh", -1) == false);
        REQUIRE(pystr::endswith<string>(".mesh", ".mesh", 0, 5) == true);
        REQUIRE(pystr::endswith<string>(".mesh", ".mesh") == true);
        REQUIRE(pystr::endswith<string>("a.mesh", ".mesh") == true);
        REQUIRE(pystr::endswith<string>("a.", ".") == true);
        REQUIRE(pystr::endswith<string>("abcdef", "ef") == true);
        REQUIRE(pystr::endswith<string>("abcdef", "cdef") == true);
        REQUIRE(pystr::endswith<string>("abcdef", "cdef", 2) == true);
        REQUIRE(pystr::endswith<string>("abcdef", "cdef", 3) == false);
        REQUIRE(pystr::endswith<string>("abcdef", "cdef", 2, 3) == false);
        REQUIRE(pystr::endswith<string>("abcdef", "cdef", -10) == true);
}