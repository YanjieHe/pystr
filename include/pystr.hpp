#ifndef PY_STR_HPP
#define PY_STR_HPP
#include <pprint.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <optional>

namespace pystr
{

    using std::basic_string;
    using std::make_tuple;
    using std::optional;
    using std::tuple;
    using std::vector;

    namespace internal
    {
        inline void adjust_start(int &start, int length)
        {
            if (start < 0)
            {
                start = start + length;
                if (start < 0)
                {
                    start = 0;
                }
            }
        }
        inline void adjust_end(int &end, int length)
        {
            if (end > length)
            {
                end = length;
            }
            else if (end < 0)
            {
                end = end + length;
                if (end < 0)
                {
                    end = 0;
                }
            }
        }
        inline void adjust_slice(int &start, int &end, int length)
        {
            adjust_start(start, length);
            adjust_end(end, length);
        }
    }; // namespace internal

    template <class StringType>
    inline int len(const StringType &s)
    {
        return static_cast<int>(s.size());
    }

    template <class StringType>
    inline vector<StringType>
    split_whitespace(const StringType &s, int max_split = -1)
    {
        vector<StringType> result;
        int n_splits = 0;
        typename StringType::const_iterator start = s.begin();
        typename StringType::const_iterator end;
        start = std::find_if_not(s.begin(), s.end(),
                                 [](auto c)
                                 { return std::isspace(c); });
        while (start != s.end())
        {
            if (max_split != -1)
            {
                if (n_splits >= max_split)
                {
                    result.push_back(s.substr(end - s.begin()));
                    return result;
                }
                else
                {
                    n_splits++;
                }
            }
            end = std::find_if(start, s.end(),
                               [](auto c)
                               { return std::isspace(c); });
            result.push_back(s.substr(start - s.begin(), end - start));
            start = std::find_if_not(end, s.end(),
                                     [](auto c)
                                     { return std::isspace(c); });
        }
        return result;
    }

