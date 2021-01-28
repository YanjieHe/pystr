#ifndef PY_STR_HPP
#define PY_STR_HPP
#include <pprint.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <optional>

namespace pystr {

using std::vector;
using std::basic_string;
using std::tuple;
using std::make_tuple;
using std::optional;

namespace internal {
inline void adjust_start(int& start, int length) {
    if (start < 0) {
        start = start + length;
        if (start < 0) {
            start = 0;
        }
    }
}
inline void adjust_end(int& end, int length) {
    if (end > length) {
        end = length;
    } else if (end < 0) {
        end = end + length;
        if (end < 0) {
            end = 0;
        }
    }
}
inline void adjust_slice(int& start, int& end, int length) {
    adjust_start(start, length);
    adjust_end(end, length);
}
}; // namespace internal

template <class CharType>
inline int len(const basic_string<CharType>& s) {
    return static_cast<int>(s.size());
}

template <class CharType>
inline vector<basic_string<CharType>>
split_whitespace(const basic_string<CharType>& s, int max_split = -1) {
    vector<basic_string<CharType>> result;
    int n_splits = 0;
    typename basic_string<CharType>::const_iterator start = s.begin();
    typename basic_string<CharType>::const_iterator end;
    start = std::find_if_not(s.begin(), s.end(),
                             [](CharType c) { return std::isspace(c); });
    while (start != s.end()) {
        if (max_split != -1) {
            if (n_splits >= max_split) {
                result.push_back(s.substr(end - s.begin()));
                return result;
            } else {
                n_splits++;
            }
        }
        end = std::find_if(start, s.end(),
                           [](CharType c) { return std::isspace(c); });
        result.push_back(s.substr(start - s.begin(), end - start));
        start = std::find_if_not(end, s.end(),
                                 [](CharType c) { return std::isspace(c); });
    }
    return result;
}

template <class CharType>
inline vector<basic_string<CharType>>
rsplit_whitespace(const basic_string<CharType>& s, int max_split = -1) {
    vector<basic_string<CharType>> result;
    int n_splits = 0;
    typename basic_string<CharType>::const_reverse_iterator start = s.rbegin();
    typename basic_string<CharType>::const_reverse_iterator end;
    start = std::find_if_not(s.rbegin(), s.rend(),
                             [](CharType c) { return std::isspace(c); });
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
        end = std::find_if(start, s.rend(),
                           [](CharType c) { return std::isspace(c); });
        result.push_back(s.substr(s.size() - (end - s.rbegin()), end - start));
        start = std::find_if_not(end, s.rend(),
                                 [](CharType c) { return std::isspace(c); });
    }
    std::reverse(result.begin(), result.end());
    return result;
}

