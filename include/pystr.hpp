#ifndef PY_STR_HPP
#define PY_STR_HPP

#include <string>
#include <vector>

namespace pystr {

using std::vector;
using std::basic_string;

template <class CharType>
inline vector<basic_string<CharType>>
split(const basic_string<CharType> &s, const basic_string<CharType> &separator,
      int maxsplit = -1) {
  basic_string<CharType>::size_type p1 = 0;
  basic_string<CharType>::size_type p2 = 0;
  vector<basic_string<CharType>> result;
  int n_splits = 0;

  p2 = s.find(separator, p1);
  while (p2 != basic_string<CharType>::npos) {
    if (maxsplit != -1) {
      if (n_splits >= maxsplit) {
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

}; // namespace pystr

#endif // PY_STR_HPP