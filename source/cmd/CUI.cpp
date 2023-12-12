#include "CUI.h"
#include "../file_management/file_management.h"
#include "../string_manipulation/string_manipulation.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = filesystem;


void clearCin()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool optional() {
    // string because working with char input is painful
    string option;
    bool res;
    while (true) {
        cin >> option;
        if (option[0] == 'n') {
            res = false;
            break;
        } else if (option[0] == 'y') {
            res = true;
            break;
        } else {
            clearCin();
            std::cout << "please enter a valid option: ";
        }
    }

    return res;
}


void start_option_1()
{
    fs::path filepath = inputTxtFileDirectory();
    vector<fs::path> imported_files = getFilesFromTxt(filepath);

    while (imported_files.size() == 0)
    {
        cout << "\nthis is not a valid directory: \n";
        imported_files = getFilesFromTxt(inputTxtFileDirectory());
    }

    cout << '\n' << imported_files.size() << " files loaded\n"; 
    while (true)
    {
        vector<string> load_options = {"copy all files", "delete all files", "move all files", "filter", "back"};
        int load_option = choose(load_options);

        if (load_option == 1)
        {
            cout << "\nenter path directory where you will copy the files: \n";
            fs::path path_directory = inputPathDirectory();
            
            cout << "\nare you sure you want to copy all files into this directory (y, n)?: \n";
            bool copy = optional();
            
            if (copy)
            {
                copyFilesIntoDirectory(path_directory, imported_files);
            }
            cout << imported_files.size() << " files copied!\n";
        }

        else if (load_option == 2)
        {
            Options options = getOptionsFromUser();
            vector<fs::path> filtered = filter(imported_files, options);
            cout << "\nfiltered!, remaining files: " << filtered.size() << '\n';
            cout << "save files (y, n)? : ";
            bool save_files = optional();

            if (save_files)
            {
                inputSaveInfo(filtered);
            }
        }

        else if (load_option == 3)
        {
            cout << "\nare you sure you want to delete the specified files (y, n)?: \n";
            bool remove = optional();
            if (remove)
            {
                int files_deleted = removeFiles(imported_files);
                cout << files_deleted << " files deleted\n";
            }
        }

        else if (load_option == 4)
        {
            cout << "\nenter path directory where you will move the files: ";
            fs::path path_directory = inputPathDirectory();

            cout << "\nare you sure you want to move all files into this directory (y, n)?: ";
            bool move = optional();
            
            if (move)
            {
                int result = moveFilesIntoDirectory(path_directory, imported_files);
                cout << '\n' << result << " files moved!\n";
            }
        }

        else
        {
            return;
        }
    }
}

void printCollectedFiles(const vector<fs::path>& collected_files)
{
    for (const auto& file : collected_files) {
        cout << file.string() << " | " <<  getFileName(file) << " | " << std::to_string(fs::file_size(file) / 1000) << "KB" << '\n';
    }
}

void start_option_2()
{
    while(true)
    {
        vector<fs::path> path_directories;
        while (true)
        {
            cout << "\nenter the path directory you want to traverse: ";
            path_directories.push_back(inputPathDirectory());
            cout << "\nwant to add more directories (y, n)?: ";
            bool addmore = optional();

            if (!addmore)
                break;
        }

        Options options = getOptionsFromUser();
        vector<fs::path> collected_files;
        getFilesMulti(path_directories, collected_files, options);

        printCollectedFiles(collected_files);
        cout << "\ncollected!\n";

        cout << "\nsave files (y, n)?: ";
        bool save_files = optional();

        if (save_files)
        {
            inputSaveInfo(collected_files);
        }

        cout << "\nwant to explore other directories (y, n)?: ";
        bool explore_others = optional();

        if(!explore_others)
            break;
    }
}

organizeOptions inputOrganizeOptions()
{
    organizeOptions organzie_groups;

    while (true)
    {
        clearCin();
        cout << "\nenter the sub directory name (e.g 'photos'): ";
        string group_name;
        getline(cin, group_name);

        Options options = getOptionsFromUser();
        if (options.isEmpty()){
            cout << "\noptions are invalid\n";
        }
        else{
            organzie_groups.push_back({group_name, options});
            cout << "\nwant to add more groups? (y, n): ";
            if(!optional()) break;
        }
    }

    return organzie_groups;
}

void start_option_3(Cookies& cookies)
{
    cout << "\nWarning: this option may delete (empty) directories\n";

    cout << "\nenter path directory (e.g 'c:\\folder\\'): ";
    fs::path directory = inputPathDirectory();

    vector<string> organize_options {"organize based on extensions", "organize based on group of criterias (e.g, 'jpg', 'png' in (photos) dir)"};
    bool basedOnExtensions = !(choose(organize_options)-1);
    if (basedOnExtensions)
        organizeOnExtensions(directory);
    else
    {
        organizeOptions organize_groups;
        vector<string> organizeOnGroups_options = {"get last organize options", "load saved organize options", "create new organize options"};
        int choosen = choose(organizeOnGroups_options);

        if (choosen == 1)
        {
            if (cookies.lastSavedOrganizeOptionsPath == "N" || !fs::exists(cookies.lastSavedOrganizeOptionsPath))
            {
                cout << "\nthere's no previous saved files\n";
                organizeOnGroups_options.erase(organizeOnGroups_options.begin());
                choosen = choose(organizeOnGroups_options) + 1;
            }
            
            else
            {
                ifstream input{cookies.lastSavedOrganizeOptionsPath.string()};
                string content;
                getline(input, content, '\0');
                input.close();
                organize_groups = loadOrganizeOptions(content);
            }
        }

        if (choosen == 2){
            fs::path file_path = inputTxtFileDirectory();

            ifstream input{file_path.string()};
            string content;
            getline(input, content, '\n');
            input.close();

            organize_groups = loadOrganizeOptions(content);
        }

        if (choosen == 3){
            organize_groups = inputOrganizeOptions();
        }

        organizeOnGroups(directory, organize_groups);
        cout << "\nDone!\n";

        if (choosen == 3)
        {
            // TODO: saving as last options and saving the options seperately most be independent of each other
            cout << "\nwant to save your organize options? (y, n): ";
            if (optional()){
                cout << "\nwant to save into a specific directory? (y, n): ";
                if (optional())
                {
                    fs::path save_path = inputPathDirectory();
                    cookies.lastSavedOrganizeOptionsPath = saveOrganizeOptions(organize_groups, save_path);
                }
                else {
                    cookies.lastSavedOrganizeOptionsPath = saveOrganizeOptions(organize_groups);
                }

                saveCookies(cookies);
            }

        }

    }

}

