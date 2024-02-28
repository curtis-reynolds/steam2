#include "AdminActions.h"
#include <iostream>
// AdminActions class uses UserAccounts to manipulate user data and TransactionProcessing for refund operations.
#include "UserAccounts.h"
#include "TransactionProcessing.h"

// Constructor for the AdminActions class.
// It takes references to UserAccounts and TransactionProcessing to perform admin-related actions.
AdminActions::AdminActions(UserAccounts& userAccounts, TransactionProcessing& transactionProcessing)
: userAccounts(userAccounts), transactionProcessing(transactionProcessing) {
    // Constructor initialization
}

// Creates a new user with specified username, userType, and initial credit.
// It first checks if the user already exists in the system to prevent duplicates.
void AdminActions::createUser(const std::string& username, UserType userType, float initialCredit) {
    // Check if user already exists
    if (userAccounts.userExists(username)) {
        std::cerr << "Error: User '" << username << "' already exists." << std::endl;
        return;
    }
    // Create the user account
    userAccounts.createUser(username, userType, initialCredit);
   // std::cout << "User '" << username << "' created successfully." << std::endl;
}

// Deletes an existing user from the system by their username.
// It checks if the user exists before attempting deletion.
void AdminActions::deleteUser(const std::string& username) {
    // Check if user exists
    if (!userAccounts.userExists(username)) {
        std::cerr << "Error: User '" << username << "' does not exist." << std::endl;
        return;
    }
    // Delete the user account
    userAccounts.deleteUser(username);
  //  std::cout << "User '" << username << "' deleted successfully." << std::endl;
}

// Issues a refund from one user's account (seller) to another (buyer) by their usernames and the specified amount.
// It calls TransactionProcessing to handle the refund logic.
void AdminActions::issueRefund(const std::string& buyerUsername, const std::string& sellerUsername, float amount) {
    // Process the refund
    transactionProcessing.processRefund(buyerUsername, sellerUsername, amount);
    //std::cout << "Refund of " << amount << " from '" << sellerUsername << "' to '" << buyerUsername << "' processed successfully." << std::endl;
}

// Adds credit to a user's account by their username and the specified amount.
// It calls UserAccounts to handle the credit addition.
void AdminActions::addCredit(const std::string& username, float amount) {
    // Add credit to user account
    userAccounts.addCredit(username, amount);
    std::cout << "Added " << amount << " credit to '" << username << "' successfully." << std::endl;
}

// Displays all user accounts in the system.
// It calls UserAccounts to retrieve all user account information and displays it.
void AdminActions::displayAllAccounts() const {
    auto accountsInfo = userAccounts.getAllAccountsInfo();
    for (const auto& info : accountsInfo) {
        std::cout << info << std::endl;
    }
}
