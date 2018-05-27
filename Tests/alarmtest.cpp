#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <filesystem>
#include <sstream> 
#include <fstream>
#include <algorithm>
#include <cctype>

namespace fs = std::experimental::filesystem;

std::vector<std::string> split(const std::string& s, char delim)
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

std::string ToLower(const std::string& str)
{
    std::string s = str;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

std::string ToUpper(const std::string& str)
{
    std::string s = str;
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

template <typename T>
std::vector<T> sortt(const std::vector<T>& v)
{
    return std::vector<T>();
}

std::vector<std::string>&& sort(std::vector<std::string>&& v)
{
    std::sort(std::begin(v), std::end(v));
    return std::move(v);
}

// the vector needs to be sorted for this to work
std::vector<std::string>&& uniq(std::vector<std::string>&& v)
{
    v.erase(std::unique(v.begin(), v.end()), v.end());
    return std::move(v);
}

std::vector<std::string>&& trim(std::vector<std::string>&& v)
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

std::vector<std::string> filterNoCase(std::vector<std::string>&& v, const std::string& filter)
{
    std::vector<std::string> result;
    for (auto value : v)
    {
        if (ToLower(value).find(filter) != std::string::npos)
        {
            result.emplace_back(value);
        }
    }
    return result;
}

std::string readfile(const std::string& filename)
{
    if (!fs::exists(filename))
    {
        throw std::runtime_error(filename + " does not exist");
    }
    // reads upto the first \0 character or the whole file if not found
    std::fstream f(filename, std::fstream::in);
    std::string s;
    std::getline(f, s, '\0');
    return s;
}

TEST_CASE( "Analyze warnings", "[warning(s)]" ) {

    int warnings = 0;
    auto lines = filterNoCase(uniq(sort(trim(split(readfile("..\\..\\Alarm\\build.txt"), '\n')))), "warning c");
    for (auto line: lines)
    {
        warnings++;
        std::cerr << line << "\n";
    }
    REQUIRE(warnings == 0);
}
