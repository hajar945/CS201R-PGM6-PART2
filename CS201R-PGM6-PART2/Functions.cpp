#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Functions.h"
#include "Media.h"

#include "Movie.h"
#include "Book.h"
#include "Song.h"


using namespace std;

// Utility function to print media details
void print(ofstream& outFile, int lineNumber, Media& media) {
    outFile << left
        << setw(10) << lineNumber
        << setw(30) << media.getTitle()
        << setw(6) << media.getYearReleased()
        << setw(6) << media.getRating()
        << endl;
}

// Utility to check if a string is all digits
bool checkIfDigits(const string& str) {
    if (str.empty()) return false;
    for (char ch : str) {
        if (!isdigit(ch)) return false;
    }
    return true;
}

// Reads and validates the media list from the file
int readMediaList(istream& inFile, ostream& outFile, vector<Media*>& mediaLib) {
    string line;
    int lineNum = 0;

    while (std::getline(inFile, line)) {
        lineNum++;
        istringstream ss(line); // istringstream is reading from the medialist.txt
        string typeStr, title, creator, ratingStr, genre, lengthStr, yearStr;
        int rating, length, year;

        // Parse fields
        std::getline(ss, typeStr, ',');

        // Check for 'Q' to quit the program
        if (typeStr == "Q") {
            break; // Exit the loop if 'Q' is encountered
        }

        std::getline(ss, title, ',');
        std::getline(ss, creator, ',');
        std::getline(ss, ratingStr, ',');
        std::getline(ss, genre, ',');
        std::getline(ss, lengthStr, ',');
        std::getline(ss, yearStr, ',');

        bool valid = true;

        // Validate numeric fields only if typeStr is not 'Q'
        if (typeStr != "Q") {
            if (!checkIfDigits(ratingStr) || !checkIfDigits(lengthStr) || !checkIfDigits(yearStr)) {
                outFile << "Error on line " << lineNum << ": invalid numeric input\n";
                valid = false;
            }
            else {
                try {
                    rating = stoi(ratingStr);
                    length = stoi(lengthStr);
                    year = stoi(yearStr);
                }
                catch (const invalid_argument&) {
                    outFile << "Error on line " << lineNum << ": Invalid numeric value\n";
                    valid = false;
                    continue;
                }
                catch (const out_of_range&) {
                    outFile << "Error on line " << lineNum << ": Numeric value out of range\n";
                    valid = false;
                    continue;
                }

                if (rating <= 0 || rating > 10 || length <= 0 || year < 1000 || year > 2024) {
                    outFile << "Error on line " << lineNum << ": Invalid value in rating, length, or year\n";
                    valid = false;
                }
            }
        }

        // Validate type
        if (typeStr != "M" && typeStr != "B" && typeStr != "S" && typeStr != "N") {
            outFile << "Error on line " << lineNum << ": Invalid type '" << typeStr << "'\n";
            valid = false;
        }

       
    }

    return mediaLib.size();
}

// Processes the file to read media list
void processFile(const string& filename, ostream& outFile, vector<Media*>& mediaLib) {
    ifstream inFile(filename);
    if (!inFile) {
        outFile << "Error: Could not open file " << filename << endl;
        return;
    }

    int validEntries = readMediaList(inFile, outFile, mediaLib);
    inFile.close();
}


// Prints all movies
void printMovies(ostream& outFile, const vector<Media*>& mediaLib) {
    outFile << "Movies:\n";
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M') {
            outFile << media->getTitle() << " | Rating: " << media->getRating() << endl;
        }
    }

}


// Prints all books
void printBooks(ostream& outFile, const vector<Media*>& mediaLib) {
    outFile << "Books:\n";
    for (const auto& media : mediaLib) {
        if (media->getType() == 'B') {
            outFile << media->getTitle() << " | Rating: " << media->getRating() << endl;
        }
    }

}


// Prints all songs
void printSongs(ostream& outFile, const vector<Media*>& mediaLib) {
    outFile << "Songs:\n";
    for (const auto& media : mediaLib) {
        if (media->getType() == 'S') {
            outFile << media->getTitle() << " | Rating: " << media->getRating() << endl;
        }
    }

}


