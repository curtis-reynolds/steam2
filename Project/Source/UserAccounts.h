#ifndef USERACCOUNTS_H
#define USERACCOUNTS_H

#include <string>
#include <vector>
#include "Types.h"

struct UserAccount {
    std::string username;
    UserType type;
    float credit;

    UserAccount(const std::string& uname, UserType utype, float cred)
    : username(uname), type(utype), credit(cred) {}
};

class UserAccounts {
private:
    std::vector<UserAccount> accounts;
    std::string accountsFilePath;

    void loadAccounts();
    void saveAccounts();
    std::string userTypeToString(UserType type);

public:
    UserAccounts();
    UserAccounts(const std::string& accountsFile);
    void createUser(const std::string& username, UserType type, float credit);
    void deleteUser(const std::string& username);
    bool userExists(const std::string& username);
    void addCredit(const std::string& username, float amount);
    // Other necessary member functions...
};

#endif // USERACCOUNTS_H
