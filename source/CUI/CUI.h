#pragma once
#include "../types/types.h"
#include <filesystem>
#include <vector>

void clearCin();
bool optional();
std::filesystem::path inputPathDirectory();
void start_option_1();
void printCollectedFiles(const std::vector<std::filesystem::path>& collected_files);
void start_option_2();
organizeOptions inputOrganizeOptions();
void start_option_3(Cookies& cookies);
void inputSaveInfo(const std::vector<std::filesystem::path>& files);
Options getOptionsFromUser();
std::filesystem::path inputTxtFileDirectory();
int choose(const std::vector<std::string>& options);
