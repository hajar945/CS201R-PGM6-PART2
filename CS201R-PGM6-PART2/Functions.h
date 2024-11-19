#pragma once
#include <vector>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include "Media.h"
using namespace std;

bool isDigits(string str1);
bool checkIfDigits(const string& str);

//int  readMediaList(istream& i, ostream& o, vector<Media*>& m);
int readMediaList(istream& inFile, ostream& outFile, ostream& outErr, vector<Media*>& mediaLib);

//void processCommand(const string& command, const vector<Media*>& mediaLib, ostream& report);

//void processCommands(const std::string& filename, ofstream& outFile, vector<Media*>& mediaLib);
//void processCommands(const std::string & filename, std::ofstream & outFile, std::vector<Media*>&mediaLib);


void processCommands(const std::string& filename, std::ofstream& outFile, std::ofstream& outErr, std::vector<Media*>& mediaLib);

void processFile(const string& filename, ostream& outFile, ostream& outErr, vector<Media*>& mediaLib);



void printAllMedia(ostream& outFile, const vector<Media*>& mediaLib);
void printMovies(ostream& outFile, const vector<Media*>& mediaLib);
void printBooks(ostream& outFile, const vector<Media*>& mediaLib);
void printSongs(ostream& outFile, const vector<Media*>& mediaLib);
//void printTotals(ostream& outFile, const vector<Media*>& mediaLib);


//void printByRating(ostream& outFile, const vector<Media*>& mediaLib, int rating);

//void addNewMedia(ostream& outFile, const string& type, const string& title, const string& name,
  //  int rating, const string& genre, int length, int year, vector<Media*>& mediaLib);
void addNewMedia(ostream& outFile, const string& type, const string& title, const string& name,
    int rating, const string& genre, int length, int year, vector<Media*>& mediaLib,
    const vector<string>& stars, int weeksNYT, int top40);

void printByGenre(ostream& outFile, const vector<Media*>& mediaLib, const string& genre, char type);

void logError(const std::string& message);


//void printTotalCounts(ofstream& outFile, const vector<Media*>& myLib);





// Function prototypes
//void processCommand(const string& command, vector<Media*>& myLib, ofstream& outFile, ofstream& outErr);
void listAllMedia(ofstream& outFile, const vector<Media*>& myLib, int rating = -1, const string& genre = "");
void listMediaByType(ofstream& outFile, const vector<Media*>& myLib, char type, int rating = -1, const string& genre = "");
void printTotalCounts(ofstream& outFile, const vector<Media*>& myLib);
//void addNewMedia(const string& data, vector<Media*>& myLib, ofstream& outErr);

Media* createMedia(char typeChar, const string& title, const string& creator, int rating,
    const string& genre, int length, int year, const vector<string>& stars,
    int weeksNYT, bool top40);
