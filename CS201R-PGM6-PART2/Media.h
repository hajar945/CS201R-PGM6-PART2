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

    // Pure virtual function to get additional fields for each media type
    virtual string getOtherFields() const = 0;
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
    string getOtherFields() const override; // Override to provide additional fields for Movie
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
    string getOtherFields() const override; // Override to provide additional fields for Book
};

// Song class
class Song : public Media {
private:
    bool top40;

public:
    Song(char type, string title, string creator, int rating, string genre, int length, int yearReleased, bool top40);
    bool getTop40() const;
    void setTop40(bool t);
    void print(ostream& os, vector<Media*> mediaList) override;
    string getOtherFields() const override; // Override to provide additional fields for Song
};

// Declaration of createMedia function
Media* createMedia(char typeChar, const string& title, const string& creator, int rating,
    const string& genre, int length, int year, const vector<string>& stars,
    int weeksNYT, bool top40);