    template <class StringType>
    inline vector<StringType>
    rsplit_whitespace(const StringType &s, int max_split = -1)
    {
        vector<StringType> result;
        int n_splits = 0;
        typename StringType::const_reverse_iterator start = s.rbegin();
        typename StringType::const_reverse_iterator end;
        start = std::find_if_not(s.rbegin(), s.rend(),
                                 [](auto c)
                                 { return std::isspace(c); });
        while (start != s.rend())
        {
            if (max_split != -1)
            {
                if (n_splits >= max_split)
                {
                    result.push_back(s.substr(0, s.size() - (end - s.rbegin())));
                    std::reverse(result.begin(), result.end());
                    return result;
                }
                else
                {
                    n_splits++;
                }
            }
            end = std::find_if(start, s.rend(),
                               [](auto c)
                               { return std::isspace(c); });
            result.push_back(s.substr(s.size() - (end - s.rbegin()), end - start));
            start = std::find_if_not(end, s.rend(),
                                     [](auto c)
                                     { return std::isspace(c); });
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    template <class StringType>
    inline vector<StringType>
    split(const StringType &s, const StringType &separator,
          int max_split = -1)
    {
        if (separator.empty())
        {
            return split_whitespace<StringType>(s, max_split);
        }
        else
        {
            typename StringType::size_type p1 = 0;
            typename StringType::size_type p2 = 0;
            vector<StringType> result;
            int n_splits = 0;

            p2 = s.find(separator, p1);
            while (p2 != StringType::npos)
            {
                if (max_split != -1)
                {
                    if (n_splits >= max_split)
                    {
                        result.push_back(s.substr(p1));
                        return result;
                    }
                    else
                    {
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

    template <class StringType>
    inline vector<StringType>
    rsplit(const StringType &s, const StringType &separator,
           int max_split = -1)
    {
        if (max_split == -1)
        {
            return split(s, separator, max_split);
        }
        else if (s.empty())
        {
            return {s};
        }
        else if (separator.empty())
        {
            return rsplit_whitespace(s, max_split);
        }
        else
        {
            typename StringType::size_type p1 = s.size() - 1;
            typename StringType::size_type p2 = 0;
            vector<StringType> result;
            int n_splits = 0;

            p2 = s.rfind(separator, p1);
            while (p2 != StringType::npos)
            {
                if (max_split != -1)
                {
                    if (n_splits >= max_split)
                    {
                        result.push_back(s.substr(0, p1 + 1));
                        std::reverse(result.begin(), result.end());
                        return result;
                    }
                    else
                    {
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

    template <class StringType>
    inline StringType
    strip(const StringType &s,
          const StringType &chars = StringType())
    {
        if (chars.empty())
        {
            auto start = std::find_if_not(
                s.begin(), s.end(), [](auto c)
                { return std::isspace(c); });
            auto end = std::find_if_not(s.rbegin(), s.rend(),
                                        [](auto c)
                                        { return std::isspace(c); });
            return s.substr(start - s.begin(),
                            (s.size() - (end - s.rbegin())) - (start - s.begin()));
        }
        else
        {
            auto start = std::find_if_not(s.begin(), s.end(), [&chars](auto c)
                                          { return chars.find(c) != chars.npos; });
            auto end = std::find_if_not(s.rbegin(), s.rend(), [&chars](auto c)
                                        { return chars.find(c) != chars.npos; });
            return s.substr(start - s.begin(),
                            (s.size() - (end - s.rbegin())) - (start - s.begin()));
        }
    }

    template <class StringType>
    inline StringType
    lstrip(const StringType &s,
           const StringType &chars = StringType())
    {
        if (chars.empty())
        {
            auto start = std::find_if_not(s.begin(), s.end(), std::isspace);
            return s.substr(start - s.begin(), s.end());
        }
        else
        {
            auto start = std::find_if_not(s.begin(), s.end(), [&chars](auto c)
                                          { return chars.find(c) != chars.npos; });
            return s.substr(start - s.begin(), s.end());
        }
    }

    template <class StringType>
    inline StringType
    rstrip(const StringType &s,
           const StringType &chars = StringType())
    {
        if (chars.empty())
        {
            auto end = std::find_if_not(s.rbegin(), s.rend(), std::isspace);
            return s.substr(s.begin(), (s.size() - (end - s.rbegin())));
        }
        else
        {
            auto end = std::find_if_not(s.rbegin(), s.rend(), [&chars](auto c)
                                        { return chars.find(c) != chars.npos; });
            return s.substr(s.begin(), (s.size() - (end - s.rbegin())));
        }
    }

    template <class StringType>
    inline tuple<StringType, StringType, StringType>
    partition(const StringType &s,
              const StringType &separator)
    {
        auto index = s.find(separator);
        if (index == s.npos)
        {
            return make_tuple(
                s,
                StringType(),
                StringType());
        }
        else
        {
            return make_tuple(
                s.substr(0, index),
                separator,
                s.substr(index + separator.size(), s.size() - (index + separator.size())));
        }
    }

    template <class StringType>
    inline tuple<StringType, StringType,
                 StringType>
    rpartition(const StringType &s,
               const StringType &separator)
    {
        auto index = s.rfind(separator);
        if (index == s.npos)
        {
            return make_tuple(
                StringType(),
                StringType(),
                s);
        }
        else
        {
            return make_tuple(
                s.substr(0, s.size() - (index - separator.size())),
                separator,
                s.substr(index + separator.size(), s.size() - index));
        }
    }

    template <class StringType>
    inline tuple<StringType, StringType, StringType>
    join(const vector<StringType> &strs,
         const StringType &separator)
    {
        typename StringType::size_type length = 0;
        for (size_t i = 0; i < strs.size(); i++)
        {
            if (i == 0)
            {
                length = length + strs[i].size();
            }
            else
            {
                length = length + separator.size();
                length = length + strs[i].size();
            }
        }
        StringType result;
        result.reserve(length);
        for (size_t i = 0; i < strs.size(); i++)
        {
            if (i == 0)
            {
                result += strs[i];
            }
            else
            {
                result += separator;
                result += strs[i];
            }
        }
        return result;
    }

    template <class StringType>
    inline bool startswith(const StringType &s,
                           const StringType &prefix,
                           int start = 0,
                           optional<int> end = {})
    {
        if (end.has_value())
        {
            if (prefix.size() + start > s.size() || prefix.size() < end.value())
            {
                return false;
            }
        }
        else
        {
            if (prefix.size() + start > s.size())
            {
                return false;
            }
        }
        for (size_t i = 0; i < prefix.size(); i++)
        {
            if (s[start + i] != prefix[i])
            {
                return false;
            }
        }
        return true;
    }

    template <class StringType>
    inline bool endswith(const StringType &s,
                         const StringType &prefix,
                         optional<int> start = {}, optional<int> end = {})
    {
        if (start.has_value())
        {
            if (end.has_value())
            {
                internal::adjust_slice(start.value(), end.value(), len(s));
                if (start.value() + prefix.size() > std::min(len(s), end.value()))
                {
                    return false;
                }
            }
            else
            {
                internal::adjust_start(start.value(), len(s));
                if (start.value() + prefix.size() > s.size())
                {
                    return false;
                }
                end = len(s);
            }
        }
        else
        {
            if (end.has_value())
            {
                internal::adjust_end(end.value(), len(s));
                if (prefix.size() > std::min(len(s), end.value()))
                {
                    return false;
                }
            }
            else
            {
                if (prefix.size() > s.size())
                {
                    return false;
                }
                end = len(s);
            }
        }
        for (size_t i = 0; i < prefix.size(); i++)
        {
            if (s[end.value() - i] != prefix[prefix.size() - i])
            {
                return false;
            }
        }
        return true;
    }

    template <class StringType>
    inline void capitalize(StringType &s)
    {
        if (s.empty())
        {
            return;
        }
        else
        {
            if (std::islower(s[0]))
            {
                s[0] = std::toupper(s[0]);
            }
            for (size_t i = 1; i < s.size(); i++)
            {
                if (std::isupper(s[i]))
                {
                    s[i] = std::tolower(s[i]);
                }
            }
        }
    }

    template <class StringType>
    inline bool isalnum(const StringType &s)
    {
        return std::all_of(s.begin(), s.end(), std::isalnum);
    }

    template <class StringType>
    inline bool isalpha(const StringType &s)
    {
        return std::all_of(s.begin(), s.end(), std::isalpha);
    }

    template <class StringType>
    inline bool isdigit(const StringType &s)
    {
        return std::all_of(s.begin(), s.end(), std::isdigit);
    }

    template <class StringType>
    inline bool islower(const StringType &s)
    {
        return std::all_of(s.begin(), s.end(), std::islower);
    }

    template <class StringType>
    inline bool isupper(const StringType &s)
    {
        return std::all_of(s.begin(), s.end(), std::isupper);
    }

    template <class StringType>
    inline bool isspace(const StringType &s)
    {
        return std::all_of(s.begin(), s.end(), std::isspace);
    }

    template <class StringType>
    inline void lower(StringType &s)
    {
        std::transform(s.begin(), s.end(), s.begin(), std::tolower);
    }

    template <class StringType>
    inline void upper(StringType &s)
    {
        std::transform(s.begin(), s.end(), s.begin(), std::toupper);
    }

    template <class StringType>
    inline void swapcase(StringType &s)
    {
        std::transform(
            s.begin(),
            s.end(),
            s.begin(),
            [](auto c)
            {
                if (std::isupper(c)) { return std::tolower(c); }
                else if (std::islower(c)) { return std::toupper(c); }
                else { return c; } });
    }

}; // namespace pystr

#endif // PY_STR_HPP