//  Print all media items report
void printAllMedia(ostream& outFile, const vector<Media*>& mediaLib) {
    outFile << "All Media:\n";
    for (const auto& media : mediaLib) {
        outFile << media->getTitle() << " (" << media->getType() << ") by "
            << media->getName() << " | Rating: " << media->getRating()
            << " | Genre: " << media->getGenre() << " | Year: " << media->getYearReleased() << endl;
    }

}

// Print media by genre and type
void printByGenre(ostream& outFile, const vector<Media*>& mediaLib, const string& genre, char type) {
    bool found = false; // To track if any media matched

    outFile << "Media in Genre: " << genre << " of Type: " << type << "\n";
    for (const auto& media : mediaLib) {
        // Filter by genre and media type
        if (media->getGenre() == genre && media->getType() == type) {
            found = true;
            outFile << media->getTitle() << " (" << media->getType() << ") | Rating: "
                << media->getRating() << " | Released: " << media->getYearReleased() << endl;
        }
    }
    if (!found) {
        outFile << "No media found in genre: " << genre << " for type: " << type << endl;
    }

}


//  Prints media by rating
void printByRating(ostream& outFile, const vector<Media*>& mediaLib, int rating) {
    outFile << "Media with Rating >= " << rating << ":\n";
    for (const auto& media : mediaLib) {
        if (media->getRating() >= rating) {
            outFile << media->getTitle() << " | Rating: " << media->getRating() << endl;
        }
    }

}

//  Print total media counts
void printTotals(ostream& outFile, const vector<Media*>& mediaLib) {
    int movieCount = 0, bookCount = 0, songCount = 0;
    for (const auto& media : mediaLib) {
        if (media->getType() == 'M') ++movieCount;
        else if (media->getType() == 'B') ++bookCount;
        else if (media->getType() == 'S') ++songCount;
    }
    outFile << "Total Counts - Movies: " << movieCount
        << ", Books: " << bookCount
        << ", Songs: " << songCount << endl;

}

bool validateSong(const std::vector<std::string>& fields, std::string& error, Song& song) {
    if (fields.size() != 8) { // Ensure the record has the correct number of fields
        error = "Incorrect number of fields.";
        return false;
    }

    char type = fields[0][0];
    if (type != 'M' && type != 'B' && type != 'S') {
        error = "Invalid media type.";
        return false;
    }

    std::string title = fields[1];
    std::string artist = fields[2];
    std::string genre = fields[4];

    int rating, length, yearReleased, top40;

    try {
        rating = std::stoi(fields[3]);
        length = std::stoi(fields[5]);
        yearReleased = std::stoi(fields[6]);
        top40 = std::stoi(fields[7]);
    }
    catch (...) {
        error = "Non-numeric value in numeric field.";
        return false;
    }

    if (rating <= 0 || rating > 10) {
        error = "Invalid rating (must be 1-10).";
        return false;
    }

    if (length <= 0) {
        error = "Invalid length (must be > 0).";
        return false;
    }

    if (yearReleased < 1920 || yearReleased > 2024) {
        error = "Invalid year released (must be between 1920 and 2024).";
        return false;
    }

    if (top40 != 0 && top40 != 1) {
        error = "Invalid top40 value (must be 0 or 1).";
        return false;
    }

    // If everything is valid, create the Song object
    song = Song(type, title, artist, rating, genre, length, yearReleased, static_cast<bool>(top40));
    return true; // All checks passed
}




