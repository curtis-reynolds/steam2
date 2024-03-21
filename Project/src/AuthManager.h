#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <iostream>
#include <vector>
#include "User.h"
#include "FileReader.h"
#include "SharedData.h"

class AuthManager
{
public:
    // Constructor that takes a SharedData reference and a file name
    AuthManager(SharedData &sharedData, const std::string fileName)
        : sharedData(sharedData),
          fileReader(fileName),
          users(sharedData.getUsers()),
          currentUser(sharedData.getCurrentUser()) {}

    // Function to handle user login
    bool login()
    {
        std::string username;

        while (true)
        {
            std::cout << "Enter username: ";
            std::cin >> username;

            // Iterate through the vector of users to find a match
            for (const User &user : users)
            {
                if (user.getUsername() == username)
                {
                    // Set the current user in shared data and indicate successful login
                    sharedData.setCurrentUser(user);
                    std::cout << "Login successful." << std::endl;
                    return true;
                }
            }

            // If no matching username is found, notify the user and return false
            std::cout << "Invalid username" << std::endl;
            return false;
        }
    }

    // Function to handle user logout
    bool logout()
    {
        try
        {
            // Set the current user in shared data to a default user (empty username)
            sharedData.setCurrentUser(User("", 0, 0.0));
            std::cout << "Logout successful." << std::endl;
            return true;
        }
        catch (const std::exception &e)
        {
            // Catch and print any exceptions that might occur during the logout process
            std::cerr << e.what() << '\n';
            return false;
        }
    }

private:
    // Reference to the current user in shared data
    User &currentUser;

    // Reference to the vector of users in shared data
    std::vector<User> &users;

    // Reference to the shared data object
    SharedData &sharedData;

    // FileReader object to handle file operations
    FileReader fileReader;
};

#endif