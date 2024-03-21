#ifndef USERUPDATER_H
#define USERUPDATER_H

#include "general.h"
#include "FileWriter.h"
#include "User.h"

class UserUpdater : public FileWriter
{
public:
    // Constructor that takes a filename as a parameter
    UserUpdater(const std::string &filename) : FileWriter(filename){};

    // Function to add a user to the file
    void addUser(const User &user)
    {
        removeEndLine(28); // Remove the existing END line
        writeUser(user);   // Write the user information to the file
        writeEndLine(28);  // Add the END line back to the file
    }

    // Function to remove a user from the file
    void deleteUser(const User &user)
    {
        removeUser(user.getUsername()); // Write the user information to the file
    }

    // Function to update the credit for a user in the file
    void updateUserCredit(const User &user, double newCredit)
    {
        if (openFile())
        {
            removeUser(user.getUsername());                                      // Remove the existing user
            User updatedUser(user.getUsername(), user.getType() + 1, newCredit); // Create a new user object with the updated credit
            addUser(updatedUser);                                                // Add updated user
            closeFile();
        }
        else
        {
            std::cerr << "Error: Unable to open the file for writing." << std::endl;
            return;
        }
    }

private:
    // Function to write user information to the file
    void writeUser(const User &user)
    {
        if (fileStream.is_open())
        {
            std::string username = user.getUsername();
            username.resize(16, '_'); // Ensure the username is 16 characters long

            std::string userType = userTypes[user.getType() - 1]; // Get user type code

            // Format the credit with leading zeros and ".00" suffix
            std::ostringstream formattedCreditStream;
            formattedCreditStream << std::fixed << std::setw(9) << std::setfill('0') << std::setprecision(2) << user.getCredit();
            std::string formattedCredit = formattedCreditStream.str();

            // Write the user information to the file
            fileStream << std::setw(16) << std::left << username
                       << std::setw(2) << std::left << userType << "_"
                       << formattedCredit << "\n";

            // Reopen the file in append mode
            fileStream.open(filename, std::ios::app);
        }
        else
        {
            std::cerr << "Error: Unable to write to the file. Make sure it is open." << std::endl;
        }
    }

    void removeUser(const std::string &username)
    {
        if (fileStream.is_open())
        {
            // Close the file
            fileStream.close();

            // Create a temporary file to store all lines except the user to be removed
            std::ifstream inFile(filename); // Change to ifstream for reading
            std::ofstream tempFile("temp.txt");
            if (!tempFile.is_open())
            {
                std::cerr << "Error: Unable to create temporary file." << std::endl;
                return;
            }

            std::string line;

            // Read and store all lines except the user to be removed
            while (std::getline(inFile, line))
            {
                // Extract username from the line
                std::string userLineUsername = line.substr(0, 16);
                // Remove underscores from username
                userLineUsername.erase(std::remove(userLineUsername.begin(), userLineUsername.end(), '_'), userLineUsername.end());

                // If the username does not match the one to be removed, write the line to the temporary file
                if (userLineUsername != username)
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
            std::cerr << "Error: Unable to write to the file. Make sure it is open.b" << std::endl;
        }
    }
};

#endif
