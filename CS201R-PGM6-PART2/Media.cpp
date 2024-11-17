#include <string>
#include <iostream>
#include "Media.h"
#include <iomanip> // Required for setw()

using namespace std;

// Media class constructor definitions
Media::Media() {
    this->type = ' ';
    this->title = " ";
    this->name = " ";
    this->rating = -1;
    this->genre = " ";
    this->length = -1;
    this->yearReleased = -1;
}

// Getter functions
char Media::getType() {
    return type;
}

string Media::getTitle() {
    return title;  // This should return the title of the media item
}

string Media::getName() {
    return name;  // This should return the name of the creator
}

int Media::getRating() {
    return rating;  // This should return the rating of the media item
}

string Media::getGenre() {
    return genre;  // This should return the genre of the media
}

int Media::getLength() {
    return length;  // This should return the length of the media item
}

int Media::getYearReleased() {
    return yearReleased;  // This should return the year the media was released
}


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

// Member function implementations for the Movie, Book, and Song classes

// Movie class constructor definition
Movie::Movie(char type, string title, string name, int rating, string genre, int length, int yearReleased, vector<string> stars)
    : Media(type, title, name, rating, genre, length, yearReleased), stars(stars) {}



vector<string> Movie::getStars() { return stars; }
void Movie::setStars(vector<string> s) { stars = s; }

void Movie::print(ostream& os, vector<Media*> mediaList) {
    os << "Movie: " << getTitle() << " by " << getName() << endl;
    os << "Stars: ";
    for (const string& star : stars)
        os << star << " ";
    os << endl;
}

// Book class constructor definition
Book::Book(char type, string title, string name, int rating, string genre, int length, int yearReleased, int weeksNYT)
    : Media(type, title, name, rating, genre, length, yearReleased), weeksNYT(weeksNYT) {}

int Book::getWeeks() { return weeksNYT; }
void Book::setWeeks(int w) { weeksNYT = w; }

void Book::print(ostream& os, vector<Media*> mediaList) {
    os << "Book: " << getTitle() << " by " << getName() << endl;
    os << "Weeks on NYT bestseller: " << weeksNYT << endl;
}

// Song class constructor definition
Song::Song(char type, string title, string keyName, int rating, string genre, int length, int yearReleased, bool top40)
    : Media(type, title, keyName, rating, genre, length, yearReleased), top40(top40) {}

bool Song::getTop40() { return top40; }
void Song::setTop40(bool t) { top40 = t; }

void Song::print(ostream& os, vector<Media*> mediaList) {
    os << "Song: " << getTitle() << " by " << getName() << endl;
    os << (top40 ? "Top 40 hit" : "Not a Top 40 hit") << endl;
}


Media* createMedia(char typeChar, const string& title, const string& creator, int rating,
    const string& genre, int length, int year, const vector<string>& stars,
    int weeksNYT, int top40) {
    if (typeChar == 'M') {
        return new Movie(typeChar, title, creator, rating, genre, length, year, stars);
    }
    else if (typeChar == 'B') {
        return new Book(typeChar, title, creator, rating, genre, length, year, weeksNYT);
    }
    else if (typeChar == 'S') {
        return new Song(typeChar, title, creator, rating, genre, length, year, top40);
    }
    return nullptr;
}
