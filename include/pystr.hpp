#ifndef PY_STR_HPP
#define PY_STR_HPP
#include <pprint.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <execution>

namespace pystr {

using std::vector;
using std::basic_string;

template <class CharType>
inline vector<basic_string<CharType>>
split_whitespace(const basic_string<CharType> &s, int max_split = -1) {
  vector<basic_string<CharType>> result;
  int n_splits = 0;
  basic_string<CharType>::const_iterator start = s.begin();
  basic_string<CharType>::const_iterator end;
  start = std::find_if_not(s.begin(), s.end(), isspace);
  while (start != s.end()) {
    if (max_split != -1) {
      if (n_splits >= max_split) {
        result.push_back(s.substr(end - s.begin()));
        return result;
      } else {
        n_splits++;
      }
    }
    end = std::find_if(start, s.end(), isspace);
    result.push_back(s.substr(start - s.begin(), end - start));
    start = std::find_if_not(end, s.end(), isspace);
  }
  return result;
}

template <class CharType>
inline vector<basic_string<CharType>>
rsplit_whitespace(const basic_string<CharType> &s, int max_split = -1) {
  vector<basic_string<CharType>> result;
  int n_splits = 0;
  basic_string<CharType>::const_reverse_iterator start = s.rbegin();
  basic_string<CharType>::const_reverse_iterator end;
  start = std::find_if_not(s.rbegin(), s.rend(), isspace);
  while (start != s.rend()) {
    if (max_split != -1) {
      if (n_splits >= max_split) {
        result.push_back(s.substr(0, s.size() - (end - s.rbegin())));
        std::reverse(result.begin(), result.end());
        return result;
      } else {
        n_splits++;
      }
    }
    end = std::find_if(start, s.rend(), isspace);
    result.push_back(s.substr(s.size() - (end - s.rbegin()), end - start));
    start = std::find_if_not(end, s.rend(), isspace);
  }
  std::reverse(result.begin(), result.end());
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

template <class CharType>
inline vector<basic_string<CharType>>
rsplit(const basic_string<CharType> &s, const basic_string<CharType> &separator,
       int max_split = -1) {
  if (max_split == -1) {
    return split(s, separator, max_split);
  } else if (s.empty()) {
    return {s};
  } else if (separator.empty()) {
    return rsplit_whitespace(s, max_split);
  } else {
    basic_string<CharType>::size_type p1 = s.size() - 1;
    basic_string<CharType>::size_type p2 = 0;
    vector<basic_string<CharType>> result;
    int n_splits = 0;

    p2 = s.rfind(separator, p1);
    while (p2 != basic_string<CharType>::npos) {
      if (max_split != -1) {
        if (n_splits >= max_split) {
          result.push_back(s.substr(0, p1 + 1));
          std::reverse(result.begin(), result.end());
          return result;
        } else {
          n_splits++;
        }
      }
      result.push_back(
          s.substr(p2 + separator.size(), p1 - p2 - separator.size() + 1));
      p1 = p2 - 1;
      p2 = s.rfind(separator, p1);
    }
    result.push_back(s.substr(0, p1 + 1));
    std::reverse(result.begin(), result.end());
    return result;
  }
}

}; // namespace pystr

#endif // PY_STR_HPP