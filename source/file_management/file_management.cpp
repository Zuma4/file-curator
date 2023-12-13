#include "file_management.h"
#include "../string_manipulation/string_manipulation.h"
#include "../types/types.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <filesystem>
#include <set>
#include <fstream>

namespace fs = std::filesystem;


void setup()
{
    createIfNotExist("savings/");
    createIfNotExist("savings/collected/");
    createIfNotExist("savings/organize_options/");
    createIfNotExist("savings/cookies/");
}

void createIfNotExist(const fs::path& file, bool folder) {
    if (!fs::exists(file)) {
        if (folder)
            fs::create_directory(file);
        else{
            std::ofstream create{file};
        }
    }
}

std::string getFileName(const fs::path& file, bool withExtension) {
    std::string fileString = file.string();
    int last_back = fileString.find_last_of("/\\");
    if (last_back == std::string::npos) {
        return fileString;
    }
    
    if (!withExtension){
        int last_dot = fileString.find_last_of('.');
        if (last_dot == std::string::npos) {return fileString.substr(last_back + 1);}
        else {return fileString.substr(last_back + 1, last_dot - last_back - 1);}
    }
    else
    {return fileString.substr(last_back + 1);}
}


bool matching(const fs::path& file, const Options& options) {
    if (!fs::exists(file))
        return false;

    std::string fileDirectoryName = file.string();
    std::string fileExtension = getFileExtension(file);

    if (options.extensions.size() > 0)
    {
        bool matchingExtension = false;
        for (const auto& extension : options.extensions) {
            if (fileExtension == extension) {
                matchingExtension = true;
                break;
            }
        }

        if (!matchingExtension) return false;
    }

    int fileSize = fs::file_size(file);
    if (fileSize > options.sizeMax || fileSize < options.sizeMin) return false;

    std::string filename = getFileName(fileDirectoryName);
    if (!options.nameCaseSensitive)
        filename = lowerize(filename);

    if (options.strongName != "")
    {
        if (filename != options.strongName) return false;
    }

    else if (options.filenames.size() > 0){
        bool nameMatch = false;
        for (const auto& name : options.filenames) 
        {
            if (contains(filename, name))
            {
                nameMatch = true;
                break;
            }
        }
        if (!nameMatch) return false;
    }

    return true;
}


std::filesystem::path saveOrganizeOptions(const organizeOptions& options, const std::filesystem::path& directory)
{
    std::string content;
    for (const auto& pair : options){
        content.append(pair.first + " | ");
        for (const auto& extension : pair.second.extensions){
            content.append(extension + " ");
        }
        content.append("| ");
        if (pair.second.strongName == ""){
            for (const auto& filename: pair.second.filenames){
                content.append(filename + " ");
            }
        content.append("| ");
        }
        else{
            content.append("SN:" + pair.second.strongName + " | ");
        }
        if (pair.second.nameCaseSensitive) content.append("true | ");
        else content.append("false | ");

        if (pair.second.sizeMin != 0) content.append(std::to_string(pair.second.sizeMin));
        else content.append("N");
        content.append(" | ");

        if (pair.second.sizeMax != INT_MAX) content.append(std::to_string(pair.second.sizeMax));
        else content.append("N");
        content.append("\n");
    }

    std::string filename = handleCopies(directory.string() + "options.txt");
    std::ofstream file{filename};
    file << content;
    file.close();

    return filename;
}

void getFiles(const fs::path& directory, std::vector<fs::path>& files, const Options& options) {

    std::vector<fs::path> directories;
    directories.push_back(directory);

    while (directories.size() > 0)
    {
        // for correct ordering
        fs::path cur_directory = directories[0];
        directories.erase(directories.begin());

        for (const auto& file : fs::directory_iterator(cur_directory)) 
        {
            if (fs::is_directory(file)) 
                directories.push_back(file);

            else 
            {
                if (matching(file, options))
                    files.push_back(file);
            }
        }
    }
}

// passing by value because i'll need to create copy anyway
std::string handleCopies(std::string fileName) {
    std::string fileExtension = '.' + getFileExtension(fileName);
    if (fileExtension.size() == 1) {
        fileExtension = "";
    }
    else
    {
        fileName.erase(fileName.find_last_of('.'));
    }

    while (fs::exists(fileName + fileExtension)) {
        fileName.append("(c)");
    }

    if (fileExtension != ""){
        fileName.append(fileExtension);
    }

    return fileName;
}