void addNewMedia(ostream& outFile, const string& type, const string& title, const string& name,
    int rating, const string& genre, int length, int year, vector<Media*>& mediaLib) {

    char mediaType = type.empty() ? 'N' : type[0]; // Handle empty type case
    // Validate input data for the resolved mediaType
    if ((mediaType == 'M' || mediaType == 'B' || mediaType == 'S') &&
        rating > 0 && rating <= 10 && length > 0 && year >= 1000 && year <= 2024) {

    Media* mediaItem = nullptr;

    // Create a new media item based on the type
    if (mediaType == 'M') {
        vector<string> stars; // Replace with actual star names as needed
        mediaItem = new Movie(mediaType, title, name, rating, genre, length, year, stars);
    }
    else if (mediaType == 'B') {
        int weeksNYT = 0; // Replace with the actual weeks on NYT as needed
        mediaItem = new Book(mediaType, title, name, rating, genre, length, year, weeksNYT);
    }
    else if (mediaType == 'S') {
        bool top40 = false; // Replace with actual top 40 status as needed
        mediaItem = new Song(mediaType, title, name, rating, genre, length, year, top40);
    }

    // Add to mediaLib vector if mediaItem was successfully created
    if (mediaItem) {
        mediaLib.push_back(mediaItem);
        outFile << endl << title << " [" << mediaType << "]" << " was added to your list" << endl;
    }
    else {
        outFile << "Error: Failed to create media item for type '" << mediaType << "'" << endl;
    }
} else {
    outFile << "Error: Invalid data for media type '" << mediaType << "'" << endl;
    }
}

void logError(const std::string& message) {
    std::ofstream errorFile("error.txt", std::ios::app);
    if (errorFile.is_open()) {
        errorFile << "Error: " << message << std::endl;
        errorFile.close();
    }
    else {
        std::cerr << "Unable to open error log file." << std::endl;
    }
}

