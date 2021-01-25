#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <pprint.hpp>
#include "pystr.hpp"

using std::vector;
using std::string;

TEST_CASE("split", "[pystr::split]") {
  pprint::PrettyPrinter printer;
  auto res = pystr::split<char>("a::b::c", "::", 1);
  printer.print("size = ", res.size(), "values = ", res);
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
}