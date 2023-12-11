#include "unit_testing.h"
#include "../file_management/file_management.h"
#include "../types/types.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <set>
#include <unordered_map>

namespace fs = std::filesystem;
// *** most functions here assumes privilage for creating and deleting files in system *** //


void unitmine_setup()
{
    fs::remove_all("unit_testing/testDirs/");
    fs::create_directory("unit_testing/testDirs/");
}

bool unit_matching()
{
    Options options;
    options.extensions.push_back("txt");
    options.filenames.push_back("test");
    // options.matchByDateOfCreation // #Todo 
    // options.matchingDateOfCreation // #Todo

    createIfNotExist("unit_testing/testDirs/matching");
    createIfNotExist("unit_testing/testDirs/matching/test.txt", false);
    bool result1 = matching("unit_testing/testDirs/matching/test.txt", options); // should be true

    options.sizeMin = 1000;
    bool result2 = matching("unit_testing/testDirs/matching/test.txt", options); // should be false

    options.sizeMin = 0;
    options.strongName = "test2";

    bool result3 = matching("unit_testing/testDirs/matching/test.txt", options); // should be false
    createIfNotExist("unit_testing/testDirs/matching/test2.txt", false);
    bool result4 = matching("unit_testing/testDirs/matching/test2.txt", options); // should equal true

    return result1 && !result2 && !result3 && result4;
}

bool unit_getFiles()
{
    Options options;
    options.strongName = "getFiles";
    options.nameCaseSensitive = true;
    createIfNotExist("unit_testing/testDirs/getFiles/");
    createIfNotExist("unit_testing/testDirs/getFiles/getFiles.txt", false);

    std::vector<fs::path> files;
    getFiles("unit_testing/testDirs/getFiles/", files, options);
    return files.size() == 1;
}

bool unit_handleCopies()
{
    createIfNotExist("unit_testing/testDirs/handleCopies/");
    createIfNotExist("unit_testing/testDirs/handleCopies/test.txt", false);

    Options options;
    options.filenames.push_back("test");
    for (const auto& file : fs::directory_iterator("unit_testing/testDirs/handleCopies/"))
    {
        if (matching(file, options)) return true;
    }

    return false;
}

bool unit_getFilesFromTxt()
{
    createIfNotExist("unit_testing/testDirs/getFilesFromTxt/");
    createIfNotExist("unit_testing/testDirs/getFilesFromTxt/test.txt", false);
    createIfNotExist("unit_testing/testDirs/getFilesFromTxt/text1.txt", false);

    std::vector<fs::path> files{"unit_testing/testDirs/getFilesFromTxt/test.txt",
                                "unit_testing/testDirs/getFilesFromTxt/text1.txt"};

    saveDirectories(files, "unit_testing/testDirs/getFilesFromTxt/", "getFilesFromTxt");
    std::vector<fs::path> newFiles = getFilesFromTxt("unit_testing/testDirs/getFilesFromTxt/getFilesFromTxt 2.txt");

    if (newFiles.size() != files.size()) return false;
    return true;
}

bool unit_saveDirectories()
{

    Options options;

    std::vector<fs::path> files;
    createIfNotExist("unit_testing/testDirs/saveDirectories/");
    createIfNotExist("unit_testing/testDirs/saveDirecotories/test.txt", false);
    getFiles("unit_testing/testDirs/saveDirectories/", files, options);
    
    saveDirectories(files, "unit_testing/testDirs/saveDirectories/");
    std::string fullDirectoy = "unit_testing/testDirs/saveDirectories/result " + std::to_string(files.size()) + ".txt";

    if (!fs::exists(fullDirectoy)) return false;
    std::vector<fs::path> newfiles = getFilesFromTxt(fullDirectoy);
    if (files.size() != newfiles.size()) return false;

    for (int i = 0; i < files.size(); ++i)
    {
        if(files[i] != newfiles[i]) return false;
    }

    return true;
}

