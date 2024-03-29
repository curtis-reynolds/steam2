#include <iostream>
#include <string>
#include <unordered_map>
#include <limits>
#include <cctype>
#include <unordered_map>
// Include classes that handle different aspects of the application
#include "UserSession.h"
#include "UserAccounts.h"
#include "GameInventory.h"
#include "TransactionProcessing.h"
#include "AdminActions.h"
#include "Types.h"

// Helper function to convert a string to lowercase
std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    for (auto& c : lowerStr) {
        c = std::tolower(c);
    }
    return lowerStr;
}

// Function prototype for displaying the main menu to the console.
void displayMenu() {
    std::cout << "\n===== Steam 2 - Menu =====\n";
    std::cout << "1. Login\n";
    std::cout << "2. Logout\n";
    std::cout << "3. Create User (Admin only)\n";
    std::cout << "4. Delete User (Admin only)\n";
    std::cout << "5. Sell Game (Sell-standard or Admin only)\n";
    std::cout << "6. Buy Game (Buy-standard or Admin only)\n";
    std::cout << "7. Add Credit\n";
    std::cout << "8. Refund (Admin Only)\n";
    std::cout << "9. Display All Accounts (Admin only)\n"; 
    std::cout << "10. Exit\n";
    std::cout << "==========================================\n";
    std::cout << "Select an option: ";
}

