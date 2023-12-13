#pragma once
#include "../types/types.h"
#include <string>
#include <filesystem>
#include <vector>

void setup();

void createIfNotExist(const std::filesystem::path& file, bool folder = true);
std::string getFileName(const std::filesystem::path& file, bool withExtension = false);
bool matching(const std::filesystem::path& file, const Options& options);
void getFiles(const std::filesystem::path& directory, std::vector<std::filesystem::path>& files, const Options& options = {});
int saveDirectories(const std::vector<std::filesystem::path>& files, const std::filesystem::path& directory = "savings/collected/", const std::string& name = "collected");
std::vector<std::filesystem::path> getFilesFromTxt(const std::filesystem::path& txt_file_dir);
int copyFilesIntoDirectory(const std::filesystem::path& directory, const std::vector<std::filesystem::path>& collectedFiles);
void getFilesMulti(const std::vector<std::filesystem::path>& fileDirectories, std::vector<std::filesystem::path>& out_collectedFiles, const Options& options);
int removeFile(const std::filesystem::path& file);
std::vector<std::filesystem::path> filter(const std::vector<std::filesystem::path>& imported_files, const Options& options);
int removeFiles(const std::vector<std::filesystem::path>& files);
int moveFilesIntoDirectory(const std::filesystem::path& path_directory, const std::vector<std::filesystem::path>& files);
bool moveFileIntoDirectory(const std::filesystem::path& path_directory, const std::filesystem::path& file);
bool copyFileIntoDirectory(const std::filesystem::path& path_directory, const std::filesystem::path& file);
void organizeOnExtensions(const std::filesystem::path& directory);
void organizeOnGroups(const std::filesystem::path& directory, const organizeOptions& organize_group);
std::filesystem::path saveOrganizeOptions(const organizeOptions& options, const std::filesystem::path& directory = "savings/organize_options/");
std::string handleCopies(std::string fileName);
std::string getFileExtension(const std::filesystem::path& file);
void deleteEmpties(const std::filesystem::path& directory);
organizeOptions loadOrganizeOptions(const std::string& content);
Options loadOptions(const std::string& content);
Cookies loadCookies(const std::filesystem::path& cookies_path = "savings/cookies/cookies.txt");
int saveCookies(const Cookies& cookies, const std::filesystem::path& = "savings/cookies/");
