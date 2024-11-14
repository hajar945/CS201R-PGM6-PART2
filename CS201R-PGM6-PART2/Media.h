#pragma once
#include <string>
#include <iostream>
using namespace std;

class Media {

private:
    char type;
    string title;
    string name;
    int rating;
    string genre;
    int length;
    int yearReleased;

public:
    Media();

    // add logic for these methods
    Media(char type, string title, string name, int rating, string genre, int length, int yearReleased);

    // Setters
    void setType(char c);
    void setTitle(string t);
    void setName(string n);
    void setRating(int r);
    void setGenre(string g);
    void setLength(int l);
    void setYearReleased(int yr);

    // Getters
    char getType();
    string getTitle();
    string getName();
    int getRating();
    string getGenre();
    int getLength();
    int getYearReleased();


};
