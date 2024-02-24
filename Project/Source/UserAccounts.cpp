#include "UserAccounts.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
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

// Utility function to trim spaces from the right of a string
std::string rtrim(const std::string& s) {
    size_t end = s.find_last_not_of(" ");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

// Loads user account data from the specified file.
// Opens the file and reads each line, parsing the user data to create UserAccount objects.
void UserAccounts::loadAccounts() {
    std::ifstream file(accountsFilePath);
    std::string line;
    while (getline(file, line)) {
        if (line.substr(0, 15).find("END") != std::string::npos) break; // Stop if "END" user found

        std::string username = rtrim(line.substr(0, 15));
        std::string userTypeStr = line.substr(16, 2);
        std::string creditStr = line.substr(19, 9);

        UserType userType;
        if (userTypeStr == "AA") userType = UserType::Admin;
        else if (userTypeStr == "FS") userType = UserType::FullStandard;
        else if (userTypeStr == "BS") userType = UserType::BuyStandard;
        else if (userTypeStr == "SS") userType = UserType::SellStandard;
        else continue; // Skip invalid user types

        float credit = std::stof(creditStr);

        accounts.emplace_back(username, userType, credit);
    }
    file.close();
}

// Retrieves information for all user accounts in a formatted string vector.
std::vector<std::string> UserAccounts::getAllAccountsInfo() const {
    std::vector<std::string> accountsInfo;

    std::ifstream file(accountsFilePath);
    if (!file) {
        std::cerr << "Error: Unable to open accounts file at " << accountsFilePath << std::endl;
        return accountsInfo;
    }

    std::string line;
    while (getline(file, line)) {
        if (line.substr(0, 15).find("END") != std::string::npos) break; // Stop if "END" user found

        std::string username = line.substr(0, 15);
        std::string userTypeStr = line.substr(16, 2);
        std::string creditStr = line.substr(19, 9);

        // Trim the username of trailing spaces for display
        username.erase(username.find_last_not_of(" ") + 1);

        // Map user type code to descriptive string (if necessary)
        std::string userType;
        if (userTypeStr == "AA") userType = "Admin";
        else if (userTypeStr == "FS") userType = "Full-Standard";
        else if (userTypeStr == "BS") userType = "Buy-Standard";
        else if (userTypeStr == "SS") userType = "Sell-Standard";
        else userType = "Unknown"; // Fallback for unrecognized codes

        // Convert credit string to float and back to string to remove leading zeros
        float credit = std::stof(creditStr);
        std::ostringstream creditStream;
        creditStream << std::fixed << std::setprecision(2) << credit;
        std::string formattedCredit = creditStream.str();

        // Format the line for display
        std::string info = "Username: " + username + ", Type: " + userType + ", Credit: " + formattedCredit;
        accountsInfo.push_back(info);
    }

    return accountsInfo;
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
    // Check if the initial credit exceeds the maximum allowed value.
    if (credit > 999999.99) {
        std::cerr << "Error: Initial credit exceeds maximum allowed value." << std::endl;
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
    std::ofstream file(accountsFilePath);
    if (!file) {
        std::cerr << "Error: Unable to open accounts file for writing." << std::endl;
        return;
    }

    for (const auto& account : accounts) {
        // Username: left-justified, padded with spaces
        file << std::left << std::setw(15) << std::setfill(' ') << account.username;
        file << " "; // Space separator

        // User type
        file << userTypeToString(account.type);

        // Credit: right-justified, padded with zeros, ensuring two decimal places
        file << " " << std::right << std::setw(9) << std::setfill('0') 
             << std::fixed << std::setprecision(2) << account.credit;

        file << std::endl;
    }

    // Write the "END" line with appropriate spacing and zeros
    file << "END             __ 000000000\n";
}

// Converts a UserType enum to its corresponding string representation.
std::string UserAccounts::userTypeToString(UserType type) const {
    switch (type) {
        case UserType::Admin: return "AA";
        case UserType::FullStandard: return "FS";
        case UserType::BuyStandard: return "BS";
        case UserType::SellStandard: return "SS";
        default: return "__"; // Placeholder for unknown types
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