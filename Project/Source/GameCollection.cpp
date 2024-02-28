#include "GameCollection.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <iostream>
//blank default constructor
GameCollection::GameCollection() {
    collectionFilePath = "gamescollection.txt";
    loadCollection();
}

GameCollection::GameCollection(const std::string& filePath) : collectionFilePath(filePath) {
    loadCollection();
}

void GameCollection::loadCollection() {
    std::ifstream file(collectionFilePath);
    std::string line;
    while (getline(file, line)) {
        // Check for the "END" line indicating the end of the file
        if (line.substr(0, 3) == "END") break;

        // Dynamically find the separator between game name and owner username
        size_t separatorIndex = line.find_first_of(" ", 25); // Find the first space after the 25th character
        if (separatorIndex == std::string::npos) continue; // Skip if no space found (malformed line)

        // Extract the game name, considering the dynamic position of the first separator
        std::string gameName = line.substr(0, separatorIndex);
        // Ensure we don't start extracting the username before the actual start
        size_t usernameStart = separatorIndex + 1; // Move past the separator space
        // Extract the owner's username, starting after the separator
        std::string ownerUsername = line.substr(usernameStart, std::string::npos); // Extract till the end, trimming later

        // Trim trailing spaces from both the game name and owner's username
        gameName.erase(gameName.find_last_not_of(" ") + 1);
        ownerUsername.erase(ownerUsername.find_last_not_of(" ") + 1);
        // Add the game to the collection under the owner's username
        userGameCollections[ownerUsername].push_back(gameName);
    }
    file.close();
}

void GameCollection::saveCollection() {
    std::ofstream file(collectionFilePath, std::ofstream::trunc); // Open the file for writing, truncating existing content

    for (const auto& pair : userGameCollections) {
        for (const auto& gameName : pair.second) {
            file << std::left << std::setw(25) << gameName // Game name, left-justified, padded with spaces to 25 characters
                 << " " // Space separator
                 << std::setw(15) << std::setfill(' ') << pair.first // Owner's username, left-justified, padded to 15 characters
                 << std::endl; // End of line
        }
    }

    // Adjust the "END" line to correctly fill the line up to 42 characters, including the "END" text
    file << "END" << std::setw(38) << std::setfill(' ') << " "; // "END" plus 38 spaces to reach 42 characters in total

    file.close(); // Close the file after writing
}

std::string trim(const std::string& str) {
    auto start = str.find_first_not_of(" ");
    if (start == std::string::npos) return ""; // String contains only spaces

    auto end = str.find_last_not_of(" ");
    return str.substr(start, (end - start + 1));
}

// Helper function for case-insensitive string comparison
bool caseInsensitiveCompare(const std::string& str1, const std::string& str2) {
    if (str1.size() != str2.size()) {
        return false;
    }
    for (size_t i = 0; i < str1.size(); ++i) {
        if (tolower(str1[i]) != tolower(str2[i])) {
            return false;
        }
    }
    return true;
}

bool GameCollection::ownsGame(const std::string& username, const std::string& gameName) const {
    auto it = userGameCollections.find(trim(username));
    if (it != userGameCollections.end()) {
        auto& games = it->second;
        return std::any_of(games.begin(), games.end(), [&](const std::string& ownedGame) {
            return caseInsensitiveCompare(trim(ownedGame), trim(gameName));
        });
    }
    return false;
}

void GameCollection::addGameToUser(const std::string& username, const std::string& gameName) {
    // print the game collection
    if (!ownsGame(username, gameName)) {
        userGameCollections[username].push_back(gameName);
        saveCollection(); // Save after modification
    }
}
