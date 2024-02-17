#include "FileIO.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Default constructor: Initializes a FileIO object.
FileIO::FileIO() {}

// Reads user account data from a specified file and stores it in a vector of UserAccount structures.
void FileIO::readUserAccounts(const std::string& filePath, std::vector<UserAccount>& accounts) {
    // Attempts to open the specified file.
    std::ifstream file(filePath);
    // Checks if the file is successfully opened.
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return; // Exits the function if the file cannot be opened.
    }

    // Reads the file line by line.
    std::string line;
    while (std::getline(file, line)) {
        // Parses each line to extract user account data.
        std::istringstream iss(line);
        UserAccount account;
        // If the line is successfully parsed, adds the user account to the vector.
        if (parseUserAccountLine(iss, account)) {
            accounts.push_back(account);
        }
    }

    // Closes the file after reading.
    file.close();
}

// Reads game inventory data from a specified file and stores it in a vector of Game structures.
void FileIO::readGameInventory(const std::string& filePath, std::vector<Game>& games) {
    // Similar structure to readUserAccounts, adapted for game inventory data.
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Game game;
        if (parseGameLine(iss, game)) {
            games.push_back(game);
        }
    }

    file.close();
}

// Parses a line from the user accounts file and fills a UserAccount structure with the data.
bool FileIO::parseUserAccountLine(std::istringstream& iss, UserAccount& account) {
    // Parses a line according to the expected format: "username,type,credit".
    // Returns true if the line is successfully parsed; otherwise, returns false.
    std::string username, type;
    float credit;
    if (iss >> username >> type >> credit) {
        account.username = username;
        account.type = type;
        account.credit = credit;
        return true;
    }
    return false;
}

// Parses a line from the game inventory file and fills a Game structure with the data.
bool FileIO::parseGameLine(std::istringstream& iss, Game& game) {
    // Similar logic to parseUserAccountLine, adapted for game inventory data.
    // Example: "gameName,price,sellerUsername"
    std::string gameName, sellerUsername;
    float price;
    if (iss >> gameName >> price >> sellerUsername) {
        game.gameName = gameName;
        game.price = price;
        game.sellerUsername = sellerUsername;
        return true;
    }
    return false;
}

// Writes transaction log data to a specified file. The transactions are appended to the file.
void FileIO::writeTransactionLog(const std::string& filePath, const std::vector<std::string>& transactions) {
    // Opens the specified file in append mode.
    std::ofstream file(filePath, std::ofstream::app);
    // Checks if the file is successfully opened.
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return; // Exits the function if the file cannot be opened.
    }

    // Writes each transaction in the vector to the file, one per line.
    for (const auto& transaction : transactions) {
        file << transaction << std::endl;
    }

    // Closes the file after writing.
    file.close();
}