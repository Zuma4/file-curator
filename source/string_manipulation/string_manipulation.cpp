#include "string_manipulation.h"
#include <string>
#include <algorithm>
#include <vector>

bool endsWith(const std::string& full, const std::string& end)
{
    if (end.length() > full.length()) return false;
    return (std::search(full.begin() + (full.length() - end.length()), full.end(), end.begin(), end.end())) != full.end();
}

bool contains(const std::string& string, const std::string& text) {
    if (text == "" || text.length() > string.length()) return false;
    return (std::search(string.begin(), string.end(), text.begin(), text.end()) != string.end());
}

std::string lowerize(std::string str)
{
    for(int i = 0; i < str.length(); ++i)
    {
        // if the char is a capital letter
        if (str[i] >= 65 && str[i] <= 90)
            str[i] += 32; // conversion from capital to small
    }

    return str;
}

std::vector<std::string> split(const std::string& string, char split_letter)
{
    std::vector<std::string> result{""};
    for (int i = 0; i < string.size(); ++i)
    {
        if (string[i] == split_letter && result.back().length() > 0) {result.push_back("");}
        else {result.back().push_back(string[i]);}
    }

    if (result.back().empty()) result.pop_back();
    return result;
}

std::string& strip(std::string& string, char strip_letter)
{
    // lstrip
    if (string.size() > 0){
        while(string[0] == strip_letter)
        {string.erase(string.begin());}
    }
    // rstrip
    if (string.size() > 0){
        while(string.back() == strip_letter)
        {string.pop_back();}
    }

    return string;
}

std::string cpy_strip(std::string string, char strip_letter)
{
    // lstrip
    if (string.size() > 0){
        while(string[0] == strip_letter)
        {string.erase(string.begin());}
    }
    // rstrip
    if (string.size() > 0){
        while(string.back() == strip_letter)
        {string.pop_back();} 
    }

    return string;
}

int integerize(const std::string& num)
{
    int res=0;
    for (const auto& digit: num)
    {
        if (!isdigit(digit)) 
            throw "unable to convert non-number";
        else{
            res *= 10;
            res += digit - 48; // ascii-to-integer conversion
        }
    }
    return res;
}