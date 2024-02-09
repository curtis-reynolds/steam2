#include "UserSession.h"
#include <iostream>
//Temporary additions to prove in phase 2 that our functions are being loaded properly.
#include <string>
#include <sstream>

UserSession::UserSession() {
    // Initialize variables
    loggedIn = false;
    currentUserType = UserType::None;
}


void UserSession::login() {
    std::string username;
    std::cout << "Please enter your username: ";
    std::getline(std::cin, username);

    // Here you would normally check against the user accounts file
    // For now, we'll just simulate a successful login
    loggedIn = true;
    currentUser = username;

    // Determine the user type (Admin, Full-Standard, Buy-Standard, Sell-Standard)
    // This would typically be loaded from the user accounts file
    // For the sake of the example, let's say all users are Full-Standard
    currentUserType = UserType::FullStandard;

    std::cout << "User '" << currentUser << "' logged in as Full-Standard." << std::endl;
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
