#pragma once
#include "../types/types.h"
#include <filesystem>
#include <vector>

void clearCin();
bool optional();
std::filesystem::path inputPathDirectory();
void exploreDirectory();
void printCollectedFiles(const std::vector<std::filesystem::path>& collected_files);
void organizeDirectory(Cookies& cookies);
organizeOptions inputOrganizeOptions();
void loadSavedFiles();
void inputSaveInfo(const std::vector<std::filesystem::path>& files);
Options getOptionsFromUser();
std::filesystem::path inputTxtFileDirectory();
int choose(const std::vector<std::string>& options);
