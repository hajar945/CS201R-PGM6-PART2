#pragma once
// Book.h
#include "Media.h"

class Book : public Media {
private:
    int weeksNYT;

public:
    Book(char type, std::string title, std::string name, int rating, std::string genre, int length, int yearReleased, int weeksNYT);

    int getWeeks();
    void setWeeks(int weeksNYT);

    // Override the print function with const
    void print(std::ostream& out) const override;
};
