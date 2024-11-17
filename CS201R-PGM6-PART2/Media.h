#pragma once
#include <string>
#include <iostream>
#include <vector>
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

    // Pure virtual function
    virtual void print(ostream&, vector<Media*>) = 0;
};

// Movie class
class Movie : public Media {
private:
    vector<string> stars;

public:
    Movie(char type, string title, string keyName, int rating, string genre, int length, int yearReleased, vector<string> stars);
    vector<string> getStars();
    void setStars(vector<string> s);
    void print(ostream& os, vector<Media*> mediaList) override;
};

// Book class
class Book : public Media {
private:
    int weeksNYT;

public:
    Book(char type, string title, string keyName, int rating, string genre, int length, int yearReleased, int weeksNYT);
    int getWeeks();
    void setWeeks(int w);
    void print(ostream& os, vector<Media*> mediaList) override;
};

// Song class
class Song : public Media {
private:
    bool top40;

public:
    Song(char type, string title, string keyName, int rating, string genre, int length, int yearReleased, bool top40);
    bool getTop40();
    void setTop40(bool t);
    void print(ostream& os, vector<Media*> mediaList) override;
};
