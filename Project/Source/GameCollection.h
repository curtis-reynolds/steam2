#ifndef GAMECOLLECTION_H
#define GAMECOLLECTION_H

#include <string>
#include <unordered_map>
#include <vector>

class GameCollection {
private:
    std::unordered_map<std::string, std::vector<std::string>> userGameCollections; // Maps usernames to lists of owned games
    std::string collectionFilePath; // Path to the game collection file

    void loadCollection(); // Load game collections from a file
    void saveCollection(); // Save game collections to a file

public:
    GameCollection();
    GameCollection(const std::string& filePath);
    bool ownsGame(const std::string& username, const std::string& gameName) const; // Check if a user owns a specific game
    void addGameToUser(const std::string& username, const std::string& gameName); // Add a game to a user's collection
    // Other necessary member functions...
};

#endif // GAMECOLLECTION_H