template <class CharType>
inline vector<basic_string<CharType>>
split(const basic_string<CharType>& s, const basic_string<CharType>& separator,
      int max_split = -1) {
    if (separator.empty()) {
        return split_whitespace<CharType>(s, max_split);
    } else {
        typename basic_string<CharType>::size_type p1 = 0;
        typename basic_string<CharType>::size_type p2 = 0;
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
rsplit(const basic_string<CharType>& s, const basic_string<CharType>& separator,
       int max_split = -1) {
    if (max_split == -1) {
        return split(s, separator, max_split);
    } else if (s.empty()) {
        return {s};
    } else if (separator.empty()) {
        return rsplit_whitespace(s, max_split);
    } else {
        typename basic_string<CharType>::size_type p1 = s.size() - 1;
        typename basic_string<CharType>::size_type p2 = 0;
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
            result.push_back(s.substr(p2 + separator.size(),
                                      p1 - p2 - separator.size() + 1));
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
strip(const basic_string<CharType>& s,
      const basic_string<CharType>& chars = basic_string<CharType>()) {
    if (chars.empty()) {
        auto start = std::find_if_not(
            s.begin(), s.end(), [](CharType c) { return std::isspace(c); });
        auto end = std::find_if_not(s.rbegin(), s.rend(),
                                    [](CharType c) { return std::isspace(c); });
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
lstrip(const basic_string<CharType>& s,
       const basic_string<CharType>& chars = basic_string<CharType>()) {
    if (chars.empty()) {
        auto start = std::find_if_not(s.begin(), s.end(), std::isspace);
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
rstrip(const basic_string<CharType>& s,
       const basic_string<CharType>& chars = basic_string<CharType>()) {
    if (chars.empty()) {
        auto end = std::find_if_not(s.rbegin(), s.rend(), std::isspace);
        return s.substr(s.begin(), (s.size() - (end - s.rbegin())));
    } else {
        auto end = std::find_if_not(s.rbegin(), s.rend(), [&chars](CharType c) {
            return chars.find(c) != chars.npos;
        });
        return s.substr(s.begin(), (s.size() - (end - s.rbegin())));
    }
}

template <class CharType>
inline tuple<basic_string<CharType>, basic_string<CharType>,
             basic_string<CharType>>
partition(const basic_string<CharType>& s,
          const basic_string<CharType>& separator) {
    auto index = s.find(separator);
    if (index == s.npos) {
        return make_tuple(s, basic_string<CharType>(),
                          basic_string<CharType>());
    } else {
        return make_tuple(s.substr(0, index), separator,
                          s.substr(index + separator.size(),
                                   s.size() - (index + separator.size())));
    }
}

template <class CharType>
inline tuple<basic_string<CharType>, basic_string<CharType>,
             basic_string<CharType>>
rpartition(const basic_string<CharType>& s,
           const basic_string<CharType>& separator) {
    auto index = s.rfind(separator);
    if (index == s.npos) {
        return make_tuple(basic_string<CharType>(), basic_string<CharType>(),
                          s);
    } else {
        return make_tuple(s.substr(0, s.size() - (index - separator.size())),
                          separator,
                          s.substr(index + separator.size(), s.size() - index));
    }
}

template <class CharType>
inline tuple<basic_string<CharType>, basic_string<CharType>,
             basic_string<CharType>>
join(const vector<basic_string<CharType>>& strs,
     const basic_string<CharType>& separator) {
    typename basic_string<CharType>::size_type length = 0;
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
inline bool startswith(const basic_string<CharType>& s,
                       const basic_string<CharType>& prefix, int start = 0,
                       optional<int> end = {}) {
    if (end.has_value()) {
        if (prefix.size() + start > s.size() || prefix.size() < end.value()) {
            return false;
        }
    } else {
        if (prefix.size() + start > s.size()) {
            return false;
        }
    }
    for (size_t i = 0; i < prefix.size(); i++) {
        if (s[start + i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

template <class CharType>
inline bool endswith(const basic_string<CharType>& s,
                     const basic_string<CharType>& prefix,
                     optional<int> start = {}, optional<int> end = {}) {
    if (start.has_value()) {
        if (end.has_value()) {
            internal::adjust_slice(start.value(), end.value(), len(s));
            if (start.value() + prefix.size() > std::min(len(s), end.value())) {
                return false;
            }
        } else {
            internal::adjust_start(start.value(), len(s));
            if (start.value() + prefix.size() > s.size()) {
                return false;
            }
            end = len(s);
        }
    } else {
        if (end.has_value()) {
            internal::adjust_end(end.value(), len(s));
            if (prefix.size() > std::min(len(s), end.value())) {
                return false;
            }
        } else {
            if (prefix.size() > s.size()) {
                return false;
            }
            end = len(s);
        }
    }
    for (size_t i = 0; i < prefix.size(); i++) {
        if (s[end.value() - i] != prefix[prefix.size() - i]) {
            return false;
        }
    }
    return true;
}

template <class CharType>
inline void capitalize(basic_string<CharType>& s) {
    if (s.empty()) {
        return;
    } else {
        if (std::islower(s[0])) {
            s[0] = std::toupper(s[0]);
        }
        for (size_t i = 1; i < s.size(); i++) {
            if (std::isupper(s[i])) {
                s[i] = std::tolower(s[i]);
            }
        }
    }
}

template <class CharType>
inline bool isalnum(const basic_string<CharType>& s) {
    return std::all_of(s.begin(), s.end(), std::isalnum);
}

template <class CharType>
inline bool isalpha(const basic_string<CharType>& s) {
    return std::all_of(s.begin(), s.end(), std::isalpha);
}

template <class CharType>
inline bool isdigit(const basic_string<CharType>& s) {
    return std::all_of(s.begin(), s.end(), std::isdigit);
}

template <class CharType>
inline bool islower(const basic_string<CharType>& s) {
    return std::all_of(s.begin(), s.end(), std::islower);
}

template <class CharType>
inline bool isupper(const basic_string<CharType>& s) {
    return std::all_of(s.begin(), s.end(), std::isupper);
}

template <class CharType>
inline bool isspace(const basic_string<CharType>& s) {
    return std::all_of(s.begin(), s.end(), std::isspace);
}

template <class CharType>
inline void lower(basic_string<CharType>& s) {
    std::transform(s.begin(), s.end(), s.begin(), std::tolower);
}

template <class CharType>
inline void upper(basic_string<CharType>& s) {
    std::transform(s.begin(), s.end(), s.begin(), std::toupper);
}

template <class CharType>
inline void swapcase(basic_string<CharType>& s) {
    std::transform(s.begin(), s.end(), s.begin(), [](CharType c) {
        if (std::isupper(c)) {
            return std::tolower(c);
        } else if (std::islower(c)) {
            return std::toupper(c);
        } else {
            return c;
        }
    });
}

}; // namespace pystr

#endif // PY_STR_HPP