// Main function - entry point of the application
int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " user_accounts.txt availablegames.txt gamescollection.txt transout.atf\n";
        return 1;
    }

    std::string currentAccountsFile = argv[1];
    std::string availableGamesFile = argv[2];
    std::string gamesCollectionFile = argv[3];
    std::string transactionOutputFile = argv[4];

    // Initialize components with command line arguments
    UserAccounts userAccounts(currentAccountsFile);
    GameInventory gameInventory(availableGamesFile); 
    TransactionProcessing transactionProcessing(userAccounts, gameInventory); // Processes transactions.
    AdminActions adminActions(userAccounts, transactionProcessing); // Executes admin-specific actions.
    UserSession userSession(userAccounts); // Manages user session state, like login status.

    bool running = true; // Controls the main loop of the application.
    int choice; // Stores the user's menu choice
    std::string logoutMessage; // Stores the message returned by the logout action.

    // Main loop that keeps the application running until the user chooses to exit.
    while (!std::cin.eof() && running) {
        //displayMenu(); // Displays the main menu options to the user. Commented out for now
        std::string input;
        std::getline(std::cin, input); // Read the line of text

        // Check if the input is empty (blank line)
        if (input.empty()) {
            continue; // Skip to the next iteration of the loop
        }

        // Map of string commands to choice numbers
        std::unordered_map<std::string, int> commandMap = {
            {"login", 1},
            {"logout", 2},
            {"create user", 3},
            {"create", 3},
            {"delete user", 4},
            {"delete", 4},
            {"sell game", 5},
            {"sell", 5},
            {"buy game", 6},
            {"buy", 6},
            {"add credit", 7},
            {"refund", 8},
            {"display all accounts", 9},
            {"exit", 10}
        };

        // Check if the input is a known command
        std::string lowerInput = toLower(input); // Convert input to lowercase for case-insensitive comparison
        if (commandMap.find(lowerInput) != commandMap.end()) {
            choice = commandMap[lowerInput]; // Set choice based on the command
        } else {
            // Attempt to parse the input as a number
            try {
                choice = std::stoi(input); // Convert string to integer
            } catch (const std::invalid_argument& e) {
                // Handle the case where the input is not a valid number or command
                std::cout << "Invalid input, please try again." << std::endl;
                continue;
            }
        }

        // Will prevent an infinite loop from occurring if the user enters a non-integer value.
        if (std::cin.fail()) {
            std::cin.clear(); // clear the error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard the input
            std::cout << "Invalid input, please try again." << std::endl;
            continue;
        }
        
        // Ensure user is logged in for any option other than "Login" and "Exit"
        if (!userSession.isLoggedIn() && choice != 1 && choice != 10) {
            std::cout << "Please login to access this feature." << std::endl;
            continue; // Skip to the next iteration of the loop, forcing the user to login or exit
        }

        // Switch statement to handle the user's menu choice.
        switch (choice) {
            case 1: { // Login
                if (userSession.isLoggedIn()) { // If the user is already logged in, do not log them in again
                    std::cout << "Error: A user is already logged in. Please logout before trying to login again." << std::endl;
                } else {
                    userSession.login();
                }
                break;
            }
            case 2: // Logout
                logoutMessage = userSession.logout();
                std::cout << logoutMessage;
                break;
            case 3: {// Create User (Admin only)
                // First, check if the current user is an admin
                if (userSession.getCurrentUserType() != UserType::Admin) {
                    std::cout << "Error: Only admin users can create new accounts." << std::endl;
                    break; // Exit this case since the current user is not an admin
                }

                std::string username;
                std::string userTypeStr;
                std::string initialCreditStr;
                float initialCredit;
                UserType userType;

                // std::cout << "Enter username: ";
                std::getline(std::cin, username);
                // std::cout << "Enter user type (Admin, FullStandard, BuyStandard, SellStandard): ";
                std::getline(std::cin, userTypeStr);
                // std::cout << "Enter initial credit: ";
                std::getline(std::cin, initialCreditStr);

                // Attempt to convert initialCreditStr to float
                try {
                    initialCredit = std::stof(initialCreditStr);
                } catch (const std::invalid_argument& ia) {
                    std::cout << "Invalid initial credit input, please enter a numeric value." << std::endl;
                    break; // Exit this case to avoid creating a user with an invalid credit
                } catch (const std::out_of_range& oor) {
                    std::cout << "Initial credit input is out of range." << std::endl;
                    break;
                }

                for (auto& c : userTypeStr) c = std::tolower(c);

                if (userTypeStr == "admin") {
                    userType = UserType::Admin;
                } else if (userTypeStr == "fullstandard" || userTypeStr == "full standard") {
                    userType = UserType::FullStandard;
                } else if (userTypeStr == "buystandard" || userTypeStr == "buy standard") {
                    userType = UserType::BuyStandard;
                } else if (userTypeStr == "sellstandard" || userTypeStr == "sell standard") {
                    userType = UserType::SellStandard;
                } else {
                    std::cout << "Invalid user type specified. Please try again." << std::endl;
                    break; // Exit this case to avoid creating a user with an invalid type
                }

                adminActions.createUser(username, userType, initialCredit);
                break;
            }
            case 4: { // Delete User (Admin only)
                // First check if the current user is an admin
                if (userSession.getCurrentUserType() != UserType::Admin) {
                    std::cout << "Error: Only admin users can delete accounts." << std::endl;
                    break; // Exit this case since the current user is not an admin
                }
                std::string username;

                // std::cout << "Enter username to delete: ";
                std::getline(std::cin, username);
                // Check if the user is attempting to delete their own account in the same session
                if (username == userSession.getCurrentUser()) {
                    std::cout << "Error: You cannot delete your own account." << std::endl;
                    break; // Exit this case to avoid deleting the user's own account
                }

                adminActions.deleteUser(username);
                break;
            }
            case 5: { // Add Game (Sell-standard or Admin only)
                if (userSession.getCurrentUserType() == UserType::SellStandard || userSession.getCurrentUserType() == UserType::Admin) {
                    std::vector<std::string> args;
                    std::string gameName, sellerUsername, priceStr;

                    // std::cout << "Enter game name: ";
                    std::getline(std::cin, gameName);
                    args.push_back(gameName);

                    // Assume sellerUsername is obtained from the current session
                    sellerUsername = userSession.getCurrentUser();
                    args.push_back(sellerUsername);

                    // std::cout << "Enter price: ";
                    std::getline(std::cin, priceStr);
                    args.push_back(priceStr);

                    // Now, call processSellTransaction with args
                    transactionProcessing.processSellTransaction(args);
                } else {
                    std::cout << "This action is only available to sell-standard or admin users.\n";
                    break;
                }
                break;
            }
            case 6: { // Purchase Game (Buy-standard or Admin only)
                UserType currentUserType = userSession.getCurrentUserType();
                
                // Check if the user is allowed to purchase (not SellStandard)
                if (currentUserType == UserType::SellStandard) {
                    std::cout << "Error: Sell-standard accounts are not eligible to purchase games." << std::endl;
                    break;
                }

                // Prompt for game name and seller's username
                std::string gameName, sellerUsername;
                // std::cout << "Enter the name of the game you wish to purchase: ";
                std::getline(std::cin, gameName);
                // std::cout << "Enter the seller's username: ";
                std::getline(std::cin, sellerUsername);

                // Retrieve the buyer's username from the session
                std::string buyerUsername = userSession.getCurrentUser();

                // Prepare the arguments for processBuyTransaction
                std::vector<std::string> args = {buyerUsername, gameName, sellerUsername};

                // Execute the purchase transaction
                transactionProcessing.processBuyTransaction(args);

                break;
            }
            case 7: { // Add Credit
                std::vector<std::string> args;
                if (userSession.getCurrentUserType() == UserType::Admin) {
                    std::string username;
                    // std::cout << "Enter username: ";
                    std::getline(std::cin, username);
                    args.push_back(username);
                    std::string amountStr;
                    // std::cout << "Enter amount of credit to add: ";
                    std::getline(std::cin, amountStr);
                    args.push_back(amountStr);
                        
                    transactionProcessing.processAddCreditTransaction(args);
                    break;
                }
                std::string username = userSession.getCurrentUser();
                args.push_back(username);
                std::string amountStr;
                // std::cout << "Enter amount of credit to add: ";
                std::getline(std::cin, amountStr);
                args.push_back(amountStr);

                transactionProcessing.processAddCreditTransaction(args);
                break;
            }
            case 8:{ // Refund (Admin only)
                // Ensure the user is an admin before allowing a refund
                if (userSession.getCurrentUserType() != UserType::Admin) {
                    std::cout << "This action is only available to admin users.\n";
                    break;
                }
                // Ask for the buyer's username
                std::string buyerUsername;
                // std::cout << "Enter the buyer's username: ";
                std::getline(std::cin, buyerUsername);

                // Ask for the seller's username
                std::string sellerUsername;
                // std::cout << "Enter the seller's username: ";
                std::getline(std::cin, sellerUsername);

                // Ask for the amount of credit to transfer
                std::string amountStr;
                // std::cout << "Enter the amount of credit to transfer: ";
                std::getline(std::cin, amountStr);
                float amount;
                try {
                    amount = std::stof(amountStr);
                } catch (const std::invalid_argument& ia) {
                    std::cout << "Invalid input for amount. Please enter a numeric value." << std::endl;
                    break; // Exit this case to avoid further processing with invalid input
                } catch (const std::out_of_range& oor) {
                    std::cout << "Amount is out of range." << std::endl;
                    break; // Exit this case to avoid further processing with invalid input
                }

                // Call issueRefund with the collected information
                adminActions.issueRefund(buyerUsername, sellerUsername, amount);

                break;
            }
            case 9: { // Display All Accounts (Admin only)
                if (userSession.getCurrentUserType() == UserType::Admin) {
                    adminActions.displayAllAccounts();
                } else {
                    std::cout << "This action is only available to admin users.\n";
                }
                break;
            }
            case 10:
                // Ensure the user is not logged in before exiting
                if (userSession.isLoggedIn()) {
                    std::cout << "Please logout before exiting the application." << std::endl;
                    break;
                }
                running = false;
                std::cout << "Exiting Steam 2...\n";
                break;
            default:
                std::cout << "Invalid option, please try again." << std::endl;
                break;
        }
    }
    return 0;
}
