#include "UserSession.h"
#include <iostream>

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

void UserSession::logout() {
    if (loggedIn) {
        std::cout << "User '" << currentUser << "' logged out." << std::endl;
        loggedIn = false;
        currentUser.clear();
        currentUserType = UserType::None;
        // Write out the daily transaction file here
    } else {
        std::cout << "No user is currently logged in." << std::endl;
    }
}

bool UserSession::isLoggedIn() const {
    return loggedIn;
}

std::string UserSession::getCurrentUser() const {
    return currentUser;
}

UserType UserSession::getCurrentUserType() const {
    return currentUserType;
}
