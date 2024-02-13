#include "UserSession.h"
#include <iostream>
//Temporary additions to prove in phase 2 that our functions are being loaded properly.
#include <string>
#include <sstream>
#include <fstream>

UserSession::UserSession() {
    // Initialize variables
    loggedIn = false;
    currentUserType = UserType::None;
}

void UserSession::login() {
    std::string username, line, fileUsername, userType, otherInfoStr; // Use a string for otherInfo to initially read from the line
    int otherInfo; // To store the numeric value after parsing

    std::cout << "Please enter your username: ";
    std::getline(std::cin, username);

    std::ifstream userFile("user_accounts.txt");
    bool userFound = false;

    if (userFile.is_open()) {
        while (std::getline(userFile, line)) {
            std::istringstream iss(line);

            // Use std::getline with a delimiter to correctly parse the line
            if (!(std::getline(iss, fileUsername, ',') &&
                  std::getline(iss, userType, ',') &&
                  std::getline(iss, otherInfoStr))) {
                std::cerr << "Error parsing line: " << line << std::endl;
                break; // Skip malformed lines or handle error
            }

            // Convert otherInfoStr to an integer
            otherInfo = std::stoi(otherInfoStr);

            if (username == fileUsername) {
                loggedIn = true;
                currentUser = username;
                userFound = true;

                // Determine the user type based on the 'userType' string
                if (userType == "admin") {
                    currentUserType = UserType::Admin;
                } else {
                    // Add conditions for other user types here
                    currentUserType = UserType::FullStandard; // Example default case
                }

                std::cout << "User '" << currentUser << "' logged in as " << userType << "." << std::endl;
                break;
            }
        }
        userFile.close();

        if (!userFound) {
            std::cout << "Login failed: User not found." << std::endl;
        }
    } else {
        std::cout << "Unable to open user accounts file." << std::endl;
    }
}


/* Fucntion used for the action of login out.
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
