#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "User.h"
#include "Game.h"
#include "general.h"

class FileReader
{
public:
    // Constructor that takes a filename as a parameter
    FileReader(const std::string &filename) : filename(filename) {}

    // Function to open the file and return whether the operation was successful
    bool openFile()
    {
        // Attempt to open the file
        fileStream.open(filename, std::ios::binary);
        return fileStream.is_open();
    }

    // Function to read user data from the file and populate the given vector of users
    void readUsers(std::vector<User> &users)
    {
        if (fileStream.is_open())
        {
            std::string line;
            std::string endLine = "END";
            endLine.resize(28, '_');
            while (std::getline(fileStream, line))
            {
                // Check for the END line to stop reading
                if (line == endLine)
                    break;

                // Ensure the line has the correct length
                if (line.length() == 28)
                {
                    // Extract username, userType, and credit information from the line
                    std::string username = line.substr(0, 16);
                    // Remove underscores from username
                    username.erase(std::remove(username.begin(), username.end(), '_'), username.end());

                    std::string userTypeCode = line.substr(16, 2);
                    int userType = getUserTypeFromCode(userTypeCode);

                    double credit = std::stod(line.substr(19, 9));

                    // Create a User object and add it to the vector
                    User user(username, userType, credit);
                    users.push_back(user);
                }
                else
                {
                    // Notify about an error if the line length is invalid and skip the line
                    std::cerr << "Error: Invalid line length. Skipping line." << line.length() << std::endl;
                    std::cerr << "Debug: Line contents - " << line << std::endl;
                }
            }
        }
        else
        {
            // Notify about an error if the file is not open
            std::cerr << "Error: Unable to read the file. Make sure it is open." << std::endl;
        }
    }

    // Function to read available games data from the file and populate the given vector of games
    void readAvailableGames(std::vector<Game> &games)
    {
        if (fileStream.is_open())
        {
            std::string line;
            std::string endLine = "END";
            endLine.resize(49, '_');
            while (std::getline(fileStream, line))
            {
                // Check for the END line to stop reading
                if (line == endLine)
                    break;

                // Ensure the line has the correct length
                if (line.length() == 49)
                {
                    // Extract game name, seller's username, and price information from the line
                    std::string gameName = line.substr(0, 25);
                    // Remove spaces from game name
                    gameName.erase(std::remove(gameName.begin(), gameName.end(), '_'), gameName.end());

                    std::string sellerUsername = line.substr(25, 16);
                    // Remove spaces from seller's username
                    sellerUsername.erase(std::remove(sellerUsername.begin(), sellerUsername.end(), '_'), sellerUsername.end());

                    double price = std::stod(line.substr(43, 6));

                    // Create a Game object and add it to the vector
                    Game game(gameName, sellerUsername, price);
                    games.push_back(game);
                }
                else
                {
                    // Notify about an error if the line length is invalid and skip the line
                    std::cerr << "Error: Invalid line length. Skipping line." << std::endl;
                    std::cerr << "Debug: Line contents - " << line << std::endl;
                }
            }
        }
        else
        {
            // Notify about an error if the file is not open
            std::cerr << "Error: Unable to read the file. Make sure it is open." << std::endl;
        }
    }

    // Function to read games collection data from the file and assign to users in SharedData
    void readGamesCollection(std::vector<User> &users)
    {
        if (fileStream.is_open())
        {
            // Read and assign games collection data to users
            std::string line;
            std::string endLine = "END";
            endLine.resize(42, '_');
            while (std::getline(fileStream, line))
            {
                // Check for the END line to stop reading
                if (line == endLine)
                    break;

                // Validate line length
                if (line.length() == 42)
                {
                    std::string gameName = line.substr(0, 25);
                    std::string ownerUsername = line.substr(25, 16);

                    // Remove spaces from game name and owner name
                    gameName.erase(std::remove(gameName.begin(), gameName.end(), '_'), gameName.end());
                    ownerUsername.erase(std::remove(ownerUsername.begin(), ownerUsername.end(), '_'), ownerUsername.end());

                    // Find the user with the matching username
                    auto it = std::find_if(users.begin(), users.end(), [ownerUsername](const User &user)
                                           { return user.getUsername() == ownerUsername; });

                    // If user is found, add the game to their collection
                    if (it != users.end())
                    {
                        it->addGameToCollection(gameName);
                    }
                    else
                    {
                        std::cerr << "Error: User not found for game in games collection file." << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Error: Invalid line length in games collection file. " << line.length() << std::endl;
                }
            }
        }
        else
        {
            // Notify about an error if the file is not open
            std::cerr << "Error: Unable to read the file. Make sure it is open." << std::endl;
        }
    }

    // Function to close the file if it is open
    void closeFile()
    {
        if (fileStream.is_open())
        {
            fileStream.close();
        }
    }

private:
    // Member variable to store the filename
    std::string filename;

    // Input file stream object to handle file operations
    std::ifstream fileStream;
};

#endif
