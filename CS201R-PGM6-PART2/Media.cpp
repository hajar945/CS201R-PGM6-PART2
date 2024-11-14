#include <string>
#include <iostream>
#include "Media.h"
#include <iomanip> // Required for setw()


using namespace std;



Media::Media() {
    this->type = ' ';
    this->title = " ";
    this->name = " ";
    this->rating = -1;
    this->genre = " ";
    this->length = -1;
    this->yearReleased = -1;

}



// Parameterized constructor
// Parameterized constructor with debug output
Media::Media(char type, string title, string name, int rating, string genre, int length, int yearReleased) {
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


// Definition for getGenre
string Media::getGenre() {
    return genre;  // Return the genre member variable
}

char Media::getType() {
    return type;
}

string Media::getTitle() {
    return title;
}

string Media::getName() {
    return name;
}

int Media::getRating() {
    return rating;
}

int Media::getLength() {
    return length;
}

int Media::getYearReleased() {
    return yearReleased;
}

