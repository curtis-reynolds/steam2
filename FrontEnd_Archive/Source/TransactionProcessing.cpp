#include "TransactionProcessing.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
// TransactionProcessing class relies on UserAccounts and GameInventory to process transactions.
#include "UserAccounts.h"
#include "GameInventory.h"
#include "GameCollection.h"
#include "UserSession.h"

// Constructor initializes the class with references to the UserAccounts and GameInventory,
// allowing transactions to interact with user data and the game inventory.
TransactionProcessing::TransactionProcessing(UserAccounts& userAccounts, GameInventory& gameInventory)
: userAccounts(userAccounts), gameInventory(gameInventory) {
    // Initialize with references to user accounts and game inventory
}

// Main method to process transactions based on a transaction code and arguments.
// It routes the transaction to the appropriate handler based on the code.
void TransactionProcessing::processTransaction(const std::string& transactionCode, const std::vector<std::string>& args) {
    // Routes to the correct transaction processing method based on the transactionCode.
    // Each transaction type has its own dedicated method for processing.
    if (transactionCode == "sell") {
        processSellTransaction(args);
    } else if (transactionCode == "buy") {
        processBuyTransaction(args);
    } else if (transactionCode == "refund") {
        processRefundTransaction(args);
    } else if (transactionCode == "addcredit") {
        processAddCreditTransaction(args);
    } else {
        std::cerr << "Unknown transaction code: " << transactionCode << std::endl;
    }
}

// Processes a sell transaction, allowing a user to list a game for sale.
// Validates the provided arguments and, if valid, adds the game to the game inventory.
void TransactionProcessing::processSellTransaction(const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cerr << "Error: Missing arguments for selling a game." << std::endl;
        return;
    }

    // Extract information from args
    std::string gameName = args[0];
    std::string sellerUsername = args[1];
    float price;

    // Try to convert price to float
    try {
        price = std::stof(args[2]);
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: Invalid price format." << std::endl;
        return;
    } catch (const std::out_of_range& e) {
        std::cout << "Error: Price is out of range." << std::endl;
        return;
    }

    // Add the game to the inventory
    gameInventory.addGame(gameName, price, sellerUsername);

    // Save transaction to the daily transaction file
    // Write Sell Transaction to the Transaction file
    std::ostringstream transactionStream;
    transactionStream << std::left << std::setw(2) << "03" << "_"
                      << std::setw(15) << gameName << "_"
                      << std::setw(15) << sellerUsername << "_"
                      << std::right << std::setw(9)
                      << std::fixed << std::setprecision(2) << price;

    // Record sell transaction
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
}

// Processes a buy transaction, allowing a user to purchase a game.
// Validates the provided arguments and, if valid, updates the game inventory and the buyer's account.
void TransactionProcessing::processBuyTransaction(const std::vector<std::string>& args) {
    // Create instance of GameCollection
    GameCollection gameCollection;

    if (args.size() < 3) {
        std::cerr << "Error: Missing arguments for buying a game. Required: buyerUsername, gameName, sellerUsername." << std::endl;
        return;
    }

    std::string buyerUsername = args[0];
    std::string gameName = args[1];
    std::string sellerUsername = args[2];

    // if the seller username does not match an existing user, return an error
    if (!userAccounts.userExists(sellerUsername)) {
        std::cout << "Error: The specified seller does not exist." << std::endl;
        return;
    }

    // Validate that the game exists and is available for sale
    if (!gameInventory.gameExists(gameName)) {
        std::cout << "Error: The specified game is not available for sale by the user " << sellerUsername << "." << std::endl;
        return;
    }

    float gamePrice = gameInventory.getGamePrice(gameName, sellerUsername);

    // Check if the buyer has sufficient credit
    if (!userAccounts.hasSufficientCredit(buyerUsername, gamePrice)) {
        std::cout << "Error: The buyer does not have sufficient credit to complete the purchase." << std::endl;
        return;
    }

    // Check if the buyer already owns the game
    if (gameCollection.ownsGame(buyerUsername, gameName)) {
        std::cout << "Error: The buyer already owns a copy of this game." << std::endl;
        return;
    }

    // Process the transaction: Update credits and transfer game ownership
    userAccounts.processPurchase(buyerUsername, sellerUsername, gamePrice);
    gameCollection.addGameToUser(buyerUsername, gameName);

    std::cout << "Purchase successful. " << gameName << " has been added to " << buyerUsername << "'s collection." << std::endl;

    // save transaction to the daily transaction file
}


