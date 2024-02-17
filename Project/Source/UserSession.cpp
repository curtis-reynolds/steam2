#include "UserSession.h"
#include <iostream>
// Include necessary headers for handling strings and file I/O.
#include <string>
#include <sstream>
#include <fstream>

// Default constructor initializes the session state as not logged in and sets the user type to None.
UserSession::UserSession() {
    loggedIn = false; // Initially, no user is logged in.
    currentUserType = UserType::None; // The user type is set to None indicating no current user.
}

// The login method prompts the user for a username and attempts to authenticate them by checking against stored user accounts.
void UserSession::login() {
    std::string username, line, fileUsername, userType, otherInfoStr; // Declare variables for parsing the file and user input.
    int otherInfo; // Variable to store additional user information after conversion from string to integer.

    std::cout << "Please enter your username: "; // Prompt for the username.
    std::getline(std::cin, username); // Read the username from standard input.

    // Open the file containing user accounts information.
    std::ifstream userFile("user_accounts.txt");
    bool userFound = false; // Flag to indicate whether the user was found in the file.

    // Check if the file was successfully opened.
    if (userFile.is_open()) {
        // Read the file line by line.
        while (std::getline(userFile, line)) {
            std::istringstream iss(line); // Use string stream to parse the line.

            // Parse the line into username, user type, and other information using ',' as a delimiter.
            if (!(std::getline(iss, fileUsername, ',') &&
                  std::getline(iss, userType, ',') &&
                  std::getline(iss, otherInfoStr))) {
                std::cerr << "Error parsing line: " << line << std::endl; // Handle parsing error.
                break; // Optionally, skip malformed lines or exit the loop.
            }

            // Convert additional user information from string to integer.
            otherInfo = std::stoi(otherInfoStr);

            // Check if the input username matches the username from the file.
            if (username == fileUsername) {
                loggedIn = true; // Set the session state to logged in.
                currentUser = username; // Store the current username.
                userFound = true; // Mark that the user has been found.

                // Determine the user type based on the string value read from the file.
                if (userType == "admin") {
                    currentUserType = UserType::Admin; // Set the user type for admin.
                } else {
                    // Additional conditions for other user types.
                    currentUserType = UserType::FullStandard; // Example default case or adjust as needed.
                }

                std::cout << "User '" << currentUser << "' logged in as " << userType << "." << std::endl;
                break; // Exit the loop once the user is found and logged in.
            }
        }
        userFile.close(); // Close the file after processing.

        // If the user was not found, display a login failure message.
        if (!userFound) {
            std::cout << "Login failed: User not found." << std::endl;
        }
    } else {
        // If the file could not be opened, display an error message.
        std::cout << "Unable to open user accounts file." << std::endl;
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
