#include "AdminActions.h"
#include <iostream>
// Include other necessary components
#include "UserAccounts.h"
#include "TransactionProcessing.h"

AdminActions::AdminActions(UserAccounts& userAccounts, TransactionProcessing& transactionProcessing)
: userAccounts(userAccounts), transactionProcessing(transactionProcessing) {
    // Constructor initialization
}

void AdminActions::createUser(const std::string& username, UserType userType, float initialCredit) {
    // Check if user already exists
    if (userAccounts.userExists(username)) {
        std::cerr << "Error: User '" << username << "' already exists." << std::endl;
        return;
    }
    // Create the user account
    userAccounts.createUser(username, userType, initialCredit);
    std::cout << "User '" << username << "' created successfully." << std::endl;
}

void AdminActions::deleteUser(const std::string& username) {
    // Check if user exists
    if (!userAccounts.userExists(username)) {
        std::cerr << "Error: User '" << username << "' does not exist." << std::endl;
        return;
    }
    // Delete the user account
    userAccounts.deleteUser(username);
    std::cout << "User '" << username << "' deleted successfully." << std::endl;
}

void AdminActions::issueRefund(const std::string& buyerUsername, const std::string& sellerUsername, float amount) {
    // Process the refund
    transactionProcessing.processRefund(buyerUsername, sellerUsername, amount);
    std::cout << "Refund of " << amount << " from '" << sellerUsername << "' to '" << buyerUsername << "' processed successfully." << std::endl;
}

void AdminActions::addCredit(const std::string& username, float amount) {
    // Add credit to user account
    userAccounts.addCredit(username, amount);
    std::cout << "Added " << amount << " credit to '" << username << "' successfully." << std::endl;
}

// Implement other administrative actions as necessary
