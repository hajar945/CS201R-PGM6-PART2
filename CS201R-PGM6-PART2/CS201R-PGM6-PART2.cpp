// Program 6: Media Lib - Part 2
// CS201R Professor Gladbach 
// NAME: Hajar Wilkes
// DATE: 11/14/2024
// Aid from ChatGPT with numerous tweaks and adjustments

#include "Functions.h"
#include "Media.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>

using namespace std;

int main() {
    ifstream inList("mediaList.txt");
    ifstream commandsFile("mediaCommands.txt");
    ofstream outFile("mediaReport.txt");
    ofstream outErr("mediaError.txt");

    // Check if files opened successfully
    if (!inList.is_open() || !commandsFile.is_open()) {
        cerr << "Error: Could not open required files. Exiting program." << endl;
        return -1;
    }
    if (!outFile.is_open() || !outErr.is_open()) {
        cerr << "Error: Could not open output files. Exiting program." << endl;
        return -1;
    }

    vector<Media*> myLib;

    // Read data into myLib
    try {
        readMediaList(inList,outFile, outErr, myLib);
    }
    catch (const ifstream::failure& e) {
        cerr << "Error reading from mediaList.txt: " << e.what() << endl;
        return -1;
    }
    inList.close();

    // Process each command from mediaCommands.txt
    string line;
    while (getline(commandsFile, line)) {
        // processCommand(line, myLib, outFile, outErr); // Ensure this matches the prototype in Functions.h
        processCommands("mediaCommands.txt", outFile, outErr, myLib);
        return 0;

    }

    // Clean up dynamically allocated memory
    for (auto media : myLib) {
        delete media;
    }

    outFile.close();
    outErr.close();
    commandsFile.close();

    return 0;
}
