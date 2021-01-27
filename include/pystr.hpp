#ifndef PY_STR_HPP
#define PY_STR_HPP
#include <pprint.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

namespace pystr {

using std::vector;
using std::basic_string;
using std::tuple;
using std::make_tuple;

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

template <class CharType>
inline basic_string<CharType>
strip(const basic_string<CharType> &s,
      const basic_string<CharType> &chars = basic_string<CharType>()) {
  if (chars.empty()) {
    auto start = std::find_if_not(s.begin(), s.end(), isspace);
    auto end = std::find_if_not(s.rbegin(), s.rend(), isspace);
    return s.substr(start - s.begin(),
                    (s.size() - (end - s.rbegin())) - (start - s.begin()));
  } else {
    auto start = std::find_if_not(s.begin(), s.end(), [&chars](CharType c) {
      return chars.find(c) != chars.npos;
    });
    auto end = std::find_if_not(s.rbegin(), s.rend(), [&chars](CharType c) {
      return chars.find(c) != chars.npos;
    });
    return s.substr(start - s.begin(),
                    (s.size() - (end - s.rbegin())) - (start - s.begin()));
  }
}

template <class CharType>
inline basic_string<CharType>
lstrip(const basic_string<CharType> &s,
       const basic_string<CharType> &chars = basic_string<CharType>()) {
  if (chars.empty()) {
    auto start = std::find_if_not(s.begin(), s.end(), isspace);
    return s.substr(start - s.begin(), s.end());
  } else {
    auto start = std::find_if_not(s.begin(), s.end(), [&chars](CharType c) {
      return chars.find(c) != chars.npos;
    });
    return s.substr(start - s.begin(), s.end());
  }
}

template <class CharType>
inline basic_string<CharType>
rstrip(const basic_string<CharType> &s,
       const basic_string<CharType> &chars = basic_string<CharType>()) {
  if (chars.empty()) {
    auto end = std::find_if_not(s.rbegin(), s.rend(), isspace);
    return s.substr(s.begin(),
                    (s.size() - (end - s.rbegin())) - (start - s.begin()));
  } else {
    auto end = std::find_if_not(s.rbegin(), s.rend(), [&chars](CharType c) {
      return chars.find(c) != chars.npos;
    });
    return s.substr(s.begin(),
                    (s.size() - (end - s.rbegin())) - (start - s.begin()));
  }
}

template <class CharType>
inline tuple<basic_string<CharType>, basic_string<CharType>,
             basic_string<CharType>>
partition(const basic_string<CharType> &s,
          const basic_string<CharType> &separator) {
  auto index = s.find(separator);
  if (index == s.npos) {
    return make_tuple(s, basic_string<CharType>(), basic_string<CharType>());
  } else {
    return make_tuple(s.substr(0, index), separator,
                      s.substr(index + separator.size(),
                               s.size() - (index + separator.size())));
  }
}

template <class CharType>
inline tuple<basic_string<CharType>, basic_string<CharType>,
             basic_string<CharType>>
rpartition(const basic_string<CharType> &s,
           const basic_string<CharType> &separator) {
  auto index = s.rfind(separator);
  if (index == s.npos) {
    return make_tuple(basic_string<CharType>(), basic_string<CharType>(), s);
  } else {
    return make_tuple(s.substr(0, s.size() - (index - separator.size())),
                      separator,
                      s.substr(index + separator.size(), s.size() - index));
  }
}

template <class CharType>
inline tuple<basic_string<CharType>, basic_string<CharType>,
             basic_string<CharType>>
join(const vector<basic_string<CharType>> &strs,
     const basic_string<CharType> &separator) {
  basic_string<CharType>::size_type length = 0;
  for (size_t i = 0; i < strs.size(); i++) {
    if (i == 0) {
      length = length + strs[i].size();
    } else {
      length = length + separator.size();
      length = length + strs[i].size();
    }
  }
  basic_string<CharType> result;
  result.reserve(length);
  for (size_t i = 0; i < strs.size(); i++) {
    if (i == 0) {
      result += strs[i];
    } else {
      result += separator;
      result += strs[i];
    }
  }
  return result;
}

template <class CharType>
inline bool startswith(const basic_string<CharType> &s,
                       const basic_string<CharType> &prefix,
                       typename basic_string<CharType>::size_type start = 0,
                       typename basic_string<CharType>::size_type end =
                           basic_string<CharType>::npos) {
  if (end == basic_string<CharType>::npos) {
    if (prefix.size() + start > s.size()) {
      return false;
    }
  } else {
    if (prefix.size() + start > s.size() || prefix.size() < end) {
      return false;
    }
  }
  for (size_t i = 0; i < prefix.size(); i++) {
    if (s[start + i] == prefix[i]) {
      // pass
    } else {
      return false;
    }
  }
  return true;
}

}; // namespace pystr

#endif // PY_STR_HPP