bool unit_copyFilesFromDirectory()
{
    createIfNotExist("unit_testing/testDirs/copyFilesFromDirectory/");
    createIfNotExist("unit_testing/testDirs/copyFilesFromDirectory/text.txt", false);
    createIfNotExist("unit_testing/testDirs/copyFilesFromDirectory/text1.txt", false);
    createIfNotExist("unit_testing/testDirs/copyFilesFromDirectory/copies/");

    copyFilesIntoDirectory("unit_testing/testDirs/copyFilesFromDirectory/copies/", 
    {"unit_testing/testDirs/copyFilesFromDirectory/text1.txt", "unit_testing/testDirs/copyFilesFromDirectory/text.txt"});
    
    std::vector<fs::path> files;
    Options options;
    getFiles("unit_testing/testDirs/copyFilesFromDirectory/copies", files, options);
    return files.size() == 2;
}

bool unit_deleteFile()
{
    createIfNotExist("unit_testing/testDirs/deleteFile");
    createIfNotExist("unit_testing/testDirs/deleteFile/file", false);
    removeFile("unit_testing/testDirs/deleteFile/file");
    return !fs::exists("unit_testing/testDirs/deleteFile/file");
}

bool unit_moveFilesIntodDirectory()
{
    createIfNotExist("unit_testing/testDirs/mfid/");
    createIfNotExist("unit_testing/testDirs/mfid/moveTo/");
    createIfNotExist("unit_testing/testDirs/mfid/file", false);
    moveFilesIntoDirectory("unit_testing/testDirs/mfid/moveTo/", {"unit_testing/testDirs/mfid/file"});
    
    return !fs::exists("unit_testing/testDirs/mfid/file") && fs::exists("unit_testing/testDirs/mfid/moveTo/file");
}

bool unit_organizeOnExtensions()
{
    createIfNotExist("unit_testing/testDirs/organize");
    createIfNotExist("unit_testing/testDirs/organize/text1.txt", false);
    createIfNotExist("unit_testing/testDirs/organize/text2.txt", false);
    createIfNotExist("unit_testing/testDirs/organize/pang1.png", false);
    createIfNotExist("unit_testing/testDirs/organize/pang2.png", false);
    createIfNotExist("unit_testing/testDirs/organize/jpog1.jpg", false);
    createIfNotExist("unit_testing/testDirs/organize/jpong2.jpg", false);
    createIfNotExist("unit_testing/testDirs/organize/not1", false);
    createIfNotExist("unit_testing/testDirs/organize/noit", false);

    organizeOnExtensions("unit_testing/testDirs/organize/");
    bool result1 = (fs::exists("unit_testing/testDirs/organize/txt's") &&
            fs::exists("unit_testing/testDirs/organize/png's") &&
            fs::exists("unit_testing/testDirs/organize/jpg's") &&
            fs::exists("unit_testing/testDirs/organize/noExtension's"));
    
    createIfNotExist("unit_testing/testDirs/organize/jpong3.jpg", false);
    organizeOnExtensions("unit_testing/testDirs/organize/");
    return result1 && !fs::exists("unit_testing/testDirs/organize/jpong3.jpg");
}

bool unit_organizeOnGroups()
{
    createIfNotExist("unit_testing/testDirs/organize");
    createIfNotExist("unit_testing/testDirs/organize/text1.txt", false);
    createIfNotExist("unit_testing/testDirs/organize/text2.txt", false);
    createIfNotExist("unit_testing/testDirs/organize/pang1.png", false);
    createIfNotExist("unit_testing/testDirs/organize/pang2.png", false);
    createIfNotExist("unit_testing/testDirs/organize/jpog1.jpg", false);
    createIfNotExist("unit_testing/testDirs/organize/jpong2.jpg", false);
    createIfNotExist("unit_testing/testDirs/organize/not1", false);
    createIfNotExist("unit_testing/testDirs/organize/noit", false);

    organizeOptions options;
    options.push_back({"photos", {}});
    options.back().second.extensions.push_back("png");
    options.back().second.extensions.push_back("jpg");
    options.push_back({"textual", {}});
    options.back().second.extensions.push_back("txt");

    organizeOnGroups("unit_testing/testDirs/organize/", options);
    bool result1 = (fs::exists("unit_testing/testDirs/organize/textual/") &&
            fs::exists("unit_testing/testDirs/organize/photos/") &&
            fs::exists("unit_testing/testDirs/organize/others/"));
    
    createIfNotExist("unit_testing/testDirs/organize/jpong3.jpg", false);
    organizeOnGroups("unit_testing/testDirs/organize/", options);
    return result1 && !fs::exists("unit_testing/testDirs/organize/jpong3.jpg");
}