int saveDirectories(const std::vector<fs::path>& files, const fs::path& directory, const std::string& name) {
    if (!fs::exists(directory))
        return -1;
    
    std::string addition = "";
    if (!endsWith(directory.string(), "/") || !endsWith(directory.string(), "/"))
        addition = "/";

    createIfNotExist(directory);
    std::string fullDirectory = { directory.string() + addition + name + " " + std::to_string(files.size()) + ".txt"};

    std::ofstream textFile{ handleCopies(fullDirectory) };
    for (const auto& file : files) {
        std::string fileInfo{};
        fileInfo.append(file.string() + " | ");
        fileInfo.append(getFileName(file) + " | ");
        fileInfo.append(std::to_string(fs::file_size(file) / 1000) + "KB");
        textFile << fileInfo + '\n';
    }
    textFile.close();

    return 0;
}

int removeFile(const fs::path& file)
{
    if(!fs::exists(file))
        throw "file doesn't exist";
    else
    {
        return remove(file.string().c_str());
    } 
}

std::string getFileExtension(const fs::path& file)
{
    std::string filestr = file.string();
    int last_dot = filestr.find_last_of('.');

    if (last_dot == std::string::npos)
        return "";
    return filestr.substr(last_dot + 1);
}

std::vector<fs::path> filter(const std::vector<fs::path>& imported_files, const Options& options)
{
    std::vector<fs::path> res{};
    for (const auto& file : imported_files)
    {
        if (matching(file, options))
        {
            std::cout << file << '\n';
            res.push_back(file);
        }
    }

    return res;
}


int moveFilesIntoDirectory(const fs::path& path_directory, const std::vector<fs::path>& files)
{
    if (!fs::exists(path_directory))
        throw "invalid path directory";

    int moved = 0;
    for (const auto& file: files){
        if(moveFileIntoDirectory(path_directory, file))
            ++moved;
    }
    return moved;
}

bool moveFileIntoDirectory(const fs::path& path_directory, const fs::path& file)
{
    if(copyFileIntoDirectory(path_directory, file))
        return (removeFile(file));
    return 0;
}

int removeFiles(const std::vector<fs::path>& files)
{
    int files_removed = 0;
    for(const auto& file : files)
    {
        if(removeFile(file) == 0)
            ++files_removed;
    }
    return files_removed;
}

std::vector<fs::path> getFilesFromTxt(const fs::path& txt_file_dir) 
{
    if (!fs::exists(txt_file_dir))
        throw "invalid txt file directory";

    std::ifstream txtFile{ txt_file_dir };
    std::string fileContent;
    std::getline(txtFile, fileContent, '\0');
    txtFile.close();

    std::vector<fs::path> res;
    std::string current;
    bool waitUntilNewLine = false;
    for (int i = 0; i < fileContent.size(); ++i)
    {
        if (fileContent[i] == '|' && !waitUntilNewLine)
        {
            current.pop_back();
            waitUntilNewLine = true;
        }
        else if (fileContent[i] == '\n')
        {
            waitUntilNewLine = false;
            res.push_back(current);
            current.clear();
        }

        else if (!waitUntilNewLine)
            current.push_back(fileContent[i]);
    }

    int i = 0;
    while (i < res.size())
    {
        if (!fs::exists(res[i]))
            res.erase(res.begin() + i);
        else
            ++i;
    }

    if (!current.empty())
        res.push_back(current);
    return res;
}

int copyFilesIntoDirectory(const fs::path& directory, const std::vector<fs::path>& collectedFiles) {
    if (!fs::exists(directory))
        throw "invalid directory path";
    int copied = 0;
    for (const auto& file : collectedFiles) {
        if(copyFileIntoDirectory(directory, file))
            ++copied;
    }

    return copied;
}

bool copyFileIntoDirectory(const fs::path& directory, const fs::path& file)
{
    if (!fs::exists(directory) || !fs::exists(file))
        return false; 

    std::string file_name = getFileName(file, true);
    std::string copyTo_directory = { directory.string() + '/' + file_name };
    
    if (copyTo_directory == file)
        return false;

    std::ifstream copyFrom{ file, std::ios::binary };
    std::ofstream copyTo{ copyTo_directory, std::ios::binary };

    copyTo << copyFrom.rdbuf();
    return fs::exists(directory.string() + '/' + file_name);
}

void getFilesMulti(const std::vector<fs::path>& fileDirectories, std::vector<fs::path>& out_collectedFiles, const Options& options) 
{for (const auto& dir : fileDirectories){getFiles(dir, out_collectedFiles, options);}}


void deleteEmpties(const fs::path& directory)
{
    if (!fs::exists(directory) || !fs::is_directory(directory))
        throw "invalid directory";
    
    for (const auto& dir: fs::directory_iterator(directory))
    {
        if (fs::is_directory(dir) && fs::is_empty(dir))
            fs::remove_all(dir);
    }
}

