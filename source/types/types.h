#pragma once
#include <vector>
#include <string>
#include <filesystem>

struct Date // never used
{
    int day;
    int month;
    int year;
};

struct Options 
{
    std::vector<std::string> extensions{};
    std::vector<std::string> filenames{};
    std::string strongName{};
    int sizeMin = 0;
    int sizeMax = INT_MAX;
    bool nameCaseSensitive = false;
    //Date matchingDateOfCreation{}; // never used, but to be considered

    bool isEmpty()
    {
        return (extensions.empty() && 
                filenames.empty() && 
                strongName.empty() &&
                sizeMin == 0 &&
                sizeMax == INT_MAX);
    }
};

struct Cookies
{
    std::filesystem::path lastSavedOrganizeOptionsPath = "N";
};

using organizeOptions = std::vector<std::pair<std::string, Options>>;