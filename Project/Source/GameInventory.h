#ifndef GAMEINVENTORY_H
#define GAMEINVENTORY_H

#include <string>
#include <vector>

struct Game {
    std::string gameName;
    float price;
    std::string sellerUsername;

    Game(const std::string& name, float cost, const std::string& seller)
    : gameName(name), price(cost), sellerUsername(seller) {}
};

class GameInventory {
private:
    std::vector<Game> inventory;
    std::string inventoryFilePath;

    void loadInventory();
    void saveInventory();

public:
    GameInventory();
    GameInventory(const std::string& inventoryFile);
    void addGame(const std::string& gameName, float price, const std::string& sellerUsername);
    // Other necessary member functions...
};

#endif // GAMEINVENTORY_H
