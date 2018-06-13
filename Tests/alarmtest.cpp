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

std::vector<std::string> getFiles(const std::string& path)
{
	std::vector<std::string> result;
	for (auto &p : fs::directory_iterator(path))
		result.push_back(p.path().filename().string());
	return result;
}

std::vector<std::string> prefixFilter(const std::vector<std::string>& input, const std::string& startWith)
{
	std::vector<std::string> result;
	for (const auto &value : input)
	{
		if (value.find(startWith) != 0) continue;
		result.push_back(value);
	}
	return result;
}

std::string path = "..\\..\\Alarm";

TEST_CASE("Check for 'Build succeeded.'", "buildcheck") 
{
	for (const auto& l : prefixFilter(getFiles(path), "buildoutput"))
	{
		auto fullpath = fs::path(path) / l;
		auto content = readfile(fullpath.string());

		if (!contains(content, "Build succeeded."))
		{
			std::cerr << "Source: " << fullpath.string() << std::endl;
			auto lines = trim(split(content, '\n'));
			for (auto line : lines)
			{
				std::cerr << line << std::endl; // intentional flush
			}
		}

		SECTION(fullpath.string()) {
			CHECK(content.empty() != true);
			CHECK(contains(content, "Build succeeded.") == true);
		}
	}
}

TEST_CASE( "Check for warnings", "buildcheck" ) 
{
	for (const auto& l : prefixFilter(getFiles(path), "buildoutput"))
	{
		auto fullpath = fs::path(path) / l;
		auto content = readfile(fullpath.string());

		int warnings = 0;
		auto lines = filterNoCase(uniq(sort(trim(split(content, '\n')))), "warning c");
		std::cerr << "Source: " << fullpath.string() << std::endl;
		for (auto line : lines)
		{
			warnings++;
			std::cerr << line << std::endl; // intentional flush
		}

		SECTION(fullpath.string()) {
			CHECK(warnings == 0);
		}
	}

}
