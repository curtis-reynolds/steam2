#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <vector>
#include "User.h"

class SharedData
{
public:
    // Function to get a reference to the current user
    User &getCurrentUser()
    {
        return currentUser;
    }

    // Function to get a reference to the vector of users
    std::vector<User> &getUsers()
    {
        return users;
    }

    // Function to get a reference to the vector of games
    std::vector<Game> &getAvailableGames()
    {
        return availableGames;
    }

    User *getUserByUsername(const std::string &username)
    {
        for (User &user : users)
        {
            if (user.getUsername() == username)
            {
                return &user;
            }
        }

        // If the username is not found
        return nullptr;
    }

    // Function to set the current user to a new user
    void setCurrentUser(const User &newUser)
    {
        currentUser = newUser;
    }

    // Function to set the vector of users to a new set of users
    void setUsers(const std::vector<User> &newUsers)
    {
        users = newUsers;
    }

    // Function to set the vector of games to a new set of games
    void setAvailableGames(const std::vector<Game> &newGames)
    {
        availableGames = newGames;
    }

private:
    // Member variable representing the current user (initialized with default values)
    User currentUser = User("", 0, 0.0);

    // Member variable representing the vector of users
    std::vector<User> users;
    // Member variable representing the vector of games
    std::vector<Game> availableGames;
};

#endif
