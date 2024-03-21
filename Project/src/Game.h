#ifndef GAME_H
#define GAME_H

#include <string>

class Game
{
public:
    // Constructor that takes game name and price as parameters
    Game(const std::string &gameName, const std::string &sellerName, double price) : gameName(gameName), sellerName(sellerName), gamePrice(price) {}

    // Getter methods to retrieve game information
    std::string getGameName() const
    {
        return gameName;
    }

    std::string getSellerName() const
    {
        return sellerName;
    }

    double getPrice() const
    {
        return gamePrice;
    }

private:
    // Member variables to store game information
    std::string gameName;
    std::string sellerName;
    double gamePrice;
};

#endif