void processCommands(const std::string& filename, std::ofstream& outFile, std::vector<Media*>& mediaLib) {

    std::ifstream commandFile(filename);
    std::string commandLine;

    if (!commandFile) {
        std::cerr << "Error opening command file." << std::endl;
        return;
    }

    while (std::getline(commandFile, commandLine)) {
        std::istringstream commandStream(commandLine);
        std::string command;
        std::getline(commandStream, command, ',');

        // Debugging output to check the command
        std::cout << "Processing command: " << command << std::endl;

        // Check for command 'Q' to quit processing
        if (command == "Q") {
            break;  // End command processing
        }

    

        // Handle 'A' command with an optional integer
        if (command == "A") {
            std::string token;
            if (std::getline(commandStream, token, ',')) {
                // Check if the token is a number
                if (std::all_of(token.begin(), token.end(), ::isdigit)) {
                    int filterRating = std::stoi(token);  // Convert token to an integer
                    outFile << "\n\nALL MEDIA WITH RATING: " << filterRating << ":\n";  // New message
                    outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year" << std::setw(6) << "Rating" << "\n";
                    outFile << std::string(65, '-') << "\n";  // Print a line separator


                    bool found = false;
                    for (size_t i = 0; i < mediaLib.size(); ++i) {
                        const auto& media = mediaLib[i];
                        if (media->getRating() == filterRating) {
                            outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                << std::setw(40) << media->getTitle()
                                << std::setw(10) << media->getYearReleased()
                                << std::setw(10) << media->getRating()
                                << "\n";
                            found = true;
                        }
                    }
                }
                else {
                    outFile << "Invalid rating: " << token << "\n";
                }

            }
            else {
                // No rating specified, print all media
                outFile << "\n\nALL MEDIA: " << token << "\n";
                outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year" << std::setw(5) << "Rating" << "\n";
                outFile << std::string(60, '-') << "\n";  // Print a line separator
                bool found = false;
                for (size_t i = 0; i < mediaLib.size(); ++i) {
                    const auto& media = mediaLib[i];


                    outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                        << std::setw(40) << media->getTitle()
                        << std::setw(10) << media->getYearReleased()
                        << std::setw(10) << media->getRating()
                        << "\n";
                }
            }
        }


        if (command == "M") {
            std::string token;
            if (std::getline(commandStream, token, ',')) {

                if (std::all_of(token.begin(), token.end(), ::isdigit)) {
                    int filterRating = std::stoi(token);
                    if (filterRating >= 1 && filterRating <= 10) {
                        outFile << "\n\nMOVIE LIST WITH RATING >= " << filterRating << ":\n";
                        outFile << std::left << std::setw(5) << "#" << std::setw(30) << "Title" << std::setw(8) << "Year" << std::setw(1) << "Rating" << "\n";
                        outFile << std::string(75, '-') << "\n";  // Print a line separator
                        bool found = false;

                        for (size_t i = 0; i < mediaLib.size(); ++i) {
                            const auto& media = mediaLib[i];
                            if (media->getType() == 'M' && media->getRating() >= static_cast<float>(filterRating)) {
                                // Print line number, title, year, and rating in neat columns
                                outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                    << std::setw(30) << media->getTitle()
                                    << std::setw(10) << media->getYearReleased()
                                    << std::setw(10) << std::fixed << std::setprecision(2) << media->getRating();  // Print rating with 2 decimals
                                found = true;
                            }
                        }

                        if (!found) {
                            outFile << "No movies found with rating >= " << filterRating << ".\n";
                        }
                    }
                    else {
                        cerr << "Invalid rating for command M: " << token << " (must be between 1 and 10)\n";
                    }
                }
                else {
                    // Treat token as a genre if it’s not numeric
                    outFile << "\n\nMOVIE LIST FOR GENRE: " << token << "\n";
                    outFile << std::left << std::setw(5) << "#" << std::setw(30) << "Title" << std::setw(8) << "Year" << std::setw(1) << "Rating" << "\n";
                    outFile << std::string(60, '-') << "\n";  // Print a line separator

                    bool found = false;
                    for (size_t i = 0; i < mediaLib.size(); ++i) {
                        const auto& media = mediaLib[i];
                        if (media->getType() == 'M' && media->getGenre() == token) {
                            // Print line number, title, year, and rating in neat columns
                            outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                << std::setw(30) << media->getTitle()
                                << std::setw(10) << media->getYearReleased()
                                << std::setw(10) << std::fixed << std::setprecision(2) << media->getRating()
                                << "\n";
                            found = true;
                        }
                    }

                    if (!found) {
                        outFile << "No movies found with genre: " << token << ".\n";
                    }
                }
            }
            else {
                outFile << "\n\nALL MOVIES:\n";
                outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(8) << "Year" << std::setw(1) << "Rating" << "\n";
                outFile << std::string(75, '-') << "\n";  // Print a line separator

                bool found = false;
                for (size_t i = 0; i < mediaLib.size(); ++i) {
                    const auto& media = mediaLib[i];
                    if (media->getType() == 'M') {
                        // Print line number, title, year, and rating in neat columns
                        outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                            << std::setw(40) << media->getTitle()
                            << std::setw(10) << media->getYearReleased()
                            << std::setw(10) << std::fixed << std::setprecision(2) << media->getRating()  // Print rating with 2 decimals
                            << "\n";
                        found = true;
                    }
                }

                if (!found) {
                    outFile << "No movies found.\n";
                }
            }
        }


        if (command == "B") {
            std::string token;
            if (std::getline(commandStream, token, ',')) {
                if (std::all_of(token.begin(), token.end(), ::isdigit)) {
                    int filterRating = std::stoi(token);
                    if (filterRating >= 1 && filterRating <= 10) {
                        outFile << "\n\nBOOK LIST WITH RATING >= " << filterRating << ":\n";
                        outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year" << std::setw(10) << "Rating" << "\n";
                        outFile << std::string(65, '-') << "\n";  // Print a line separator

                        bool found = false;
                        for (size_t i = 0; i < mediaLib.size(); ++i) {
                            const auto& media = mediaLib[i];
                            if (media->getType() == 'B' && media->getRating() >= filterRating) {
                                // Print line number, title, year, and rating in neat columns
                                outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                    << std::setw(40) << media->getTitle()
                                    << std::setw(10) << media->getYearReleased()
                                    << std::setw(10) << media->getRating()
                                    << "\n";
                                found = true;
                            }
                        }

                        if (!found) {
                            outFile << "No books found with rating >= " << filterRating << ".\n";
                        }
                    }
                    else {
                        cerr << "Invalid rating for command B: " << token << " (must be between 1 and 10)\n";
                        outFile << "\n\nInvalid rating for command B: " << token << " (must be between 1 and 10)\n";

                    }
                }
                else {
                    // Treat token as a genre if it’s not numeric
                    outFile << "\n\nBOOK LIST FOR GENRE: " << token << "\n";
                    outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year" << std::setw(3) << "Rating" << "\n";
                    outFile << std::string(60, '-') << "\n";  // Print a line separator


                    bool found = false;
                    for (size_t i = 0; i < mediaLib.size(); ++i) {
                        const auto& media = mediaLib[i];
                        if (media->getType() == 'B' && media->getGenre() == token) {
                            // Cast the media to Movie* to access Movie-specific methods
                            Movie* movie = static_cast<Movie*>(media);

                            outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                << std::setw(40) << media->getTitle()
                                << std::setw(10) << media->getYearReleased()
                                << std::setw(10) << media->getRating()
                                << std::setw(10) <<  "\n";

                            // Output stars by accessing Movie-specific getStars() method
                            outFile << std::setw(20); // Adjust the width for stars output
                            for (const auto& star : movie->getStars()) {
                                outFile << star << " ";  // Print each star
                            }


                            found = true;
                        }
                    }
                    if (!found) {
                        outFile << "No movies found with genre: " << token << ".\n";
                    }
                }
            }
            else {
                outFile << "\n\nALL BOOKS: \n";
                outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year" << std::setw(3) << "Rating" << "\n";
                outFile << std::string(60, '-') << "\n";  // Print a line separator


                bool found = false;
                for (size_t i = 0; i < mediaLib.size(); ++i) {
                    const auto& media = mediaLib[i];
                    if (media->getType() == 'B') {

                        outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                            << std::setw(40) << media->getTitle()
                            << std::setw(10) << media->getYearReleased()
                            << std::setw(10) << media->getRating()
                            << "\n";
                        found = true;
                    }

                }
                if (!found) {
                    outFile << "No books found.\n";
                }

            }


        }
        if (command == "S") {
            std::string token;
            if (std::getline(commandStream, token, ',')) {
                if (std::all_of(token.begin(), token.end(), ::isdigit)) {
                    int filterRating = std::stoi(token);
                    if (filterRating >= 1 && filterRating <= 10) {
                        outFile << "\nSONG LIST WITH RATING >= " << filterRating << ":\n";
                        outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year"
                            << std::setw(5) << "Rating" << "\n";
                        outFile << std::string(70, '-') << "\n";  // Print a line separator

                        bool found = false;
                        for (size_t i = 0; i < mediaLib.size(); ++i) {
                            const auto& media = mediaLib[i];
                            if (media->getType() == 'S' && media->getRating() >= filterRating) {
                                // Cast to Song* to access getStars()
                                Song* song = dynamic_cast<Song*>(media);
                                if (song) {
                                    outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                        << std::setw(40) << song->getTitle()
                                        << std::setw(10) << song->getYearReleased()
                                        << std::setw(10) << song->getRating()
                                        << std::setw(12) << "\n";
                                    found = true;
                                }
                            }
                        }
                        if (!found) {
                            outFile << "No songs found with rating >= " << filterRating << ".\n";
                        }

                    }
                    else {
                        cerr << "Invalid rating for command S: " << token << " (must be between 1 and 10)\n";
                    }
                }
                else {
                    // Treat token as a genre if it’s not numeric
                    outFile << "\nSONG LIST FOR GENRE: " << token << "\n";
                    outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year"
                        << std::setw(5) << "Rating" << std::setw(12) << "Stars" << "\n";
                    outFile << std::string(70, '-') << "\n";  // Print a line separator

                    bool found = false;
                    for (size_t i = 0; i < mediaLib.size(); ++i) {
                        const auto& media = mediaLib[i];
                        if (media->getType() == 'S' && media->getGenre() == token) {
                            // Cast to Song* to access getStars()
                            Song* song = dynamic_cast<Song*>(media);
                            if (song) {
                                outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                    << std::setw(40) << song->getTitle()
                                    << std::setw(10) << song->getYearReleased()
                                    << std::setw(10) << song->getRating()
                                    << std::setw(12) << "\n";
                                found = true;
                            }
                        }
                    }
                    if (!found) {
                        outFile << "No songs found with genre " << token << ".\n";
                    }
                }

            }
            else {
                outFile << "\n\nALL SONGS:\n";
                outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year"
                    << std::setw(5) << "Rating" << std::setw(12) << "Stars" << "\n";
                outFile << std::string(70, '-') << "\n";  // Print a line separator
                bool found = false;
                for (size_t i = 0; i < mediaLib.size(); ++i) {
                    const auto& media = mediaLib[i];
                    if (media->getType() == 'S') {
                        // Cast to Song* to access getStars()
                        Song* song = dynamic_cast<Song*>(media);
                        if (song) {
                            outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                << std::setw(40) << song->getTitle()
                                << std::setw(10) << song->getYearReleased()
                                << std::setw(10) << song->getRating()
                                << std::setw(12) << "\n";
                            found = true;
                        }
                    }
                }
                if (!found) {
                    outFile << "No songs found.\n";
                }
            }
        }

        else if (command == "N") {
            std::string token;
            std::getline(commandStream, token, ',');
            string typeStr, title, creator, ratingStr, genre, lengthStr, yearStr;
            int rating, length, year;

           std:: getline(commandStream, title, ',');
           std::getline(commandStream, creator, ',');
           std::getline(commandStream, ratingStr, ',');
           std::getline(commandStream, genre, ',');
          std::getline(commandStream, lengthStr, ',');
           std::getline(commandStream, yearStr, ',');


            try {
                rating = stoi(ratingStr);
                length = stoi(lengthStr);
                year = stoi(yearStr);
            }
            catch (const invalid_argument&) {
                cerr << "Error with: " << title << " " << creator << " " << ratingStr << " " << genre << " "
                    << lengthStr << " " << yearStr << ": Invalid numeric value\n";
                continue;
            }
            catch (const out_of_range&) {
                cerr << "Error with: " << title << " " << creator << " " << ratingStr << " " << genre << " "
                    << lengthStr << " " << yearStr << ": Numeric value out of range\n";
                continue;
            }

            if (rating <= 0 || rating > 10 || length <= 0 || year < 1000 || year > 2024) {
                cerr << "Error with: " << title << " " << creator << " " << ratingStr << " " << genre << " "
                    << lengthStr << " " << yearStr << ": Invalid value in rating, length, or year\n";
            }


            addNewMedia(outFile, token, title, creator, rating, genre, length, year, mediaLib);  // Print total counts for each type

        
        }


        else if (command == "T") {  // Part 1T
            outFile << "\n\nYOUR MEDIA LIBRARY " << "\n";

            printTotals(outFile, mediaLib);  // Print total counts for each type


        }


        else if (command.substr(0, 2) == "A,") {  // Part 2A
            int rating;
            if (std::stringstream(command.substr(2)) >> rating && rating >= 1 && rating <= 10) {
                printByRating(outFile, mediaLib, rating);
            }
            else {
                logError("Invalid rating provided for A command: " + command);
                outFile << "Invalid rating: " << command << std::endl;
            }
        }
        else if (command.substr(0, 2) == "M,") {  // Part 2M
            int rating;
            if (std::stringstream(command.substr(2)) >> rating && rating >= 1 && rating <= 10) {
                printByRating(outFile, mediaLib, rating);  // Assuming printByRating filters by type
            }
            else {
                logError("Invalid rating provided for M command: " + command);
                outFile << "Invalid rating: " << command << std::endl;
            }
        }
        else if (command.substr(0, 2) == "B,") {  // Part 2B
            int rating;
            if (std::stringstream(command.substr(2)) >> rating && rating >= 1 && rating <= 10) {
                printByRating(outFile, mediaLib, rating);  // Assuming printByRating filters by type
            }
            else {
                logError("Invalid rating provided for B command: " + command);
                outFile << "Invalid rating: " << command << std::endl;
            }
        }
        else if (command.substr(0, 2) == "S,") {  // Part 2S
            int rating;
            if (std::stringstream(command.substr(2)) >> rating && rating >= 1 && rating <= 10) {
                printByRating(outFile, mediaLib, rating);  // Assuming printByRating filters by type
            }
            else {
                logError("Invalid rating provided for S command: " + command);
                outFile << "Invalid rating: " << command << std::endl;
            }
        }
        // Handle 'N' command to add new media
        else if (command.substr(0, 2) == "N,") {  // Part 4
            std::cout << "Command detected: " << command << std::endl;

            std::string type, title, name, ratingStr, genre, lengthStr, yearStr;
            int rating, length, year;

            std::getline(commandStream, type, ',');
            std::cout << "Type: " << type << std::endl;
            std::getline(commandStream, title, ',');
            std::cout << "Title: " << title << std::endl;
            std::getline(commandStream, name, ',');
            std::cout << "Name: " << name << std::endl;
            std::getline(commandStream, ratingStr, ',');
            std::cout << "Rating: " << ratingStr << std::endl;
            std::getline(commandStream, genre, ',');
            std::cout << "Genre: " << genre << std::endl;
            std::getline(commandStream, lengthStr, ',');
            std::cout << "Length: " << lengthStr << std::endl;
            std::getline(commandStream, yearStr, ',');
            std::cout << "Year: " << yearStr << std::endl;

            try {
                rating = std::stoi(ratingStr);
                length = std::stoi(lengthStr);
                year = std::stoi(yearStr);

                // Validate and add new media
                addNewMedia(outFile, type, title, name, rating, genre, length, year, mediaLib);
            }
            catch (const std::invalid_argument&) {
                std::cout << "Error: Invalid numeric input in N command: " << commandLine << std::endl;
                logError("Invalid numeric input in N command: " + commandLine);
                outFile << "Error parsing numeric values for N command: " << commandLine << std::endl;
            }
        }
    }

    commandFile.close();
}


