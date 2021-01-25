#ifndef PY_STR_HPP
#define PY_STR_HPP
#include <pprint.hpp>

#include <string>
#include <vector>
#include <algorithm>

namespace pystr {

using std::vector;
using std::basic_string;

template <class CharType>
inline vector<basic_string<CharType>>
split_whitespace(const basic_string<CharType> &s, int max_split = -1) {
  basic_string<CharType>::size_type p1 = 0;
  basic_string<CharType>::size_type p2 = 0;
  basic_string<CharType>::size_type skip = 0;
  vector<basic_string<CharType>> result;
  int n_splits = 0;

  auto find_whitespace = [&s, &p1,
                          &skip]() -> basic_string<CharType>::size_type {
    auto i1 = std::find_if(s.begin() + p1, s.end(),
                           [](const CharType &c) { return isspace(c); });
    auto i2 = std::find_if_not(i1, s.end(),
                               [](const CharType &c) { return isspace(c); });
    skip = i2 - i1;
    if (i1 == s.end()) {
      return basic_string<CharType>::npos;
    } else {
      return i1 - s.begin();
    }
  };

  p2 = find_whitespace();
  while (p2 != basic_string<CharType>::npos) {
    if (max_split != -1) {
      if (n_splits >= max_split) {
        result.push_back(s.substr(p1));
        return result;
      } else {
        n_splits++;
      }
    }
    result.push_back(s.substr(p1, p2 - p1));
    p1 = p2 + skip;
    p2 = find_whitespace();
  }

  if (p1 < s.size()) {
    result.push_back(s.substr(p1));
  }
  return result;
}

template <class CharType>
inline vector<basic_string<CharType>>
split(const basic_string<CharType> &s, const basic_string<CharType> &separator,
      int max_split = -1) {
  if (separator.empty()) {
    return split_whitespace<CharType>(s, max_split);
  } else {
    basic_string<CharType>::size_type p1 = 0;
    basic_string<CharType>::size_type p2 = 0;
    vector<basic_string<CharType>> result;
    int n_splits = 0;

    p2 = s.find(separator, p1);
    while (p2 != basic_string<CharType>::npos) {
      if (max_split != -1) {
        if (n_splits >= max_split) {
          result.push_back(s.substr(p1));
          return result;
        } else {
          n_splits++;
        }
      }
      result.push_back(s.substr(p1, p2 - p1));
      p1 = p2 + separator.size();
      p2 = s.find(separator, p1);
    }
    result.push_back(s.substr(p1));
    return result;
  }
}

}; // namespace pystr

#endif // PY_STR_HPP