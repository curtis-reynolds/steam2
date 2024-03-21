#ifndef GAMEUPDATER_H
#define GAMEUPDATER_H

#include "general.h"
#include "FileWriter.h"
#include "Game.h"
#include <errno.h>
#include <string.h>

class GameUpdater : public FileWriter
{
public:
    // Constructor that takes a filename as a parameter
    GameUpdater(const std::string &filename) : FileWriter(filename){};

    // Function to add a game to the available games file
    void addGameToAvailable(const Game &game)
    {
        removeEndLine(49);          // Remove the existing END line
        writeGameToAvailable(game); // Write the user information to the file
        writeEndLine(49);           // Add the END line back to the file
    }

    // Function to add a game to games collection file
    void addGameToCollection(const Game &game, std::string &username)
    {
        removeEndLine(42);                     // Remove the existing END line
        writeGameToCollection(game, username); // Write the user information to the file
        writeEndLine(42);                      // Add the END line back to the file
    }

    // Function to add a game to games collection file
    void deleteUserAvailableUserGames(std::string &username)
    {
        removeGamesFromAvailableGames(username); // Write the user information to the file
    }

    // Function to add a game to games collection file
    void deleteUserGamesFromCollection(std::string &username)
    {
        removeGamesFromCollection(username); // Write the user information to the file
    }

private:
    // Function to write game information to the file
    void writeGameToAvailable(const Game &game)
    {
        if (fileStream.is_open())
        {
            std::string gameName = game.getGameName();
            gameName.resize(26, '_'); // Ensure the game name is 26 characters long
            std::string sellerName = game.getSellerName();
            sellerName.resize(15, '_'); // Ensure the seller name is 16 characters long

            // Format the credit with leading zeros and ".00" suffix
            std::ostringstream formattedPriceStream;
            formattedPriceStream << std::fixed << std::setw(6) << std::setfill('0') << std::setprecision(2) << game.getPrice();
            std::string formattedPrice = formattedPriceStream.str();

            // Write the user information to the file
            fileStream << std::setw(26) << std::left << gameName << "_"
                       << std::setw(15) << std::left << sellerName << "_"
                       << formattedPrice << "\n";
        }
        else
        {
            std::cerr << "Error: Unable to write to the file. Make sure it is open." << std::endl;
        }
    }

    // Function to write game information to the file
    void writeGameToCollection(const Game &game, std::string &username)
    {
        if (fileStream.is_open())
        {
            std::string gameName = game.getGameName();
            gameName.resize(26, '_'); // Ensure the game name is 26 characters long
            username.resize(15, '_'); // Ensure the seller name is 15 characters long

            // Write the user information to the file
            fileStream << std::setw(26) << std::left << gameName << "_"
                       << std::setw(15) << std::left << username << "\n";
        }
        else
        {
            std::cerr << "Error: Unable to write to the file. Make sure it is open." << std::endl;
        }
    }

    void removeGamesFromCollection(const std::string &username)
    {
        if (fileStream.is_open())
        {
            fileStream.close();
            std::cerr << "Collections" << std::endl;
            // Create a temporary file to store all lines except the games associated with the user
            std::ifstream inFile(filename); // Change to ifstream for reading
            if (!inFile.is_open())
            {
                std::cerr << "Error: Unable to open the file for reading." << std::endl;
                return;
            }

            std::ofstream tempFile("temp.txt");
            if (!tempFile.is_open())
            {
                std::cerr << "Error: Unable to create temporary file." << std::endl;
                return;
            }

            std::string line;

            // Read and store all lines except the games associated with the user
            while (std::getline(inFile, line))
            {
                // Extract username from the line
                std::string gameLineUsername = line.substr(26, 15);
                // Remove underscores from username
                gameLineUsername.erase(std::remove(gameLineUsername.begin(), gameLineUsername.end(), '_'), gameLineUsername.end());

                // If the username does not match the one to be removed, write the line to the temporary file
                if (gameLineUsername != username)
                {
                    tempFile << line << "\n";
                }
            }

            // Close the input file
            inFile.close();
            tempFile.close();

            // Remove the original file
            std::remove(filename.c_str());

            // Rename the temporary file to the original file
            std::rename("temp.txt", filename.c_str());

            // Reopen the file in append mode
            fileStream.open(filename, std::ios::app);
        }
        else
        {
            std::cerr << "Error: Unable to write to the file. Make sure it is open." << std::endl;
        }
    }

    void removeGamesFromAvailableGames(const std::string &username)
    {
        if (fileStream.is_open())
        {
            // Close the file
            fileStream.close();

            // Create a temporary file to store all lines except the games associated with the user
            std::ifstream inFile(filename); // Change to ifstream for reading
            if (!inFile.is_open())
            {
                std::cerr << "Error: Unable to open the file for reading." << std::endl;
                return;
            }

            std::ofstream tempFile("temp.txt");
            if (!tempFile.is_open())
            {
                std::cerr << "Error: Unable to create temporary file." << std::endl;
                return;
            }

            std::string line;

            // Read and store all lines except the games associated with the user
            while (std::getline(inFile, line))
            {
                // Extract seller username from the line
                std::string gameLineSellerUsername = line.substr(26, 15);
                // Remove underscores from seller username
                gameLineSellerUsername.erase(std::remove(gameLineSellerUsername.begin(), gameLineSellerUsername.end(), '_'), gameLineSellerUsername.end());

                // If the seller username does not match the one to be removed, write the line to the temporary file
                if (gameLineSellerUsername != username)
                {
                    tempFile << line << "\n";
                }
            }

            // Close the input file
            inFile.close();
            tempFile.close();

            // // Remove the original file
            std::remove(filename.c_str());

            // // Rename the temporary file to the original file
            std::rename("temp.txt", filename.c_str());
        }
        else
        {
            std::cerr << "Error: Unable to write to the file. Make sure it is open." << std::endl;
        }
    }
};

#endif
