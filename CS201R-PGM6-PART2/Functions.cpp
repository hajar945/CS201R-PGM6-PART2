#include <algorithm>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Functions.h"
#include "Media.h"

using namespace std;

/*void printBorder(ostream& outFile) {
    outFile << "++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
}
*/
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
int readMediaList(istream& inFile, ostream& outFile, vector<Media*>& mediaLib) {
    string line;
    int lineNum = 0;

    if (!inFile) {
        outFile << "Error: Failed to open file\n";
        return -1;
    }

    while (getline(inFile, line)) {
        lineNum++;
        std::cout << "Processing line " << lineNum << ": " << line << std::endl;

        if (line.empty()) continue;

        istringstream ss(line);
        string typeStr, title, creator, ratingStr, genre, lengthStr, yearStr, starsStr, weeksNYTStr, top40Str;

        // Parse fields carefully
        getline(ss, typeStr, ',');
        if (typeStr == "Q") break; // Quit marker

        getline(ss, title, ',');
        getline(ss, creator, ',');
        getline(ss, ratingStr, ',');
        getline(ss, genre, ',');
        getline(ss, lengthStr, ',');
        getline(ss, yearStr, ',');

        // Ensure there's data after the year field
        if (ss.eof()) {
            outFile << "Error on line " << lineNum << ": Missing stars information after year\n";
            continue;  // Skip the line if there's no stars information
        }

        // Everything after the year is considered part of the stars
        starsStr = line.substr(ss.tellg());  // Take everything after the year
        if (!starsStr.empty() && starsStr[0] == ',') {
            starsStr = starsStr.substr(1);  // Remove the leading comma
        }
        // Debugging output
        std::cout << "Debug - Trimmed Stars value: '" << starsStr << "'" << std::endl;

        // Check if we can get the other optional fields
        if (typeStr == "S") {
            size_t lastCommaPos = line.find_last_of(',');
            if (lastCommaPos != string::npos) {
                top40Str = line.substr(lastCommaPos + 1);
            }
        }
        else if (typeStr == "B") {
            size_t lastCommaPos = line.find_last_of(',');
            if (lastCommaPos != string::npos) {
                weeksNYTStr = line.substr(lastCommaPos + 1);
            }
        }

        // Trim fields
        auto trim = [](string& s) { s.erase(0, s.find_first_not_of(" \t\r\n")); s.erase(s.find_last_not_of(" \t\r\n") + 1); };
        trim(typeStr); trim(title); trim(creator); trim(ratingStr);
        trim(genre); trim(lengthStr); trim(yearStr); trim(starsStr);
        trim(weeksNYTStr); trim(top40Str);

        // Validation
        bool valid = true;
        int rating = -1, length = -1, year = -1, weeksNYT = 0, top40 = 0;

        if (typeStr.empty() || title.empty() || creator.empty() || genre.empty()) {
            valid = false;
            outFile << "Error on line " << lineNum << ": Missing required fields\n";
        }

        if (!ratingStr.empty() && !isdigit(ratingStr[0])) valid = false;
        if (!lengthStr.empty() && !isdigit(lengthStr[0])) valid = false;
        if (!yearStr.empty() && !isdigit(yearStr[0])) valid = false;

        try {
            if (!ratingStr.empty()) rating = stoi(ratingStr);
            if (!lengthStr.empty()) length = stoi(lengthStr);
            if (!yearStr.empty()) year = stoi(yearStr);
            if (typeStr == "B" && !weeksNYTStr.empty()) weeksNYT = stoi(weeksNYTStr);
            if (typeStr == "S" && !top40Str.empty()) top40 = stoi(top40Str);
        }
        catch (const exception&) {
            valid = false;
            outFile << "Error on line " << lineNum << ": Invalid numeric data\n";
        }

        if (!valid) {
            outFile << "Error on line " << lineNum << ": Invalid data, skipping line\n";
            continue;
        }

        // Parse stars for movies
        vector<string> starsVec;
        if (typeStr == "M" && !starsStr.empty()) {
            istringstream starStream(starsStr);
            string star;
            while (getline(starStream, star, ',')) {
                trim(star);
                if (!star.empty()) starsVec.push_back(star);
            }
            // Debugging output for stars
            std::cout << "Debug - Movie: " << title << ", Stars: ";
            for (const auto& star : starsVec) {
                std::cout << star << " ";
            }
            std::cout << std::endl;
        }

        // Create media item
        Media* mediaItem = createMedia(typeStr[0], title, creator, rating, genre, length, year, starsVec, weeksNYT, top40);
        if (mediaItem) {
            mediaLib.push_back(mediaItem);
            std::cout << "Added media item: " << title << std::endl;
        }
        else {
            outFile << "Error on line " << lineNum << ": Failed to create media item\n";
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
/*

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
*/

/*//  Print all media items report
void printAllMedia(ostream& outFile, const vector<Media*>& mediaLib) {
    outFile << "All Media:\n";
    for (const auto& media : mediaLib) {
        outFile << media->getTitle() << " (" << media->getType() << ") by "
            << media->getName() << " | Rating: " << media->getRating()
            << " | Genre: " << media->getGenre() << " | Year: " << media->getYearReleased() << endl;
    }

}*/
/*
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
*/
/*

//  Prints media by rating
void printByRating(ostream& outFile, const vector<Media*>& mediaLib, int rating) {
    outFile << "Media with Rating >= " << rating << ":\n";
    for (const auto& media : mediaLib) {
        if (media->getRating() >= rating) {
            outFile << media->getTitle() << " | Rating: " << media->getRating() << endl;
        }
    }

}
*/
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

void addNewMedia(ostream& outFile, const string& type, const string& title, const string& name,
    int rating, const string& genre, int length, int year, vector<Media*>& mediaLib,
    const vector<string>& stars, int weeksNYT, int top40) {

    char mediaType = type.empty() ? 'N' : type[0]; // Handle empty type case

    // Validate input data for the resolved mediaType
    if ((mediaType == 'M' || mediaType == 'B' || mediaType == 'S') &&
        rating > 0 && rating <= 10 && length > 0 && year >= 1920 && year <= 2024) {

        // Create a new media item based on the type
        Media* mediaItem = nullptr;

        if (mediaType == 'M') {  // For Movie
            mediaItem = new Movie(mediaType, title, name, rating, genre, length, year, stars);
        }
        else if (mediaType == 'B') {  // For Book
            mediaItem = new Book(mediaType, title, name, rating, genre, length, year, weeksNYT);
        }
        else if (mediaType == 'S') {  // For Song
            mediaItem = new Song(mediaType, title, name, rating, genre, length, year, top40);
        }

        // Add to mediaLib vector if mediaItem was successfully created
        if (mediaItem != nullptr) {
            mediaLib.push_back(mediaItem);
            outFile << endl << title << " [" << mediaType << "]" << " was added to your list" << endl;
        }

    }
    else {
        // Handle invalid data cases with detailed output
        if (rating <= 0 || rating > 10) {
            cerr << "Error: Invalid rating for media item '" << title << "'\n";
        }
        if (length <= 0) {
            cerr << "Error: Invalid length for media item '" << title << "'\n";
        }
        if (year < 1920 || year > 2024) {
            cerr << "Error: Invalid year for media item '" << title << "'\n";
        }
        if (mediaType != 'M' && mediaType != 'B' && mediaType != 'S' && mediaType != 'N') {
            cerr << "Error: Unrecognized media type for '" << title << "'\n";
        }
    }
}


void logError(const std::string& message) {
    std::ofstream errorFile("error.txt", std::ios::app);
    if (errorFile.is_open()) {
        errorFile << message << std::endl;
        errorFile.close();
    }
    else {
        std::cerr << "Unable to open error log file." << std::endl;
    }
}
void processCommands(const std::string& filename, std::ofstream& outFile, std::vector<Media*>& mediaLib) {
    std::ifstream commandFile(filename);
    std::string commandLine;
    std::cout << "Media library size: " << mediaLib.size() << std::endl;

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

        if (command == "Q") {
            break;  // End command processing
        }

        if (command == "A") {
            std::string token;
            bool found = false;  // Flag to track if any media is found

            if (std::getline(commandStream, token, ',')) {
                std::cout << "Rating from command: " << token << std::endl;

                if (std::all_of(token.begin(), token.end(), ::isdigit)) {
                    int filterRating = std::stoi(token);  // Convert token to an integer
                    outFile << "\n\nALL MEDIA WITH RATING: " << filterRating << ":\n";
                    outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year" << std::setw(6) << "Rating" << "\n";
                    outFile << std::string(65, '-') << "\n";  // Print a line separator

                    for (size_t i = 0; i < mediaLib.size(); ++i) {
                        const auto& media = mediaLib[i];
                        std::cout << "Checking media: " << media->getTitle() << ", Rating: " << media->getRating() << std::endl;

                        if (media->getRating() == filterRating) {
                            outFile << std::left << std::setw(5) << (i + 1)
                                << std::setw(40) << media->getTitle()
                                << std::setw(10) << media->getYearReleased()
                                << std::setw(10) << media->getRating() << "\n";
                            found = true;
                        }

                    }

                    if (!found) {
                        outFile << "No media found with the specified rating." << std::endl;
                    }
                }
                else {
                    outFile << "Invalid rating: " << token << "\n";
                }
            }
            else {
                outFile << "\n\nALL MEDIA:\n";
                outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year" << std::setw(5) << "Rating" << "\n";
                outFile << std::string(60, '-') << "\n";  // Print a line separator

                for (size_t i = 0; i < mediaLib.size(); ++i) {
                    const auto& media = mediaLib[i];

                    std::cout << "Media " << (i + 1) << ": "
                        << media->getTitle() << ", "
                        << media->getYearReleased() << ", "
                        << media->getRating() << std::endl;

                    outFile << std::left << std::setw(5) << (i + 1)
                        << std::setw(40) << media->getTitle()
                        << std::setw(10) << media->getYearReleased()
                        << std::setw(10) << media->getRating() << "\n";
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
                        outFile << std::left << std::setw(5) << "#" << std::setw(30) << "Title" << std::setw(8) << "Year" << std::setw(10) << "Rating" << "Stars\n";
                        outFile << std::string(85, '-') << "\n";  // Print a line separator
                        bool found = false;
                        for (size_t i = 0; i < mediaLib.size(); ++i) {
                            const auto& media = mediaLib[i];
                            if (media->getType() == 'M' && media->getRating() >= filterRating) {
                                // Cast to Movie to access getStars()
                                Movie* movie = static_cast<Movie*>(media);

                                // Print line number, title, year, rating
                                outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                    << std::setw(30) << media->getTitle()
                                    << std::setw(10) << media->getYearReleased()
                                    << std::setw(10) << media->getRating();

                                // Print stars without commas, separated by spaces
                                for (size_t j = 0; j < movie->getStars().size(); ++j) {
                                    outFile << movie->getStars()[j];
                                    if (j != movie->getStars().size() - 1) {
                                        outFile << " ";  // Add a space between stars
                                    }
                                }

                                outFile << "\n";
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
                    outFile << std::left << std::setw(5) << "#" << std::setw(30) << "Title" << std::setw(8) << "Year" << std::setw(10) << "Rating" << "Stars\n";
                    outFile << std::string(85, '-') << "\n";  // Print a line separator

                    bool found = false;
                    for (size_t i = 0; i < mediaLib.size(); ++i) {
                        const auto& media = mediaLib[i];
                        if (media->getType() == 'M' && media->getGenre() == token) {
                            // Cast to Movie to access getStars()
                            Movie* movie = static_cast<Movie*>(media);

                            // Print line number, title, year, rating
                            outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                << std::setw(30) << media->getTitle()
                                << std::setw(10) << media->getYearReleased()
                                << std::setw(10) << media->getRating();

                            // Print stars without commas, separated by spaces
                            for (size_t j = 0; j < movie->getStars().size(); ++j) {
                                outFile << movie->getStars()[j];
                                if (j != movie->getStars().size() - 1) {
                                    outFile << " ";  // Add a space between stars
                                }
                            }

                            outFile << "\n";
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
                outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(8) << "Year" << std::setw(10) << "Rating" << "Stars\n";
                outFile << std::string(85, '-') << "\n";  // Print a line separator

                bool found = false;
                for (size_t i = 0; i < mediaLib.size(); ++i) {
                    const auto& media = mediaLib[i];
                    if (media->getType() == 'M') {
                        // Cast to Movie to access getStars()
                        Movie* movie = static_cast<Movie*>(media);

                        // Print line number, title, year, rating
                        outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                            << std::setw(40) << media->getTitle()
                            << std::setw(10) << media->getYearReleased()
                            << std::setw(10) << media->getRating();

                        // Print stars without commas, separated by spaces
                        for (size_t j = 0; j < movie->getStars().size(); ++j) {
                            outFile << movie->getStars()[j];
                            if (j != movie->getStars().size() - 1) {
                                outFile << " ";  // Add a space between stars
                            }
                        }

                        outFile << "\n";
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
                        outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year" << std::setw(10) << "Rating" <<
                            std::setw(10) << "Weeks on NYT" << "\n";
                        outFile << std::string(65, '-') << "\n";  // Print a line separator

                        bool found = false;
                        for (size_t i = 0; i < mediaLib.size(); ++i) {
                            const auto& media = mediaLib[i];
                            if (media->getType() == 'B' && media->getRating() >= filterRating) {
                                // Cast to Book to access getWeeks
                                Book* book = static_cast<Book*>(media);


                                // Print line number, title, year, and rating in neat columns
                                outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                    << std::setw(40) << media->getTitle()
                                    << std::setw(10) << media->getYearReleased()
                                    << std::setw(10) << media->getRating()
                                    << book->getWeeks()
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
                    outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year" << std::setw(10) << "Rating" << 
                        std::setw(10) << "Weeks on NYT" << "\n";
                    outFile << std::string(60, '-') << "\n";  // Print a line separator


                    bool found = false;
                    for (size_t i = 0; i < mediaLib.size(); ++i) {
                        const auto& media = mediaLib[i];
                        if (media->getType() == 'B' && media->getGenre() == token) {
                            // Cast to Book to access getWeeks
                            Book* book = static_cast<Book*>(media);


                            outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                                << std::setw(40) << media->getTitle()
                                << std::setw(10) << media->getYearReleased()
                                << std::setw(10) << media->getRating()
                                << book->getWeeks()
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
                outFile << "\n\nALL BOOKS: \n";
                outFile << std::left << std::setw(5) << "#" << std::setw(40) << "Title" << std::setw(7) << "Year" << std::setw(10) << "Rating" <<
                    std::setw(10) << "Weeks on NYT" << "\n";
                outFile << std::string(60, '-') << "\n";  // Print a line separator


                bool found = false;
                for (size_t i = 0; i < mediaLib.size(); ++i) {
                    const auto& media = mediaLib[i];
                    if (media->getType() == 'B') {
                        // Cast to Book to access getWeeks
                        Book* book = static_cast<Book*>(media);

                        outFile << std::left << std::setw(5) << (i + 1)  // Line number (1-based index)
                            << std::setw(40) << media->getTitle()
                            << std::setw(10) << media->getYearReleased()
                            << std::setw(10) << media->getRating()
                            << book->getWeeks()
                            << "\n";
                        found = true;
                    }

                }
                if (!found) {
                    outFile << "No books found.\n";
                }

            }


        }
        // Process "S" command (Filter songs by rating or genre, or list all songs)
        else if (command == "S") {
            std::string token;

            // Extract the filter (rating or genre)
            if (std::getline(commandStream, token, ',')) {
                if (std::all_of(token.begin(), token.end(), ::isdigit)) {
                    // Handle numeric rating filter
                    int filterRating = std::stoi(token);
                    if (filterRating >= 1 && filterRating <= 10) {
                        outFile << "\nSONG LIST WITH RATING >= " << filterRating << ":\n";
                        outFile << std::left << std::setw(5) << "#"
                            << std::setw(40) << "Title"
                            << std::setw(7) << "Year"
                            << std::setw(20) << "Rating"
                            << std::setw(25) << "Genre"
                            << "Top 40\n";
                        outFile << std::string(115, '-') << "\n";

                        bool found = false;
                        for (size_t i = 0; i < mediaLib.size(); ++i) {
                            const auto& media = mediaLib[i];
                            if (media->getType() == 'S' && media->getRating() >= filterRating) {
                                Song* song = dynamic_cast<Song*>(media);  // Cast to Song
                                if (song) {  // Ensure it's a Song
                                    outFile << std::left << std::setw(5) << (i + 1)
                                        << std::setw(40) << media->getTitle()
                                        << std::setw(10) << media->getYearReleased()
                                        << std::setw(15) << media->getRating()
                                        << std::setw(25) << media->getGenre()
                                        << (song->getTop40() ? "Top 40" : "Not Top 40") << "\n";
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
                    // Handle genre-based filter
                    outFile << "\nSONG LIST FOR GENRE: " << token << "\n";
                    outFile << std::left << std::setw(5) << "#"
                        << std::setw(40) << "Title"
                        << std::setw(7) << "Year"
                        << std::setw(20) << "Rating"
                        << std::setw(25) << "Genre"
                        << "Top 40\n";
                    outFile << std::string(80, '-') << "\n";

                    bool found = false;
                    for (size_t i = 0; i < mediaLib.size(); ++i) {
                        const auto& media = mediaLib[i];
                        Song* song = dynamic_cast<Song*>(media);  // Cast to Song
                        if (media->getType() == 'S' && media->getGenre() == token) {
                            if (song) {  // Ensure it's a Song
                                outFile << std::left << std::setw(5) << (i + 1)
                                    << std::setw(40) << media->getTitle()
                                    << std::setw(10) << media->getYearReleased()
                                    << std::setw(15) << media->getRating()
                                    << std::setw(25) << media->getGenre()
                                    << (song->getTop40() ? "Top 40" : "Not Top 40") << "\n";
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
                // Handle case with no filter (list all songs)
                outFile << "\n\nALL SONGS:\n";
                outFile << std::left << std::setw(5) << "#"
                    << std::setw(40) << "Title"
                    << std::setw(7) << "Year"
                    << std::setw(20) << "Rating"
                    << std::setw(15) << "Genre"
                    << "Top 40\n";
                outFile << std::string(80, '-') << "\n";

                bool found = false;
                for (size_t i = 0; i < mediaLib.size(); ++i) {
                    const auto& media = mediaLib[i];
                    if (media->getType() == 'S') {
                        Song* song = dynamic_cast<Song*>(media);  // Cast to Song
                        if (song) {  // Ensure it's a Song
                            outFile << std::left << std::setw(5) << (i + 1)
                                << std::setw(40) << media->getTitle()
                                << std::setw(10) << media->getYearReleased()
                                << std::setw(15) << media->getRating()
                                << std::setw(25) << media->getGenre()
                                << (song->getTop40() ? "Top 40" : "Not Top 40") << "\n";
                            found = true;
                        }
                    }
                }
                if (!found) {
                    outFile << "No songs found.\n";
                }
            }
        }

        // Process "N" command (Add new media)
        else if (command == "N") {
            std::string token;
            std::getline(commandStream, token, ',');

            // Extract data for the new media item
            string typeStr, title, creator, ratingStr, genre, lengthStr, yearStr;
            int rating, length, year;

            getline(commandStream, title, ',');
            getline(commandStream, creator, ',');
            getline(commandStream, ratingStr, ',');
            getline(commandStream, genre, ',');
            getline(commandStream, lengthStr, ',');
            getline(commandStream, yearStr, ',');

            try {
                rating = std::stoi(ratingStr);
                length = std::stoi(lengthStr);
                year = std::stoi(yearStr);
            }
            catch (const std::invalid_argument&) {
                cerr << "Error with: " << title << " " << creator << " " << ratingStr << " " << genre << " "
                    << lengthStr << " " << yearStr << ": Invalid numeric value\n";
                continue;
            }
            catch (const std::out_of_range&) {
                cerr << "Error with: " << title << " " << creator << " " << ratingStr << " " << genre << " "
                    << lengthStr << " " << yearStr << ": Numeric value out of range\n";
                continue;
            }

            // Validate input values
            if (rating <= 0 || rating > 10 || length <= 0 || year < 1000 || year > 2024) {
                cerr << "Error with: " << title << " " << creator << " " << ratingStr << " " << genre << " "
                    << lengthStr << " " << yearStr << ": Invalid value in rating, length, or year\n";
            }

            std::vector<std::string> stars;  // Declare stars vector
            int weeksNYT = 0;  // Initialize weeksNYT
            int top40 = 0;     // Initialize top40

            addNewMedia(outFile, token, title, creator, rating, genre, length, year, mediaLib, stars, weeksNYT, top40);
        }

        // Process "T" command (Print totals for media types)
        else if (command == "T") {
            outFile << "\n\nYOUR MEDIA LIBRARY\n";
            printTotals(outFile, mediaLib);  // Print total counts for each type
        
        }

    }

    commandFile.close();
}

/*
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

    // Read the first line
    getline(stream, token, ',');  // Skip N
    getline(stream, token, ',');   // string1 (type)
    type = token[0];               // Convert string1 to char
    getline(stream, title, ',');   // string2 (title)
    getline(stream, name, ',');    // string3 (name)
    stream >> rating;              // int1 (rating)
    stream.ignore();               // Ignore the comma
    getline(stream, genre, ',');   // string4 (genre)
    stream >> length;              // int2 (length)
    stream.ignore();               // Ignore the comma
    stream >> year;                // int3 (year)

    // Check for any errors in the input data
    if (stream.fail()) {
        outErr << "Error processing record: " << data << endl; // Log error to the error file
        return;  // Skip this record
    }

    // Create the media object and add it to the vector
    myLib.push_back(new Media(type, title, name, rating, genre, length, year));
}
*/
/*
// Function to read media data from a file
void readMediaList(ifstream& inList, ofstream& outErr, vector<Media*>& myLib) {
    string line;

    while (getline(inList, line)) {
        processMediaData(line, myLib, outErr);
    }
}
*/