// Function to process media data and create Media objects
void processMediaData(const string& data, vector<Media*>& myLib, ofstream& outErr) {
    // Check if the line starts with 'N'
    if (data[0] != 'N') {
        return; // Ignore this line if it doesn't start with 'N'
    }

    istringstream stream(data);
    string token;
    char type;
    string title, name, genre;
    int rating, length, year;

    // Read tokens from the stream
    std::getline(stream, token, ',');  // Skip 'N'
    std::getline(stream, token, ',');   // string1 (type)
    type = token.empty() ? ' ' : token[0]; // Convert to char safely
    std::getline(stream, title, ',');   // string2 (title)
    std::getline(stream, name, ',');    // string3 (name)

    std::getline(stream, token, ',');   // int1 (rating as string)
    rating = token.empty() ? -1 : stoi(token);  // Convert safely, -1 for invalid

    std::getline(stream, genre, ',');   // string4 (genre)

    std::getline(stream, token, ',');   // int2 (length as string)
    length = token.empty() ? -1 : stoi(token); // Convert safely, -1 for invalid

    std::getline(stream, token, ',');   // int3 (year as string)
    year = token.empty() ? -1 : stoi(token);  // Convert safely, -1 for invalid

    // Check if any values failed to parse or are invalid
    if (rating <= 0 || rating > 10 || length <= 0 || year < 1000 || year > 2024) {
        outErr << "Error processing record: " << data << endl; // Log error to the error file
        return;  // Skip this record
    }

    // Create specific media objects based on type
    Media* mediaItem = nullptr;
    if (type == 'M') {
        vector<string> stars; // Assuming Movie has a stars vector as additional data
        mediaItem = new Movie(type, title, name, rating, genre, length, year, stars);
    }
    else if (type == 'B') {
        int weeksNYT = 0; // Assuming Book requires weeks on NYT as additional data
        mediaItem = new Book(type, title, name, rating, genre, length, year, weeksNYT);
    }
    else if (type == 'S') {
        bool top40 = false; // Assuming Song has a top40 status as additional data
        mediaItem = new Song(type, title, name, rating, genre, length, year, top40);
    }
    else {
        outErr << "Error: Invalid media type '" << type << "' in record: " << data << endl;
        return; // Exit if type is invalid
    }

    // Add the valid media item to the library
    if (mediaItem) {
        myLib.push_back(mediaItem);
    }
}
// Function to read media data from a file
void readMediaList(ifstream& inList, ofstream& outErr, vector<Media*>& myLib) {
    string line;

    while (std::getline(inList, line)) {
        processMediaData(line, myLib, outErr);
    }
}