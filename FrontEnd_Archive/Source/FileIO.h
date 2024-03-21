#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <vector>
#include <sstream>

struct UserAccount {
    std::string username;
    std::string type; // Consider using an enum as in previous examples
    float credit;
    // Constructors, if needed
};

struct Game {
    std::string gameName;
    float price;
    std::string sellerUsername;
    // Constructors, if needed
};

class FileIO {
public:
    FileIO();

    void readUserAccounts(const std::string& filePath, std::vector<UserAccount>& accounts);
    void readGameInventory(const std::string& filePath, std::vector<Game>& games);

    void writeTransactionLog(const std::string& filePath, const std::vector<std::string>& transactions);

private:
    bool parseUserAccountLine(std::istringstream& iss, UserAccount& account);
    bool parseGameLine(std::istringstream& iss, Game& game);
};

#endif // FILEIO_H
