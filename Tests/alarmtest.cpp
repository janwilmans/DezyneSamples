#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include "stringmanipulation.h"

namespace fs = std::experimental::filesystem;

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

TEST_CASE("The number of builds should be 3", "[build(s)]") {

    int builds = 0;
    auto lines = filterNoCase(trim(split(readfile("..\\..\\Alarm\\build.txt"), '\n')), "Build succeeded.");
    for (auto line : lines)
    {
        builds++;
    }
    REQUIRE(builds == 3);
}

TEST_CASE( "The number of warnings should be 0", "[warning(s)]" ) {

    int warnings = 0;
    auto lines = filterNoCase(uniq(sort(trim(split(readfile("..\\..\\Alarm\\build.txt"), '\n')))), "warning c");
    for (auto line: lines)
    {
        warnings++;
        std::cerr << line << "\n";
    }
    REQUIRE(warnings == 0);
}
