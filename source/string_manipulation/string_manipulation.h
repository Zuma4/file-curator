#pragma once
#include <string>
#include <vector>
bool contains(const std::string& string, const std::string& text);
bool endsWith(const std::string& full, const std::string& end);
std::string lowerize(std::string str);
std::vector<std::string> split(const std::string& string, char split_letter = ' ');
std::string& strip(std::string& string, char strip_letter = ' ');
std::string cpy_strip(std::string string, char strip_letter = ' ');
int integerize(const std::string& num);