#include "GameInventory.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

// Default constructor that initializes the inventory file path to an empty string.
// This can be used when the inventory file path will be set later or loaded from a default location.
GameInventory::GameInventory() : inventoryFilePath("") {
    // Optionally, load inventory based on a default file path
}

// Constructor that takes a file path as an argument and loads the game inventory from this file.
GameInventory::GameInventory(const std::string& inventoryFile)
: inventoryFilePath(inventoryFile) {
    loadInventory();
}

// Loads the game inventory from the specified file.
// Opens the file, reads each line, and parses it to create Game objects which are added to the inventory.
void GameInventory::loadInventory() {
    // Attempts to open the file at the path stored in inventoryFilePath.
    // If the file cannot be opened, an error message is displayed and the function returns early.
    std::ifstream file(inventoryFilePath);
    if (!file) {
        std::cerr << "Error: Unable to open inventory file." << std::endl;
        return;
    }

    // Reads the file line by line. Each line is expected to represent a game with its details
    // such as name, price, and seller's username separated by delimiters.
    // Parses each line to extract these details and create a Game object,
    // which is then added to the inventory vector.
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
    // Closes the file after finishing reading.
    file.close();
}

// Adds a new game to the inventory.
// Checks for duplicates, validates the game name length and price before adding the game to the inventory.
// If any check fails, an error message is displayed and the function returns early.
void GameInventory::addGame(const std::string& gameName, float price, const std::string& sellerUsername) {
    // Checks if a game with the same name already exists in the inventory.
    if (std::any_of(inventory.begin(), inventory.end(), [&](const Game& game) {
        return game.gameName == gameName;
    })) {
        std::cerr << "Error: Game already exists in inventory." << std::endl;
        return;
    }
    // Checks if the game name exceeds the maximum allowed length.
    if (gameName.length() > 25) {
        std::cerr << "Error: Game name exceeds maximum length of 25 characters." << std::endl;
        return;
    }
    // Checks if the price exceeds the maximum allowed value.
    if (price > 999.99) {
        std::cerr << "Error: Price exceeds maximum allowed value." << std::endl;
        return;
    }

    // If all checks pass, creates a new Game object and adds it to the inventory vector.
    Game newGame(gameName, price, sellerUsername);
    inventory.push_back(newGame);

    // Then, calls saveInventory to write the updated inventory back to the file.
    saveInventory();
}

// Saves the current game inventory to the file.
// Opens the file for writing, overwriting any existing content.
// If the file cannot be opened, an error message is displayed and the function returns early.
// Writes each game's details to the file, one game per line, with details separated by delimiters.
void GameInventory::saveInventory() {
    std::ofstream file(inventoryFilePath);
    if (!file) {
        std::cerr << "Error: Unable to open inventory file for writing." << std::endl;
        return;
    }
     // Iterates over the inventory vector, writing each game's details to the file.
    for (const auto& game : inventory) {
        file << game.gameName << ","
             << game.price << ","
             << game.sellerUsername << std::endl;
    }
    // Closes the file after finishing writing.
    file.close();
}

// ... Additional methods to handle buying a game, searching for a game, etc.