void organizeOnExtensions(const fs::path& directory)
{
    if (!fs::is_directory(directory))
        throw "invalid directory path";

    std::string addition = "";
    if (!endsWith(directory.string(), "/") && !endsWith(directory.string(), "/"))
        addition = "/";

    std::vector<fs::path> collected_files;
    std::set<std::string> madeFolders; 
    getFiles(directory, collected_files);

    for(const auto& file: collected_files)
    {
        std::string fileExtension = getFileExtension(file);
        fileExtension = (fileExtension == "") ? "noExtension" : fileExtension;

        std::string full_directory = directory.string() + addition + fileExtension + "'s";
        if (madeFolders.find(fileExtension) == madeFolders.end())
        {
            createIfNotExist(full_directory);
            madeFolders.insert(fileExtension);
        }
        moveFileIntoDirectory(full_directory, file);
    }

    deleteEmpties(directory);
}


void organizeOnGroups(const fs::path& directory, const organizeOptions& organize_group)
{
    if (!fs::is_directory(directory))
        throw "invalid directory path";
    std::string addition = "";
    if (!endsWith(directory.string(), "/") && !endsWith(directory.string(), "\\"))
        addition = "/";

    fs::path others_dir = directory.string() + addition + "others" + "/";
    createIfNotExist(others_dir);

    for (const auto& pair: organize_group)
    {
        std::vector<fs::path> files;
        // there's probably a more sophisticated way to 
        // reduce complexity, which I'm too lazy (and stupid) to think about
        getFiles(directory, files, pair.second);
        if (files.size() > 0){
            createIfNotExist(directory.string() + addition + pair.first);
            moveFilesIntoDirectory(directory.string() + addition + pair.first, files);
        }
    }

    // put the remaining files into (others)
    std::vector<fs::path> files;
    getFiles(directory, files);

    std::vector<fs::path> addfiles;
    for (int i=0;i<files.size();++i){
        bool add = true;
        for (const auto& pair: organize_group)
            if (matching(files[i], pair.second)){
                add=false;
                break;
            }
        if (add)
            addfiles.push_back(files[i]);
    }
    if (addfiles.size() > 1){{moveFilesIntoDirectory(others_dir, addfiles);}}
    deleteEmpties(directory);
}

// this function is simple rather than efficent
Options loadOptions(const std::string& content)
{
    std::vector<std::string> parts{split(content, '|')};
    Options options;
    for (auto& part: parts)
        strip(part);

    options.extensions = split(parts[0]); // extensions

    // filenames
    if (contains(parts[1], "SN")) // if it's strong name matching
        options.strongName = split(parts[1])[1];

    else // if can match with multiple names
        options.filenames = split(parts[1]);

    if (contains(parts[2], "true")) options.nameCaseSensitive = true;
    else options.nameCaseSensitive = false;

    // min size limit
    if (parts[3] != "N") // N indicates null, meaning no limit
        options.sizeMin = integerize(parts[3]);

    // max size limit
    if (parts[4] != "N")
        options.sizeMax = integerize(parts[4]);

    return options;
}


// this function can cause some problems, but it works if used correctly
organizeOptions loadOrganizeOptions(const std::string& content)
{
    std::vector<std::string> each_option{split(content, '\n')};
    organizeOptions options;
    for (const auto& option: each_option)
    {
        options.push_back({});
        options.back().first = cpy_strip(option.substr(0, option.find_first_of('|')-1)); // name
        options.back().second = loadOptions(option.substr(option.find_first_of('|') + 1)); // options
    }

    return options;
}

Cookies loadCookies(const fs::path& cookies_path)
{
    Cookies cookies;
    std::ifstream input{cookies_path.string()};
    if (input.fail() || getFileExtension(cookies_path) != "txt"){
        throw "unable to load file";
    }
    std::string content;
    input >> content;
    input.close();

    std::vector<std::string> cookiesParts = split(content, '\n');
    cookies.lastSavedOrganizeOptionsPath = cookiesParts[0];

    return cookies;
}

int saveCookies(const Cookies& cookies, const fs::path& save_path)
{
    if (!fs::exists(save_path))
        return -1;
    std::ofstream output{save_path/"cookies.txt"};
    std::string content;

    if (fs::exists(cookies.lastSavedOrganizeOptionsPath)){
        content.append(cookies.lastSavedOrganizeOptionsPath.string() + '\n');
    }
    else 
    {
        content.append("N");
    }

    output << content;
    output.close();

    return 0;
}
