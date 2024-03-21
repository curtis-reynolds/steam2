#ifndef USER_H
#define USER_H

#include <string>
#include "general.h"
#include "Game.h"

class User
{
public:
    // Constructor that takes username, user type, and credit as parameters
    User(const std::string &username, const int &type, const double &credit)
        : username(username), type(type), credit(credit){};

    // Getter methods to retrieve user information
    std::string getUsername() const
    {
        return username;
    }

    int getType() const
    {
        return type;
    }

    double getCredit() const
    {
        return credit;
    }

    const std::vector<Game> &getGamesCollection() const
    {
        return gamesCollection;
    }

    // Setter method to update the user's credit
    void setCredit(double credit)
    {
        this->credit = credit;
    }

    // Function to add a game to the user's collection by name
    void addGameToCollection(const std::string &gameName)
    {
        gamesCollection.push_back(Game(gameName, username, 0.0));
    }

    bool hasGameInCollection(const std::string &gameName)
    {
        for (Game &ownedGame : gamesCollection)
        {
            if (ownedGame.getGameName() == gameName)
            {
                return true;
            }
        }
        return false;
    }

private:
    // Member variables to store user information
    std::string username;
    int type;
    double credit;
    // Member variable representing the vector of games in the user's collection
    std::vector<Game> gamesCollection;
};

#endif
