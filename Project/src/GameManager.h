#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "SharedData.h"
#include "Game.h"
#include "GameUpdater.h"
#include "CreditUpdater.h"
#include <iostream>

class GameManager
{
public:
    // Add a constructor that accepts SharedData and filename
    GameManager(SharedData &sharedData, const std::string &currentAccountsFilename, const std::string &availableGamesFilename, const std::string &gamesCollectionFilename)
        : sharedData(sharedData),
          gamesCollectionFilename(gamesCollectionFilename),
          availableGamesFileReader(availableGamesFilename),
          gamesCollectionFileReader(gamesCollectionFilename),
          existingGames(sharedData.getAvailableGames()),
          availableGameUpdater(availableGamesFilename),
          gamesCollectionUpdater(gamesCollectionFilename),
          creditUpdater(sharedData, currentAccountsFilename)
    {
        // Attempt to open the file for reading
        if (availableGamesFileReader.openFile())
        {
            // Read available games data from the file
            availableGamesFileReader.readAvailableGames(existingGames);

            availableGamesFileReader.closeFile();
        }
        else
        {
            std::cerr << "Error: Unable to open the file for reading." << std::endl;
        }

        // Attempt to open the file for reading
        if (gamesCollectionFileReader.openFile())
        {
            // Read games collection data and assign to users in SharedData
            gamesCollectionFileReader.readGamesCollection(sharedData.getUsers());

            gamesCollectionFileReader.closeFile();
        }
        else
        {
            std::cerr << "Error: Unable to open the file for reading." << std::endl;
        }
    }

    Game sellGame()
    {
        // Check if a new game for sale can be added in this session
        if (isGameForSaleAdded)
        {
            std::cout << "A new game for sale has already been added in this session. "
                         "You can only add one game per session."
                      << std::endl;
            return Game("", "", 0.0);
        }

        std::string gameName;
        double price;

        // Get the game name from the user
        std::cout << "Enter the game name (up to 25 characters): ";
        std::cin.ignore(); // Ignore the newline character from the previous input
        std::getline(std::cin, gameName);

        // Validate the length of the game name
        if (gameName.length() > 25)
        {
            std::cout << "Invalid game name length. Maximum length is 25 characters." << std::endl;
            return Game("", "", 0.0);
        }

        // Validate the uniqueness of the game name
        if (isGameNameAlreadyExists(gameName))
        {
            std::cout << "A game with the same name already exists. Please choose a unique name." << std::endl;
            return Game("", "", 0.0);
        }

        // Get the price from the user
        std::cout << "Enter the price for the game (up to $999.99): ";
        std::cin >> price;

        // Validate the price range
        if (price < 0.01 || price > 999.99)
        {
            std::cout << "Invalid price. The price must be between $0.01 and $999.99." << std::endl;
            return Game("", "", 0.0);
        }

        std::string sellerUsername = sharedData.getCurrentUser().getUsername();
        // Create a new Game object
        Game newGame(gameName, sellerUsername, price);

        // Open the file and write the new user information
        if (availableGameUpdater.openFile())
        {
            availableGameUpdater.addGameToAvailable(newGame);
            availableGameUpdater.closeFile();
            std::cout << "Game listed for sale." << std::endl;

            // Set the flag to indicate that a new game for sale has been added in this session
            isGameForSaleAdded = true;

            return newGame;
        }
        else
        {
            std::cerr << "Error: Unable to open the file for writing." << std::endl;
            return Game("", "", 0.0);
        }
    }