bool unit_deleteEmpties()
{
    createIfNotExist("unit_testing/testDirs/deleteEmpties");
    createIfNotExist("unit_testing/testDirs/deleteEmpties/dir1");
    createIfNotExist("unit_testing/testDirs/deleteEmpties/dir2");
    createIfNotExist("unit_testing/testDirs/deleteEmpties/dir3");
    createIfNotExist("unit_testing/testDirs/deleteEmpties/dir4");
    createIfNotExist("unit_testing/testDirs/deleteEmpties/dir1/file", false);

    deleteEmpties("unit_testing/testDirs/deleteEmpties");
    int count = 0;
    for (const auto& dir: fs::directory_iterator("unit_testing/testDirs/deleteEmpties/"))
    {
        ++count;
    }
    return count == 1;
}

bool unit_loadOptions()
{
    Options options;
    std::string content{" txt |  | false | N | N"};
    options = loadOptions(content);

    bool result1 = (options.extensions.size() == 1 && 
                    options.filenames.size() == 0 &&
                    options.sizeMin == 0 &&
                    options.nameCaseSensitive == false &&
                    options.sizeMax == INT_MAX);
    content = " jpg png | SN name | false | 100 | 1002";
    options = loadOptions(content);
    bool result2 = (options.extensions.size() == 2 &&
                options.strongName == "name" &&
                options.sizeMin == 100 &&
                options.sizeMax == 1002);

    return result1 && result2;
}

bool unit_loadOrganizeOptions()
{
    organizeOptions organize_groups;
    std::string content{" textual | txt |  | false | N | N \n photos | jpg png | SN name | true | 100 | 1002"};
    organize_groups = loadOrganizeOptions(content);
    bool result1 = (organize_groups.size() == 2 &&
                    organize_groups[0].first == "textual" &&
                    organize_groups[0].second.extensions.size() == 1 &&
                    organize_groups[0].second.filenames.size() == 0 &&
                    organize_groups[0].second.nameCaseSensitive == false &&
                    organize_groups[0].second.sizeMin == 0 &&
                    organize_groups[0].second.sizeMax == INT_MAX);

    bool result2 = (organize_groups[1].first == "photos" &&
                    organize_groups[1].second.extensions.size() == 2 &&
                    organize_groups[1].second.filenames.size() == 0 &&
                    organize_groups[1].second.strongName == "name" &&
                    organize_groups[1].second.nameCaseSensitive == true &&
                    organize_groups[1].second.sizeMin == 100 &&
                    organize_groups[1].second.sizeMax == 1002);

    return result1 && result2;
}

bool unit_loadCookies()
{
    createIfNotExist("unit_testing/testDirs/loadCookies/");
    createIfNotExist("unit_testing/testDirs/loadCookies/cookies.txt", false);
    createIfNotExist("unit_testing/testDirs/loadCookies/organizeOptions.txt", false);
    std::ofstream file("unit_testing/testDirs/loadCookies/cookies.txt");

    file << "unit_testing/testDirs/loadCookies/organizeOptions.txt";
    file.close();
    Cookies cookies = loadCookies("unit_testing/testDirs/loadCookies/cookies.txt");
    return cookies.lastSavedOrganizeOptionsPath == "unit_testing/testDirs/loadCookies/organizeOptions.txt";
}

bool unit_saveCookies()
{
    createIfNotExist("unit_testing/testDirs/saveCookies/");
    createIfNotExist("unit_testing/testDirs/saveCookies/organizeOptions.txt", false);
    Cookies cookies;
    cookies.lastSavedOrganizeOptionsPath = "unit_testing/testDirs/saveCookies/organizeOptions.txt";
    saveCookies(cookies, "unit_testing/testDirs/saveCookies/");

    cookies = loadCookies("unit_testing/testDirs/saveCookies/cookies.txt");
    bool result1 = cookies.lastSavedOrganizeOptionsPath == "unit_testing/testDirs/saveCookies/organizeOptions.txt";


    cookies.lastSavedOrganizeOptionsPath = "shitHappens/";
    saveCookies(cookies, "unit_testing/testDirs/saveCookies/");
    cookies = loadCookies("unit_testing/testDirs/saveCookies/cookies.txt");
    
    bool result2 = cookies.lastSavedOrganizeOptionsPath == "N";
    return result1 && result2;
}