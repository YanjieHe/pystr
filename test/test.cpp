#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <pprint.hpp>
#include "pystr.hpp"

using std::vector;
using std::string;

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