// Processes a refund transaction between two users.
// Validates the provided arguments and, if valid, updates the accounts of the buyer and seller accordingly.
void TransactionProcessing::processRefundTransaction(const std::vector<std::string>& args) {
    // TODO: Implement the logic to refund, e.g., validate args and update user accounts accordingly
    std::cout << "Processing refund transaction" << std::endl;
}

// Processes a transaction to add credit to a user's account.
// Validates the provided arguments and, if valid, updates the user's account with the added credit.
void TransactionProcessing::processAddCreditTransaction(const std::vector<std::string>& args) {
    // In admin mode, args should contain username and amount. In user mode, just the amount.
    if (args.empty()) {
        std::cerr << "Error: No arguments provided for add credit transaction." << std::endl;
        return;
    }

    std::string username = args[0];
    float amount;

    // error check if the amount is invalid
    try {
        amount = std::stof(args[1]);
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: Invalid amount format." << std::endl;
        return;
    } catch (const std::out_of_range& e) {
        std::cout << "Error: Amount is out of range." << std::endl;
        return;
    }

    if (amount <= 0 || amount > 1000) {
        std::cout << "Error: Invalid amount. Must be between $0 and $1000." << std::endl;
        return;
    }

    // Add the credit to the user's account
    userAccounts.addCredit(username, amount);
    //std::cout << "Successfully added $" << amount << " to " << username << "'s account." << std::endl;

    // Save transaction to the daily transaction file
}


// A specific method for processing refunds not triggered by the main transaction processing method.
// This could be used for administrative refunds or other scenarios not covered by the standard transaction codes.
void TransactionProcessing::processRefund(const std::string& buyerUsername, const std::string& sellerUsername, float amount) {
    // Verify both accounts exist
    if (!userAccounts.userExists(buyerUsername)) {
        std::cout << "Error: Buyer username does not exist." << std::endl;
        return;
    }
    if (!userAccounts.userExists(sellerUsername)) {
        std::cout << "Error: Seller username does not exist." << std::endl;
        return;
    }

    // ensure the amount isn't just 0
    if (amount <= 0) {
        std::cout << "Error: Invalid refund amount." << std::endl;
        return;
    }

    // if buyer and seller are same username return error
    if (buyerUsername == sellerUsername) {
        std::cout << "Error: Buyer and seller usernames cannot be the same." << std::endl;
        return;
    }

    // check if the seller is SellStandard user
    if (userAccounts.getCurrentUserType(sellerUsername) != UserType::SellStandard) {
        std::cout << "Error: Seller is not a standard seller." << std::endl;
        return;
    } 

    // Check if the seller has enough credit
    if (!userAccounts.hasSufficientCredit(sellerUsername, amount)) {
        std::cout << "Error: Seller does not have sufficient credit to cover the refund." << std::endl;
        return;
    }

    // Process the refund transaction
    userAccounts.addCredit(buyerUsername, amount); // Add the refund amount to the buyer's account
    userAccounts.deductCredit(sellerUsername, amount); // Deduct the refund amount from the seller's account

    //std::cout << "Refund of $" << amount << " from '" << sellerUsername << "' to '" << buyerUsername << "' processed successfully." << std::endl;
    
    // log this transaction to the daily transaction file
}

void TransactionProcessing::recordTransaction(const std::string& transaction) {
    transactionLogs.push_back(transaction);
}