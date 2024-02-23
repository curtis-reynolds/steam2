#include "UserAccounts.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

// Default constructor that initializes the file path to a default value.
// This constructor sets the path to 'user_accounts.txt', allowing for loading without specifying a path.
UserAccounts::UserAccounts() {
    accountsFilePath; // Sets the default file path for user accounts.
    loadAccounts(); // Calls loadAccounts to read and process the user data from the file.
}

// Constructor with a file path parameter for specifying a custom path to the user accounts file.
// This allows the class to work with different files as needed, increasing flexibility.
UserAccounts::UserAccounts(const std::string& accountsFile)
: accountsFilePath(accountsFile) {
    loadAccounts(); 
}

// Loads user account data from the specified file.
// Opens the file and reads each line, parsing the user data to create UserAccount objects.
void UserAccounts::loadAccounts() {
    std::ifstream file(accountsFilePath); // Attempts to open the file.
    if (!file) { // Checks for file open failure.
        std::cerr << "Error: Unable to open accounts file." << std::endl;
        return; // Exits the function if the file cannot be opened.
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string username, typeStr;
        float credit = 0.0f;
        UserType userType = UserType::None; // Default value for safety.

        // Use getline to correctly handle commas as delimiters.
        std::getline(iss, username, ',');
        std::getline(iss, typeStr, ',');
        iss >> credit;

        // Convert the type string to the UserType enum.
        if (typeStr == "admin") userType = UserType::Admin;
        else if (typeStr == "full-standard") userType = UserType::FullStandard;
        else if (typeStr == "buy-standard") userType = UserType::BuyStandard;
        else if (typeStr == "sell-standard") userType = UserType::SellStandard;
        // Additional conditionals can be added here for other user types.

        // Creates a UserAccount object with the parsed data and adds it to the accounts vector.
        UserAccount account(username, userType, credit);
        accounts.push_back(account);
    }

    file.close(); // Closes the file after processing all lines.
}


// Retrieves information for all user accounts in a formatted string vector.
std::vector<std::string> UserAccounts::getAllAccountsInfo() const {
    std::vector<std::string> accountsInfo; // Vector to hold the formatted account information strings.

    // Opens the user accounts file to read the data.
    std::ifstream file(accountsFilePath);
    if (!file) { // Checks if the file opening failed.
        std::cerr << "Error: Unable to open accounts file at " << accountsFilePath << std::endl;
        return accountsInfo; // Returns an empty vector if the file cannot be opened.
    }

    std::string line;
    while (getline(file, line)) { // Reads each line from the file.
        std::istringstream iss(line); // Uses istringstream for parsing.
        std::string username, userTypeStr;
        float credit;
        char delim; // Represents the delimiter, used for parsing.

        // Parses the line based on the expected format: "username,type,credit".
        // Skips malformed lines and logs an error.
        if (!(std::getline(iss, username, ',') &&
              std::getline(iss, userTypeStr, ',') &&
              iss >> credit)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue; // Moves to the next line if the current line is malformed.
        }

        // Formats and adds the user account information to the vector.
        std::string info = "Username: " + username + 
                           ", Type: " + userTypeStr + 
                           ", Credit: " + std::to_string(credit);
        accountsInfo.push_back(info);
    }

    return accountsInfo; // Returns the vector containing all account information strings.
}

// Adds a new user to the system with the specified username, user type, and initial credit.
// Checks for duplicates and username length constraints before adding the user.
void UserAccounts::createUser(const std::string& username, UserType type, float credit) {
    // Checks if the username already exists in the list of accounts to prevent duplicates.
    if (std::any_of(accounts.begin(), accounts.end(), [&](const UserAccount& account) {
        return account.username == username;
    })) {
        std::cerr << "Error: Username already exists." << std::endl;
        return;
    }
    // Also checks if the username exceeds the maximum allowed length, enforcing constraints on user data.
    if (username.length() > 15) {
        std::cerr << "Error: Username exceeds maximum length of 15 characters." << std::endl;
        return;
    }

    // Here we would add additional validation as necessary
    // If validation passes, creates a new UserAccount object and adds it to the accounts vector.
    UserAccount newAccount(username, type, credit);
    accounts.push_back(newAccount);

    // We could write changes to the file here or in a separate method
    saveAccounts();
}

// Deletes a user from the system based on the username.
// Searches for the user in the list of accounts and removes them if found.
void UserAccounts::deleteUser(const std::string& username) {
    // Uses std::remove_if to find and remove the user from the accounts vector based on the username.
    auto it = std::remove_if(accounts.begin(), accounts.end(), [&](const UserAccount& account) {
        return account.username == username;
    });
    // If the user is not found, an error message is displayed.
    if (it == accounts.end()) {
        std::cerr << "Error: User not found." << std::endl;
        return;
    }

    accounts.erase(it, accounts.end());

    // We could write changes to the file here or in a separate method
    saveAccounts();
}

// Saves the current list of user accounts to the file specified by accountsFilePath.
// Opens the file for writing and overwrites any existing content.
void UserAccounts::saveAccounts() {
    // Attempts to open the accounts file for writing.
    std::ofstream file(accountsFilePath);
    if (!file) {
        std::cerr << "Error: Unable to open accounts file for writing." << std::endl;
        return;
    }
     // If successful, iterates through the accounts vector and writes each account's data to the file.
    for (const auto& account : accounts) {
        file << account.username << ","
             << userTypeToString(account.type) << ","
             << account.credit << std::endl;
    }
    // Closes the file after writing is complete.
    file.close();
}

// Converts a UserType enum to its corresponding string representation.
std::string UserAccounts::userTypeToString(UserType type) const {
    // Maps UserType enum values to their string equivalents for file storage and display purposes.
    // Handles all defined UserType values, returning a string for each.
    switch (type) {
        case UserType::Admin: return "admin";
        case UserType::FullStandard: return "full-standard";
        case UserType::BuyStandard: return "buy-standard";
        case UserType::SellStandard: return "sell-standard";
        default: return "none";
    }
}

// Checks if a user exists in the system based on the username.
bool UserAccounts::userExists(const std::string& username) {
    // Iterates through the accounts vector, searching for an account with the specified username.
    // Returns true if found, false otherwise.
    for (const auto& account : accounts) {
        if (account.username == username) {
            return true;
        }
    }
    return false;
}

// Adds credit to a user's account, identified by username, in the amount specified.
void UserAccounts::addCredit(const std::string& username, float amount) {
    // Searches for the user in the accounts list and adds the specified amount to their credit.
    for (auto& account : accounts) {
        if (account.username == username) {
            account.credit += amount;
            // Make sure to implement any necessary checks, e.g., maximum credit limit
            return;
        }
    }
    // If the user is not found, an error message is displayed.
    std::cerr << "Error: User '" << username << "' not found." << std::endl;
}