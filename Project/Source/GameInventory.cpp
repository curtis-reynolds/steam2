#include "GameInventory.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

GameInventory::GameInventory() : inventoryFilePath("") {
    // Optionally, load inventory based on a default file path
}

GameInventory::GameInventory(const std::string& inventoryFile)
: inventoryFilePath(inventoryFile) {
    loadInventory();
}

void GameInventory::loadInventory() {
    std::ifstream file(inventoryFilePath);
    if (!file) {
        std::cerr << "Error: Unable to open inventory file." << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        // Parse the line to create a Game object
        // Assume the line format is "gameName,price,sellerUsername"
        std::istringstream iss(line);
        std::string gameName, sellerUsername;
        float price;
        char delim;

        iss >> gameName >> delim >> price >> delim >> sellerUsername;
        Game game(gameName, price, sellerUsername);
        inventory.push_back(game);
    }
    file.close();
}

void GameInventory::addGame(const std::string& gameName, float price, const std::string& sellerUsername) {
    if (std::any_of(inventory.begin(), inventory.end(), [&](const Game& game) {
        return game.gameName == gameName;
    })) {
        std::cerr << "Error: Game already exists in inventory." << std::endl;
        return;
    }

    if (gameName.length() > 25) {
        std::cerr << "Error: Game name exceeds maximum length of 25 characters." << std::endl;
        return;
    }

    if (price > 999.99) {
        std::cerr << "Error: Price exceeds maximum allowed value." << std::endl;
        return;
    }

    Game newGame(gameName, price, sellerUsername);
    inventory.push_back(newGame);

    // Write changes to the inventory file
    saveInventory();
}

void GameInventory::saveInventory() {
    std::ofstream file(inventoryFilePath);
    if (!file) {
        std::cerr << "Error: Unable to open inventory file for writing." << std::endl;
        return;
    }

    for (const auto& game : inventory) {
        file << game.gameName << ","
             << game.price << ","
             << game.sellerUsername << std::endl;
    }
    file.close();
}

// ... Additional methods to handle buying a game, searching for a game, etc.

