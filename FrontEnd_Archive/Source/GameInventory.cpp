#include "GameInventory.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>

// Default constructor that initializes the inventory file path to an empty string.
// This can be used when the inventory file path will be set later or loaded from a default location.
GameInventory::GameInventory() {
    inventoryFilePath = "availablegames.txt";  
}

// Constructor that takes a file path as an argument and loads the game inventory from this file.
GameInventory::GameInventory(const std::string& inventoryFile)
: inventoryFilePath(inventoryFile) {
    loadInventory();
}

// Loads the game inventory from the specified file.
// Opens the file, reads each line, and parses it to create Game objects which are added to the inventory.
void GameInventory::loadInventory() {
    std::ifstream file(inventoryFilePath);
    if (!file) {
        std::cerr << "Error: Unable to open inventory file." << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        if (line.substr(0, 3) == "END") break; // Stop if "END" line is found

        // Extract game name, seller's username, and price based on fixed positions
        std::string gameName = line.substr(0, 25);
        std::string sellerUsername = line.substr(26, 16);
        std::string priceStr = line.substr(43, 6);

        // Trim the extracted values
        gameName.erase(gameName.find_last_not_of(' ') + 1);
        sellerUsername.erase(sellerUsername.find_last_not_of(' ') + 1);

        // Convert price string to float
        float price = std::stof(priceStr);

        // Create and add the game to the inventory
        Game game(gameName, price, sellerUsername);
        inventory.push_back(game);
    }

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
        std::cout << "Error: Game already exists in inventory." << std::endl;
        return;
    }
    // Checks if the game name exceeds the maximum allowed length.
    if (gameName.length() > 25) {
        std::cout << "Error: Game name exceeds maximum length of 25 characters." << std::endl;
        return;
    }
    // check if game name is blank
    if (gameName.length() == 0) {
        std::cout << "Error: Game name cannot be blank." << std::endl;
        return;
    }
    // if game name is just spaces
    if (gameName.find_first_not_of(' ') == std::string::npos) {
        std::cout << "Error: Game name cannot be just spaces." << std::endl;
        return;
    }
    // if game name has special characters
    if (gameName.find_first_of("!@#$%^&*()_+{}|:<>?") != std::string::npos) {
        std::cout << "Error: Game name cannot contain special characters." << std::endl;
        return;
    }
    // Checks if the price exceeds the maximum allowed value.
    if (price > 999.99) {
        std::cout << "Error: Price exceeds maximum allowed value." << std::endl;
        return;
    }
    // Checks if the price is less than or equal to zero.
    if (price <= 0) {
        std::cout << "Error: Price must be greater than zero." << std::endl;
        return;
    }
    // check if the price is just blank
    if (std::to_string(price).length() == 0) {
        std::cout << "Error: Price cannot be blank." << std::endl;
        return;
    }

    // If all checks pass, creates a new Game object and adds it to the inventory vector.
    Game newGame(gameName, price, sellerUsername);
    inventory.push_back(newGame);

    // print success message
    std::cout << "Game '" << gameName << "' is now up for sale." << std::endl;

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

    for (const auto& game : inventory) {
        // Format and write each game's details to the file
        file << std::left << std::setw(25) << std::setfill(' ') << game.gameName; // Game name padded with spaces
        file << " "; // Space separator
        file << std::setw(16) << std::setfill(' ') << game.sellerUsername; // Seller's username padded with spaces
        file << " "; // Space separator
        file << std::right << std::setw(6) << std::setfill('0') 
             << std::fixed << std::setprecision(2) << game.price; // Price formatted with leading zeros
        file << std::endl;
    }

    // Write the "END" line with appropriate spacing
    file << "END" << std::setw(24) << std::setfill(' ') << " " // Pad the rest of the game name field with spaces
         << std::setw(16) << " " // Pad the username field with spaces
         << "000000" << std::endl; // Price field filled with zeros

    file.close();
}

bool GameInventory::gameExists(const std::string& gameName) const {
    // Use std::find_if with a lambda function to search for the game by name
    auto it = std::find_if(inventory.begin(), inventory.end(), 
                           [&gameName](const Game& game) {
                               return game.gameName == gameName;
                           });
    return it != inventory.end(); // Return true if the game is found, false otherwise
}

void GameInventory::removeGamesByUsername(const std::string& username) {
    auto newEnd = std::remove_if(inventory.begin(), inventory.end(),
                                 [&username](const Game& game) {
                                     return game.sellerUsername == username;
                                 });
    inventory.erase(newEnd, inventory.end());
    saveInventory(); // Save changes after removing games
}

float GameInventory::getGamePrice(const std::string& gameName, const std::string& sellerUsername) const {
    auto it = std::find_if(inventory.begin(), inventory.end(), 
                           [&gameName, &sellerUsername](const Game& game) {
                               return game.gameName == gameName && game.sellerUsername == sellerUsername;
                           });
    if (it != inventory.end()) {
        return it->price;
    } else {
        throw std::runtime_error("Game not found or seller username does not match.");
    }
}