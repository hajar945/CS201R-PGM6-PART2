// Book.cpp
#include "Book.h"
#include <iostream>

Book::Book(char type, std::string title, std::string name, int rating, std::string genre, int length, int yearReleased, int weeksNYT)
    : Media(type, title, name, rating, genre, length, yearReleased), weeksNYT(weeksNYT) {}

int Book::getWeeks() {
    return weeksNYT;
}

void Book::setWeeks(int weeksNYT) {
    this->weeksNYT = weeksNYT;
}

// Corrected print function implementation
void Book::print(std::ostream& out) const {
    out << "Title: " << title << ", Year: " << yearReleased 
        << ", Rating: " << rating << ", Genre: " << genre
        << ", Length: " << length << " pages, Weeks on NYT: " << weeksNYT << "\n";
}
