#pragma once

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

// add
// consider string_view
// - std::string join(std::vector<std::string> const &, std::string const& delim)
// - std::string replace(std::string const& str, std::string const& oldStr, std::string const& newStr);     // replace Replace/Erase

// these function are not template for the following reasons:
// - error messages for undefined conversions would because very hard to read
// - string literals from nolonger be accepted as arguments
// -

inline std::string ltrim(const std::string& s)
{
    std::string str = s;
    auto it = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isspace(ch); });
    str.erase(str.begin(), it);
    return str;
}

inline std::wstring ltrim(const std::wstring& s)
{
    std::wstring str = s;
    auto it = std::find_if(str.begin(), str.end(), [](wchar_t ch) { return !std::isspace(ch); });
    str.erase(str.begin(), it);
    return str;
}

inline std::string rtrim(const std::string& s)
{
    std::string str = s;
    auto it = std::find_if(str.rbegin(), str.rend(), [](char ch) { return !std::isspace(ch); });
    str.erase(it.base(), str.end());
    return str;
}

inline std::wstring rtrim(const std::wstring& s)
{
    std::wstring str = s;
    auto it = std::find_if(str.rbegin(), str.rend(), [](wchar_t ch) { return !std::isspace(ch); });
    str.erase(it.base(), str.end());
    return str;
}

inline std::string trim(const std::string& s)
{
    std::string str = s;
    auto iter = std::find_if(str.rbegin(), str.rend(), [](char ch) { return !std::isspace(ch); });
    str.erase(iter.base(), str.end());
    auto iter2 = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isspace(ch); });
    str.erase(str.begin(), iter2);
    return str;
}

inline std::wstring trim(const std::wstring& s)
{
    std::wstring str = s;
    auto iter = std::find_if(str.rbegin(), str.rend(), [](wchar_t ch) { return !std::isspace(ch); });
    str.erase(iter.base(), str.end());
    auto iter2 = std::find_if(str.begin(), str.end(), [](wchar_t ch) { return !std::isspace(ch); });
    str.erase(str.begin(), iter2);
    return str;
}

inline std::vector<std::string>&& trim(std::vector<std::string>&& v)
{
    for (auto& str : v)
    {
        auto iter = std::find_if(str.rbegin(), str.rend(), [](char ch) { return !std::isspace(ch); });
        str.erase(iter.base(), str.end());
        auto iter2 = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isspace(ch); });
        str.erase(str.begin(), iter2);
    }
    return std::move(v);
}

inline std::vector<std::wstring>&& trim(std::vector<std::wstring>&& v)
{
    for (auto& str : v)
    {
        auto iter = std::find_if(str.rbegin(), str.rend(), [](wchar_t ch) { return !std::isspace(ch); });
        str.erase(iter.base(), str.end());
        auto iter2 = std::find_if(str.begin(), str.end(), [](wchar_t ch) { return !std::isspace(ch); });
        str.erase(str.begin(), iter2);
    }
    return std::move(v);
}

template <typename T>
bool ends_with(T const& value, T const& ending)
{
    if (ending.size() > value.size())
        return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

inline std::vector<std::string> split(const std::string& s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elements;
    while (std::getline(ss, item, delim))
    {
        elements.push_back(item);
    }
    return elements;
}

inline std::string ToLower(const std::string& str)
{
    std::string s = str;
    std::transform(s.begin(), s.end(), s.begin(), [](char c) { return static_cast<char>(::tolower(c)); });
    return s;
}

inline std::wstring ToLower(const std::wstring& str)
{
    std::wstring s = str;
    std::transform(s.begin(), s.end(), s.begin(), [](wchar_t  c) { return static_cast<wchar_t>(::tolower(c)); });
    return s;
}

inline std::string ToUpper(const std::string& str)
{
    std::string s = str;
    std::transform(s.begin(), s.end(), s.begin(), [](char c) { return static_cast<char>(::toupper(c)); });
    return s;
}

inline std::wstring ToUpper(const std::wstring& str)
{
    std::wstring s = str;
    std::transform(s.begin(), s.end(), s.begin(), [](wchar_t c) { return static_cast<wchar_t>(::toupper(c)); });
    return s;
}

template <typename T>
std::vector<T>&& sort(std::vector<T>&& v)
{
    std::sort(std::begin(v), std::end(v));
    return std::move(v);
}

// the vector needs to be sorted for this to work
template <typename T>
std::vector<T>&& uniq(std::vector<T>&& v)
{
    v.erase(std::unique(v.begin(), v.end()), v.end());
    return std::move(v);
}

inline bool contains(const std::string&haystack, const std::string needle)
{
    return haystack.find(needle) != std::wstring::npos;
}

inline bool contains_i(const std::string&haystack, const std::string needle)
{
    return ToLower(haystack).find(ToLower(needle)) != std::wstring::npos;
}

// notice: you might think 'bjork' should filter 'Björk' also, however, it does not, since ::tolower has no locale knowledge
inline std::vector<std::string> filterNoCase(std::vector<std::string>&& v, const std::string& filter)
{
    auto filterLower = ToLower(filter);
    std::vector<std::string> result;
    for (auto value : v)
    {
        if (ToLower(value).find(filterLower) != std::string::npos)
        {
            result.emplace_back(std::move(value));
        }
    }
    return result;
}

inline std::vector<std::wstring> filterNoCase(std::vector<std::wstring>&& v, const std::wstring& filter)
{
    auto filterLower = ToLower(filter);
    std::vector<std::wstring> result;
    for (auto value : v)
    {
        if (ToLower(value).find(filterLower) != std::wstring::npos)
        {
            result.emplace_back(std::move(value));
        }
    }
    return result;
}