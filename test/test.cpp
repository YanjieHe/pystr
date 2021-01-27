#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <pprint.hpp>
#include "pystr.hpp"

using std::vector;
using std::string;
using std::make_tuple;

TEST_CASE("split strings from left to right", "[pystr::split]") {
  REQUIRE(pystr::split<char>("a, b, c", ", ") == vector<string>{"a", "b", "c"});
  REQUIRE(pystr::split<char>("", "/", 1) == vector<string>{""});
  REQUIRE(pystr::split<char>("/", "/", 1) == vector<string>{"", ""});
  REQUIRE(pystr::split<char>(" ", " ", 1) == vector<string>{"", ""});
  REQUIRE(pystr::split<char>(" /", "/", 1) == vector<string>{" ", ""});
  REQUIRE(pystr::split<char>(" //", "/", 1) == vector<string>{" ", "/"});
  REQUIRE(pystr::split<char>("a  ", " ", 1) == vector<string>{"a", " "});
  REQUIRE(pystr::split<char>("//as//rew//gdf", "//") ==
          vector<string>{"", "as", "rew", "gdf"});
  REQUIRE(pystr::split<char>("/root/world", "/", 0) ==
          vector<string>{"/root/world"});
  REQUIRE(pystr::split<char>("/root/world", "/", 1) ==
          vector<string>{"", "root/world"});
  REQUIRE(pystr::split<char>("/root/world", "/", 2) ==
          vector<string>{"", "root", "world"});
  REQUIRE(pystr::split<char>("/root/world", "/", -1) ==
          vector<string>{"", "root", "world"});
  REQUIRE(pystr::split<char>("a\r\nb \n\n", "") == vector<string>{"a", "b"});
}

TEST_CASE("split strings from right to left", "[pystr::rsplit]") {
  REQUIRE(pystr::rsplit<char>("", "/", 1) == vector<string>{""});
  REQUIRE(pystr::rsplit<char>("", "", 1) == vector<string>{""});
  REQUIRE(pystr::rsplit<char>(" ", "", 1) == vector<string>{});
  REQUIRE(pystr::rsplit<char>("  ", "", 1) == vector<string>{});
  REQUIRE(pystr::rsplit<char>("/", "/", 1) == vector<string>{"", ""});
  REQUIRE(pystr::rsplit<char>(" /", "/", 1) == vector<string>{" ", ""});
  REQUIRE(pystr::rsplit<char>(" //", "/", 1) == vector<string>{" /", ""});
  REQUIRE(pystr::rsplit<char>("/root", "/", 1) == vector<string>{"", "root"});
  REQUIRE(pystr::rsplit<char>("/root/world", "/", 0) ==
          vector<string>{"/root/world"});
  REQUIRE(pystr::rsplit<char>("/root/world", "/", 1) ==
          vector<string>{"/root", "world"});
  REQUIRE(pystr::rsplit<char>("/root/world", "/", 2) ==
          vector<string>{"", "root", "world"});
  REQUIRE(pystr::rsplit<char>("/root/world", "/", -1) ==
          vector<string>{"", "root", "world"});
  REQUIRE(pystr::rsplit<char>(" root world", "", 0) ==
          vector<string>{" root world"});
  REQUIRE(pystr::rsplit<char>(" root world", "", 2) ==
          vector<string>{"root", "world"});
  REQUIRE(pystr::rsplit<char>(" root world", " ", 0) ==
          vector<string>{" root world"});
  REQUIRE(pystr::rsplit<char>(" root world", " ", 0) ==
          vector<string>{" root world"});
  REQUIRE(pystr::rsplit<char>(" root world", " ", 1) ==
          vector<string>{" root", "world"});
  REQUIRE(pystr::rsplit<char>(" root world", " ", 2) ==
          vector<string>{"", "root", "world"});
}

TEST_CASE("strip", "[pystr::strip]") {
  REQUIRE(pystr::strip<char>("") == "");
  REQUIRE(pystr::strip<char>("a") == "a");
  REQUIRE(pystr::strip<char>("a ") == "a");
  REQUIRE(pystr::strip<char>(" a") == "a");
  REQUIRE(pystr::strip<char>("\n a ") == "a");
  REQUIRE(pystr::strip<char>("\r\n a \r\n") == "a");
  REQUIRE(pystr::strip<char>("\r\n a \r\n\t") == "a");
}

TEST_CASE("partition", "[pystr::partition]") {
  REQUIRE(pystr::partition<char>("aaabbbcccbbbddd", "bbb") ==
          make_tuple("aaa", "bbb", "cccbbbddd"));
}

TEST_CASE("rpartition", "[pystr::rpartition]") {
  REQUIRE(pystr::rpartition<char>("aaabbbcccbbbddd", "bbb") ==
          make_tuple("aaabbbccc", "bbb", "ddd"));
}

TEST_CASE("startswith", "[pystr::startswith]") {
  REQUIRE(pystr::startswith<char>("", "") == true);
  REQUIRE(pystr::startswith<char>("", "a") == false);
  REQUIRE(pystr::startswith<char>("a", "") == true);
  REQUIRE(pystr::startswith<char>("abc", "ab") == true);
  REQUIRE(pystr::startswith<char>("abc", "abc") == true);
  REQUIRE(pystr::startswith<char>("abc", "abcd") == false);
  REQUIRE(pystr::startswith<char>("abcdef", "abc") == true);
  REQUIRE(pystr::startswith<char>("abcdef", "abc", 1) == false);
  REQUIRE(pystr::startswith<char>("abcdef", "bc", 1) == true);
}