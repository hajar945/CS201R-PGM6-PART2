#include "Song.h"
#include <iostream>
#include <iomanip>

Song::Song(char type, std::string title, std::string name, int rating, std::string genre, int length, int yearReleased, bool top40)
    : Media(type, title, name, rating, genre, length, yearReleased), top40(top40) {}

bool Song::getTop40() {
    return top40;
}

void Song::setTop40(bool top40) {
    this->top40 = top40;
}

void Song::print(std::ostream& out) const {
    out << std::left << std::setw(30) << title
        << std::setw(10) << yearReleased
        << std::setw(10) << rating
        << "Top 40: " << (top40 ? "Yes" : "No") << "\n";
}
