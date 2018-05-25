#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

std::string readfile(const std::string& filename)
{
    // reads upto the first \0 character or the whole file if not found
    std::fstream f(filename, std::fstream::in);
    std::string s;
    std::getline(f, s, '\0');
    return s;
}

TEST_CASE( "Analyze warnings", "[warning(s)]" ) {

    std::string resultfile = "..\\..\\build.txt";
    std::fstream f(resultfile, std::fstream::in);
    
    int warnings = 0;
    std::string s;
    while (!f.eof())
    {
        getline(f, s);
        if (s.find("warning") != std::string::npos)
        {
            std::cerr << s;
            warnings++;
        }
    }
    REQUIRE(warnings == 0);
}