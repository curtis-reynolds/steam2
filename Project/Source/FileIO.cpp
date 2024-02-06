#include "FileIO.h"
#include <fstream>
#include <iostream>
#include <sstream>

FileIO::FileIO() {}

void FileIO::readUserAccounts(const std::string& filePath, std::vector<UserAccount>& accounts) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        UserAccount account;
        if (parseUserAccountLine(iss, account)) {
            accounts.push_back(account);
        }
    }

    file.close();
}

void FileIO::readGameInventory(const std::string& filePath, std::vector<Game>& games) {
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

bool FileIO::parseUserAccountLine(std::istringstream& iss, UserAccount& account) {
    // Implement parsing logic based on your file format
    // Example: "username,type,credit"
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

bool FileIO::parseGameLine(std::istringstream& iss, Game& game) {
    // Implement parsing logic based on your file format
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

void FileIO::writeTransactionLog(const std::string& filePath, const std::vector<std::string>& transactions) {
    std::ofstream file(filePath, std::ofstream::app); // Append mode
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return;
    }

    for (const auto& transaction : transactions) {
        file << transaction << std::endl;
    }

    file.close();
}
