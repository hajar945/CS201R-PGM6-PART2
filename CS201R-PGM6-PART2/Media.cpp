#include <string>
#include <iostream>
#include "Media.h"
#include <iomanip> // Required for setw()

using namespace std;

// Default constructor
Media::Media() {
    this->type = ' ';
    this->title = " ";
    this->name = " ";
    this->rating = 0.0f;  // Default to 0.0 for float
    this->genre = " ";
    this->length = 0;  // Default to 0 for length
    this->yearReleased = 0;  // Default to 0 for year
}

// Parameterized constructor with debug output
Media::Media(char type, string title, string name, float rating, string genre, int length, int yearReleased) {
    this->type = type;
    this->title = title;
    this->name = name;
    this->rating = rating;
    this->genre = genre;
    this->length = length;
    this->yearReleased = yearReleased;

    // Debugging output to verify values
    cout << left
        << setw(20) << title
        << setw(20) << type
        << setw(20) << name
        << setw(20) << rating
        << setw(20) << genre
        << setw(20) << (to_string(length) + " mins")
        << setw(20) << yearReleased << endl;
}

// Getter methods
string Media::getGenre() const {
    return genre;
}

char Media::getType() const {
    return type;
}

string Media::getTitle() const {
    return title;
}

string Media::getName() const {
    return name;
}

float Media::getRating() const {
    return rating;  // Return rating as float
}

int Media::getLength() const {
    return length;
}

int Media::getYearReleased() const {
    return yearReleased;
}
