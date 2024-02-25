#include "UserSession.h"
#include "UserAccounts.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

// Default constructor initializes the session state as not logged in and sets the user type to None.
UserSession::UserSession(UserAccounts& accounts): userAccounts(accounts) {
    loggedIn = false; // Initially, no user is logged in.
    currentUserType = UserType::None; // The user type is set to None indicating no current user.
}

// The login method prompts the user for a username and attempts to authenticate them by checking against stored user accounts.
void UserSession::login() {
    std::string username;
    std::cout << "Please enter your username: ";
    std::getline(std::cin, username);

    std::ifstream userFile("user_accounts.txt");
    if (!userFile.is_open()) {
        std::cout << "Unable to open user accounts file." << std::endl;
        return;
    }

    std::string line;
    bool userFound = false;
    while (std::getline(userFile, line)) {
        if (line.size() < 28) continue; // Ensure the line has the expected length

        std::string fileUsername = line.substr(0, 15);
        std::string userTypeStr = line.substr(16, 2);
        std::string creditStr = line.substr(19, 9);

        // Trim trailing spaces from username
        fileUsername.erase(fileUsername.find_last_not_of(" ") + 1);

        if (fileUsername != username) continue;

        loggedIn = true;
        currentUser = username;
        userFound = true;

        // Convert credit string to float
        float credit = std::stof(creditStr);

        // Map the userTypeStr to UserType enum
        if (userTypeStr == "AA") {
            currentUserType = UserType::Admin;
        } else if (userTypeStr == "FS") {
            currentUserType = UserType::FullStandard;
        } else if (userTypeStr == "BS") {
            currentUserType = UserType::BuyStandard;
        } else if (userTypeStr == "SS") {
            currentUserType = UserType::SellStandard;
        } else {
            currentUserType = UserType::None; // Handle unknown user type
        }

        std::cout << "User '" << currentUser << "' logged in with $" << std::fixed << std::setprecision(2) << credit << " available credit." << std::endl;
        break;
    }

    if (!userFound) {
        std::cout << "Login failed: User not found." << std::endl;
    }
}

/* Function used for the action of login out.
   It verifies if a user is logged in and returns a message. In the event that it is, then it proceeds 
   to log out the user send a message confirming that the user has been logged out as well as generate
   the daily transaction file. In the event that no user is logged in then a message will be made notifying
   that no user is logged in. */
std::string UserSession::logout() {
    std::ostringstream logoutMessage;
    if (loggedIn) {
        logoutMessage << "User '" << currentUser << "' logged out.\n";

        // Format the end of session transaction
        std::string transactionCode = "00";
        std::string userTypeStr = [this]() {
            switch (currentUserType) {
                case UserType::Admin: return "AA";
                case UserType::FullStandard: return "FS";
                case UserType::BuyStandard: return "BS";
                case UserType::SellStandard: return "SS";
                default: return "  "; // Placeholder for unknown or none
            }
        }();

        float currentCredit = userAccounts.getUserCredit(currentUser);

        std::ostringstream transactionStream;
        transactionStream << std::left << std::setw(2) << transactionCode << "_"
                          << std::setw(15) << currentUser << "_"
                          << std::setw(2) << userTypeStr << "_"
                          << std::right << std::setw(9) << std::setfill('0') 
                          << std::fixed << std::setprecision(2) << currentCredit;

        // Record end of session transaction
        recordTransaction(transactionStream.str());

        // Open transaction file for appending
        std::ofstream transactionFile("transout.atf", std::ios::app);

        // Write each transaction including end of session
        for (const auto& transaction : transactionLogs) {
            transactionFile << transaction << std::endl;
        }
        transactionFile.close(); // Close the file

        // Clear transaction logs for next session
        transactionLogs.clear();

        loggedIn = false;
        currentUser.clear();
        currentUserType = UserType::None;
    } else {
        logoutMessage << "No user is currently logged in.\n";
    }

    return logoutMessage.str();
}


// Function returns the boolean variable "loggedIn". 
bool UserSession::isLoggedIn() const {
    return loggedIn;
}

// Function returns the current users username.
std::string UserSession::getCurrentUser() const {
    return currentUser;
}

// Function returns the current user type.
UserType UserSession::getCurrentUserType() const {
    return currentUserType;
}

void UserSession::recordTransaction(const std::string& transaction) {
    transactionLogs.push_back(transaction);
}