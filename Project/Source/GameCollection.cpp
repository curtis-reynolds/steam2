#include "GameCollection.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
//blank default constructor
GameCollection::GameCollection() {
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

        // The game name occupies the first 25 characters of the line
        std::string gameName = line.substr(0, 25);
        // The owner's username is right after the game name, occupying the next 15 characters
        std::string ownerUsername = line.substr(26, 15); // Start from position 26, accounting for the space separator

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

bool GameCollection::ownsGame(const std::string& username, const std::string& gameName) const {
    auto it = userGameCollections.find(username);
    if (it != userGameCollections.end()) {
        return std::find(it->second.begin(), it->second.end(), gameName) != it->second.end();
    }
    return false;
}

void GameCollection::addGameToUser(const std::string& username, const std::string& gameName) {
    if (!ownsGame(username, gameName)) {
        userGameCollections[username].push_back(gameName);
        saveCollection(); // Save after modification
    }
}
