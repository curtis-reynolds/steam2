#include "TransactionProcessing.h"
#include <iostream>
// TransactionProcessing class relies on UserAccounts and GameInventory to process transactions.
#include "UserAccounts.h"
#include "GameInventory.h"
#include "GameCollection.h"
#include "UserSession.h"

UserSession userSession;

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
        std::cerr << "Error: Invalid price format." << std::endl;
        return;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Price is out of range." << std::endl;
        return;
    }

    // Add the game to the inventory
    gameInventory.addGame(gameName, price, sellerUsername);
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
        std::cerr << "Error: The specified seller does not exist." << std::endl;
        return;
    }

    // Validate that the game exists and is available for sale
    if (!gameInventory.gameExists(gameName)) {
        std::cerr << "Error: The specified game is not available for sale by the user " << sellerUsername << "." << std::endl;
        return;
    }

    float gamePrice = gameInventory.getGamePrice(gameName, sellerUsername);

    // Check if the buyer has sufficient credit
    if (!userAccounts.hasSufficientCredit(buyerUsername, gamePrice)) {
        std::cerr << "Error: The buyer does not have sufficient credit to complete the purchase." << std::endl;
        return;
    }

    // Check if the buyer already owns the game
    if (gameCollection.ownsGame(buyerUsername, gameName)) {
        std::cerr << "Error: The buyer already owns a copy of this game." << std::endl;
        return;
    }

    // Process the transaction: Update credits and transfer game ownership
    userAccounts.processPurchase(buyerUsername, sellerUsername, gamePrice);
    gameCollection.addGameToUser(buyerUsername, gameName);

    std::cout << "Purchase successful. " << gameName << " has been added to " << buyerUsername << "'s collection." << std::endl;
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
    float amount = std::stof(args[1]);

    if (amount <= 0 || amount > 1000) {
        std::cerr << "Error: Invalid amount. Must be between $0 and $1000." << std::endl;
        return;
    }

    // Assuming userAccounts has a method for adding credit
    userAccounts.addCredit(username, amount);
    std::cout << "Successfully added $" << amount << " to " << username << "'s account." << std::endl;

    // Save transaction to the daily transaction file
    // This part requires implementation based on how you're handling transaction logging
}


// A specific method for processing refunds not triggered by the main transaction processing method.
// This could be used for administrative refunds or other scenarios not covered by the standard transaction codes.
void TransactionProcessing::processRefund(const std::string& buyerUsername, const std::string& sellerUsername, float amount) {
    // TODO: Implement logic to process a refund transaction
    // This could involve verifying both accounts exist, adjusting their credits, etc.
    std::cout << "Processing refund from '" << sellerUsername << "' to '" << buyerUsername << "' for $" << amount << std::endl;
}