    Game buyGame()
    {
        // Check if a game for purchase has already been bought in this session
        if (isGameBought)
        {
            std::cout << "A game has already been purchased in this session. "
                         "You can only buy one game per session."
                      << std::endl;
            return Game("", "", 0.0);
        }

        std::string gameName;
        std::string sellerUsername;

        // Get the game name from the user
        std::cout << "Enter the game name you want to buy: ";
        std::cin.ignore(); // Ignore the newline character from the previous input
        std::getline(std::cin, gameName);

        // Get the seller's username from the user
        std::cout << "Enter the seller's username: ";
        std::getline(std::cin, sellerUsername);

        // Find the game in the available games
        auto gameIterator = std::find_if(existingGames.begin(), existingGames.end(),
                                         [gameName, sellerUsername](const Game &game)
                                         {
                                             return game.getGameName() == gameName && game.getSellerName() == sellerUsername;
                                         });

        // Check if the game was found
        if (gameIterator == existingGames.end())
        {
            std::cout << "Error: Game not found in the available games. " << std::endl;
            return Game("", "", 0.0);
        }

        // Get the buyer and seller users
        User &buyer = sharedData.getCurrentUser();
        User *seller = sharedData.getUserByUsername(sellerUsername);

        if (seller == nullptr)
        {
            std::cout << "Error: Seller doesn't exist." << std::endl;
            return Game("", "", 0.0);
        }

        if (buyer.getUsername() == seller->getUsername())
        {
            std::cout << "Error: You are the seller of the game." << std::endl;
            return Game("", "", 0.0);
        }

        // Check buyer's type
        if (buyer.getType() == UserType::SellStandard)
        {
            std::cout << "Error: Standard-sell users cannot perform this transaction." << std::endl;
            return Game("", "", 0.0);
        }

        // Check if buyer has enough money
        double gamePrice = gameIterator->getPrice();
        if (buyer.getCredit() < gamePrice)
        {
            std::cout << "Error: The buyer does not have enough money to purchase the game." << std::endl;
            return Game("", "", 0.0);
        }

        // Check if buyer already has a copy of the game
        if (buyer.hasGameInCollection(gameName))
        {
            std::cout << "Error: The buyer already has a copy of the game in their collection." << std::endl;
            return Game("", "", 0.0);
        }

        // Deduct amount from buyer's credit and add it to seller's credit
        double buyerNewCredit = buyer.getCredit() - gamePrice;
        double sellerNewCredit = seller->getCredit() + gamePrice;

        // Update credits
        creditUpdater.updateCreditForUser(&buyer, buyerNewCredit);
        creditUpdater.updateCreditForUser(seller, sellerNewCredit);

        // Add the game to the buyer's collection
        buyer.addGameToCollection(gameIterator->getGameName());

        std::string buyerUsername = buyer.getUsername();

        if (gamesCollectionUpdater.openFile())
        {
            gamesCollectionUpdater.addGameToCollection(*gameIterator, buyerUsername);
            gamesCollectionUpdater.closeFile();

            isGameBought = true;

            std::cout << "Game purchased successfully." << std::endl;
        }
        else
        {
            std::cerr << "Error: Unable to open the file for writing." << std::endl;
        }

        return *gameIterator;
    }

    void removeUserGames(std::string &username)
    {
        if (availableGameUpdater.openFile())
        {
            availableGameUpdater.deleteUserAvailableUserGames(username);
            availableGameUpdater.closeFile();
        }
        else
        {
            std::cerr << "Error: Unable to open the file for writing." << std::endl;
        }

        if (gamesCollectionUpdater.openFile())
        {
            gamesCollectionUpdater.deleteUserGamesFromCollection(username);
            gamesCollectionUpdater.closeFile();
        }
        else
        {
            std::cerr << "Error: Unable to open the file for writing." << std::endl;
        }

        std::cout << "User's games deleted successfully." << std::endl;
    }

    void listAvailableGames()
    {
        // Check if existingGames is empty
        if (existingGames.empty())
        {
            std::cout << "Store is empty." << std::endl;
            return;
        }

        // Display header with column names
        std::cout << "Available Games:" << std::endl;
        std::cout << std::setw(30) << std::left << "Game Name"
                  << std::setw(20) << std::left << "Seller"
                  << std::setw(10) << std::left << "Price" << std::endl;

        // Iterate through each available game
        for (const Game &game : existingGames)
        {
            // Display game information
            std::cout << std::setw(30) << std::left << game.getGameName()
                      << std::setw(20) << std::left << game.getSellerName()
                      << std::setw(10) << std::fixed << std::setprecision(2) << game.getPrice() << std::endl;

            // Add a divider line between each game
            std::cout << std::string(60, '-') << std::endl;
        }
    }

private:
    SharedData &sharedData;
    // GameUpdater and FileReader objects to handle file operations
    FileReader availableGamesFileReader;
    FileReader gamesCollectionFileReader;
    GameUpdater availableGameUpdater;
    GameUpdater gamesCollectionUpdater;
    // CreditUpdater instance for handling credit-related operations
    CreditUpdater creditUpdater;

    // Vector to store existing games
    std::vector<Game> existingGames;

    // Filename for games collection data
    const std::string gamesCollectionFilename;

    // Variable to track whether a new game for sale has been added in this session
    bool isGameForSaleAdded = false;
    // Variable to track whether a game has been bought in this session
    bool isGameBought = false;

    // Helper function to check if a game with the given name already exists
    bool isGameNameAlreadyExists(const std::string &gameName)
    {
        for (const Game &existingGame : existingGames)
        {
            if (existingGame.getGameName() == gameName)
            {
                return true;
            }
        }
        return false;
    }
};

#endif
