#pragma once
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Media {
protected:
    char type;
    string title;
    string name;
    float rating;  // Changed to float to be consistent with the constructor and setter
    string genre;
    int length;
    int yearReleased;

public:
    Media();  // Default constructor

    // Parameterized constructor    
    Media(char type, string title, string name, float rating, string genre, int length, int yearReleased);

    // Setters
    void setType(char c);
    void setTitle(string t);
    void setName(string n);
    void setRating(float r);  // Updated to float to match the rating type
    void setGenre(string g);
    void setLength(int l);
    void setYearReleased(int yr);

    // Getters

    char getType() const;
    string getTitle() const;
    string getName() const;
    float getRating() const;
    string getGenre() const;
    int getLength() const;
    int getYearReleased() const;

    // Pure virtual function for printing
    virtual void print(std::ostream& out) const = 0;

    virtual ~Media() = default; // Virtual destructor for safe deletion of derived objects
};
