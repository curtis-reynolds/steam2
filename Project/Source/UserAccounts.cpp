#include "UserAccounts.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

UserAccounts::UserAccounts() {
    // Initialize with default values or load from a default file path
}

// Takes in value of the AccountsFile variable and passes it to the loadAccounts function.
UserAccounts::UserAccounts(const std::string& accountsFile)
: accountsFilePath(accountsFile) {
    loadAccounts();
}

void UserAccounts::loadAccounts() {
    std::ifstream file(accountsFilePath);
    if (!file) {
        std::cerr << "Error: Unable to open accounts file." << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        // Parse the line to create a UserAccount object
        // Assume the line format is "username,type,credit"

        std::istringstream iss(line);
        std::string username, type;
        float credit;
        char delim;

        iss >> username >> delim >> type >> delim >> credit;
        UserType userType = UserType::None;

        // Convert string to UserType enum
        if (type == "admin") userType = UserType::Admin;
        else if (type == "full-standard") userType = UserType::FullStandard;
        else if (type == "buy-standard") userType = UserType::BuyStandard;
        else if (type == "sell-standard") userType = UserType::SellStandard;

        UserAccount account(username, userType, credit);
        accounts.push_back(account);
    }
    file.close();
}

void UserAccounts::createUser(const std::string& username, UserType type, float credit) {
    if (std::any_of(accounts.begin(), accounts.end(), [&](const UserAccount& account) {
        return account.username == username;
    })) {
        std::cerr << "Error: Username already exists." << std::endl;
        return;
    }

    if (username.length() > 15) {
        std::cerr << "Error: Username exceeds maximum length of 15 characters." << std::endl;
        return;
    }

    // Here we would add additional validation as necessary

    UserAccount newAccount(username, type, credit);
    accounts.push_back(newAccount);

    // We could write changes to the file here or in a separate method
    saveAccounts();
}

void UserAccounts::deleteUser(const std::string& username) {
    auto it = std::remove_if(accounts.begin(), accounts.end(), [&](const UserAccount& account) {
        return account.username == username;
    });

    if (it == accounts.end()) {
        std::cerr << "Error: User not found." << std::endl;
        return;
    }

    accounts.erase(it, accounts.end());

    // We could write changes to the file here or in a separate method
    saveAccounts();
}

void UserAccounts::saveAccounts() {
    std::ofstream file(accountsFilePath);
    if (!file) {
        std::cerr << "Error: Unable to open accounts file for writing." << std::endl;
        return;
    }

    for (const auto& account : accounts) {
        file << account.username << ","
             << userTypeToString(account.type) << ","
             << account.credit << std::endl;
    }
    file.close();
}

std::string UserAccounts::userTypeToString(UserType type) {
    switch (type) {
        case UserType::Admin: return "admin";
        case UserType::FullStandard: return "full-standard";
        case UserType::BuyStandard: return "buy-standard";
        case UserType::SellStandard: return "sell-standard";
        default: return "none";
    }
}

bool UserAccounts::userExists(const std::string& username) {
    // Implement logic to check if a user exists in the accounts list
    for (const auto& account : accounts) {
        if (account.username == username) {
            return true;
        }
    }
    return false;
}

void UserAccounts::addCredit(const std::string& username, float amount) {
    // Implement logic to add credit to a user's account
    for (auto& account : accounts) {
        if (account.username == username) {
            account.credit += amount;
            // Make sure to implement any necessary checks, e.g., maximum credit limit
            return;
        }
    }
    std::cerr << "Error: User '" << username << "' not found." << std::endl;
}