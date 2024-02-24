#include "UserSession.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

// Default constructor initializes the session state as not logged in and sets the user type to None.
UserSession::UserSession() {
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
     //Verifies user logged in or logged out.
    if (loggedIn) {
        logoutMessage << "User '" << currentUser << "' logged out." << std::endl;
        loggedIn = false;
        currentUser.clear();
        currentUserType = UserType::None;
        // Write out the daily transaction file here
    } else {
        logoutMessage << "No user is currently logged in." << std::endl;
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
