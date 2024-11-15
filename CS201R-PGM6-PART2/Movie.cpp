// Movie.cpp
#include "Movie.h"
#include <iostream>

Movie::Movie(char type, std::string title, std::string name, int rating, std::string genre, int length, int yearReleased, std::vector<std::string> stars)
    : Media(type, title, name, rating, genre, length, yearReleased), stars(stars) {}

std::vector<std::string> Movie::getStars() const {  // const qualifier added
    return stars;
}

void Movie::setStars(const std::vector<std::string>& stars) {
    this->stars = stars;
}

// Corrected print function implementation
void Movie::print(std::ostream& out) const {
    out << "Title: " << title << ", Year: " << yearReleased
        << ", Rating: " << rating << ", Genre: " << genre
        << ", Length: " << length << " mins, Stars: ";
    for (const auto& star : stars) {
        out << star << " ";
    }
    out << "\n";
}
