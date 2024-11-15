#pragma once
// Movie.h
#include "Media.h"
#include <vector>
#include <string>

class Movie : public Media {
private:
    std::vector<std::string> stars;

public:
    Movie(char type, std::string title, std::string name, int rating, std::string genre, int length, int yearReleased, std::vector<std::string> stars);

    // Getter for stars
    std::vector<std::string> getStars() const;  // const qualifier added

    // Setter for stars
    void setStars(const std::vector<std::string>& stars);

    // Overridden print function
    void print(std::ostream& out) const override;
};
