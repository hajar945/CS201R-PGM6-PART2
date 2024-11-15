#pragma once
// Song.h
#include "Media.h"

class Song : public Media {
private:
    bool top40;

public:
    Song(char type, std::string title, std::string name, int rating, std::string genre, int length, int yearReleased, bool top40);

    bool getTop40();
    void setTop40(bool top40);

    void print(std::ostream& out) const override;
};
