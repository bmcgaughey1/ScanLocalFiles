// ScanLocalFiles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

bool checkForFilesWithPattern(std::string& folder, std::string& pattern);
std::vector<std::string> get_directories(const std::string& s);
std::vector<std::string> splitString(const std::string& str, const char delimiter);

int main(void)
{
    /*if (__argc < 4) {
        printf("Syntax:\nScanLocalFiles RootFolder Patterns OutputFile\n");
        return(1);
    }*/

    // read command line args and put in variables
    // single root folder
    // patterns separated by |...put in vector of std::string
    // output file...csv format for simplicity

    std::string rootFolder = __argv[1];
    std::string pattern = __argv[2];            // "*.laz|*.las";
    std::string outputFileName = __argv[3];

    // parse patterns
    std::vector<std::string> patterns = splitString(pattern, '|');

    // get list of folders under and including root
    std::cout << "Scanning folders in " << rootFolder << "...";
    std::vector<std::string> folders;
    folders = get_directories(rootFolder);

    size_t folderCount = folders.size();
    std::cout << "Found " << folderCount << " folders" << std::endl;

    // open output file
    std::ofstream outputFile(outputFileName); // Open file in write mode
    if (outputFile.is_open()) {
        size_t count = 1;

        // write header for CSV
        outputFile << "\"Folder\"";
        for (auto& p : patterns) {
            outputFile << ",\"" << p << "\"";
        }
        outputFile << std::endl;

        // iterate through folders and patterns looking for files
        for (auto& f : folders) {
            std::cout << "Scanning: " << f << " -- " << count << " of " << folderCount << " folders...";
            outputFile << "\"" << f << "\"";
            for (auto& p : patterns) {
                //std::cout << "Pattern: " << p << std::endl;
                if (checkForFilesWithPattern(f, p))
                    outputFile << ",\"1\"";
                    //std::cout << f << " Found files matching " << p << "!!" << std::endl;
                else
                    outputFile << ",\"0\"";
                    //std::cout << f << " No files found matching " << p << std::endl;
            }
            outputFile << std::endl;
            std::cout << "Done" << std::endl;
        }
        outputFile.close();

        std::cout << "Data written to " << outputFileName << " successfully." << std::endl;
    }
    else {
        std::cerr << "Unable to open file: " << outputFileName << std::endl;
        return 1; // Indicate an error
    }
}

bool checkForFilesWithPattern(std::string& folder, std::string& pattern)
{
    struct _finddata_t c_file;
    intptr_t hFile;

    // Find first .c file in current directory
    char temp[1024];
    sprintf_s(temp, "%s/%s", folder.c_str(), pattern.c_str());

    if ((hFile = _findfirst(temp, &c_file)) != -1L) {
        _findclose(hFile);
        return true;
    }

    return false;
}

std::vector<std::string> get_directories(const std::string& s)
{
    std::vector<std::string> r;
    for (auto& p : std::filesystem::recursive_directory_iterator(s))
        if (p.is_directory())
            r.push_back(p.path().string());
    return r;
}

std::vector<std::string> splitString(const std::string& str, const char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