void inputSaveInfo(const vector<fs::path>& files)
{
    cout << "\nwant to save in specific directory? (y, n) (enter 'n' and it'll be saved in 'collected/'): \n";
    bool specify_directory = optional();
    if (specify_directory)
    {
        fs::path directory_path = inputPathDirectory();
        cout << "\nwant to save the result in a specific name? (enter 'n' and it'll be saved as 'collected'): \n";
        bool specify_name = optional();

        if (specify_name)
        {
            string result_name;
            cin >> result_name;
            saveDirectories(files, directory_path, result_name);
        }
        else
        {
            saveDirectories(files, directory_path);
        }
    }
    else
        saveDirectories(files);
}

Options getOptionsFromUser()
{
    clearCin();
    Options options;

    cout << "\nfilter by extension (y, n)?: ";
    bool f_by_extension = optional();

    if (f_by_extension)
    {
        while (true)
        {
            string extension;
            cout << "\nadd extension: ";
            cin >> extension;
            options.extensions.push_back(extension);

            cout << "\nwant to add more extensions (y, n)?: ";
            bool addmore = optional();

            if (!addmore)
                break;
        }
    }

    cout << "\nfilter by name (y, n)?: "; 
    bool filter_by_name = optional();
    if (filter_by_name)
    {
        cout << "\nwant the name matching to be case sensitive (y, n)?: ";
        options.nameCaseSensitive = optional();

        vector<string> matching_options{"strong matching (one name)", "loosey matching (can match with multiple names)"};
        bool strong = !((choose(matching_options)) - 1);

        if (strong)
        {
            cout << "\nenter file name: ";
            cin >> options.strongName;

            if (!options.nameCaseSensitive)
                options.strongName = lowerize(options.strongName);
        }

        else 
        {
            while (true)
            {
                clearCin();
                std::string filename;
                cout << "\nadd name: ";

                getline(cin, filename, '\n');
                if (!options.nameCaseSensitive) filename = lowerize(filename);
                options.filenames.push_back(filename);

                cout << "\nwant to add more (y, n)?: ";
                bool addmore = optional();

                if (!addmore)
                    break;
            }
        }
    }

    cout << "\nfilter by size (y, n)?: ";
    bool f_by_size = optional();

    if (f_by_size)
    {
        cout << "\nwant max size limit? (y, n): ";
        bool max_size_limit = optional();
        
        if (max_size_limit)
        {
            cout << "\nenter max size (in kilobyte): ";
            int size_in_kb;
            cin >> size_in_kb;
            options.sizeMax = size_in_kb * 1000;
        }

        cout << "\nwant minimum size limit? (y, n): ";
        bool min_size_limit = optional();

        if (max_size_limit)
        {
            cout << "\nenter min size (in kilobyte): ";
            int size_in_kb;
            cin >> size_in_kb;
            options.sizeMin = size_in_kb * 1000;
        }
    }

    return options;
}

fs::path inputTxtFileDirectory()
{
    clearCin();
    cout << "\nenter .txt file path, the txt file most be a valid output of file-collector: \n";

    while (true)
    {
        string filepath;
        getline(cin, filepath); 
        if (!fs::exists(filepath) || !endsWith(filepath, "txt") || fs::is_directory(filepath) || fs::file_size(filepath) == 0)
        {
            cout << "\nenter a valid file path: ";
        }
        else 
        {
            return filepath;
        }
    }
}


// this function returns the actual user input, not the index (index 0 == output 1)
int choose(const vector<string>& options)
{
    cout << '\n';
    for(int i = 0; i < options.size(); ++i)
    {
        cout << i + 1 << " - " << options[i] << '\n';
    }

    cout << "\nchoose between options: ";

    int option;

    while (true)
    {
        cin >> option;

        option;
        if (option-1 >= 0 && option-1 < options.size())
            return option;
        else
        {
            cout << "\nPlease Enter A Valid Option: ";
            clearCin();
        }
    }
}

fs::path inputPathDirectory() {
    clearCin();

    string directory;
    while (true) {
        getline(cin, directory);
        if (!fs::is_directory(directory)) {
            cout << "please enter a valid directory path (ex. c:\\folder_directory\\): ";
        } 
        else 
            break;
    }
    if (!endsWith(directory, "\\") && !endsWith(directory, "/"))
        directory.push_back('/');
    return directory;
}
