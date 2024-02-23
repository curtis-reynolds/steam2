#include <iostream>
#include <string>
// Include classes that handle different aspects of the application
#include "UserSession.h"
#include "UserAccounts.h"
#include "GameInventory.h"
#include "TransactionProcessing.h"
#include "AdminActions.h"
#include "Types.h"
#include <limits>
#include <cctype>

// Function prototype for displaying the main menu to the console.
void displayMenu() {
    std::cout << "\n===== Steam 2 - Menu =====\n";
    std::cout << "1. Login\n";
    std::cout << "2. Logout\n";
    std::cout << "3. Create User\n";
    std::cout << "4. Delete User (Admin only)\n";
    std::cout << "5. Add Game (Sell-standard or Admin only)\n";
    std::cout << "6. Purchase Game (Buy-standard or Admin only)\n";
    std::cout << "7. Add Credit\n";
    std::cout << "8. Refund\n";
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
    UserSession userSession; // Manages user session state, like login status.

    bool running = true; // Controls the main loop of the application.
    int choice; // Stores the user's menu choice
    std::string logoutMessage; // Stores the message returned by the logout action.

    // Main loop that keeps the application running until the user chooses to exit.
    while (running) {
        displayMenu(); // Displays the main menu options to the user.
        std::cin >> choice; // Reads the user's choice.
        // Clears the input buffer to prevent leftover input from affecting subsequent reads.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

        // Will prevent an infinite loop from occurring if the user enters a non-integer value.
        if (std::cin.fail()) {
            std::cin.clear(); // clear the error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard the input
            std::cout << "Invalid input, please try again." << std::endl;
            continue;
        }

        // Switch statement to handle the user's menu choice.
        switch (choice) {
            case 1: // Login
                userSession.login();
                break;
            case 2: // Logout
                logoutMessage = userSession.logout();
                std::cout << logoutMessage;
                break;
            case 3: {// Create User (Admin only)
                std::cout << "Create User selected.\n";
                std::string username;
                std::string userTypeStr;
                std::string initialCreditStr;
                float initialCredit;
                UserType userType;

                std::cout << "Enter username: ";
                std::getline(std::cin, username);
                std::cout << "Enter user type (Admin, FullStandard, BuyStandard, SellStandard): ";
                std::getline(std::cin, userTypeStr);
                std::cout << "Enter initial credit: ";
                std::cin >> initialCredit;
             //   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                // Attempt to convert initialCreditStr to float
                try {
                    initialCredit = std::stof(initialCreditStr);
                } catch (const std::invalid_argument& ia) {
                    std::cerr << "Invalid initial credit input, please enter a numeric value." << std::endl;
                    break; // Exit this case to avoid creating a user with an invalid credit
                } catch (const std::out_of_range& oor) {
                    std::cerr << "Initial credit input is out of range." << std::endl;
                    break;
                }

                for (auto& c : userTypeStr) c = std::tolower(c);

                if (userTypeStr == "admin") {
                    userType = UserType::Admin;
                } else if (userTypeStr == "fullstandard") {
                    userType = UserType::FullStandard;
                } else if (userTypeStr == "buystandard") {
                    userType = UserType::BuyStandard;
                } else if (userTypeStr == "sellstandard") {
                    userType = UserType::SellStandard;
                } else {
                    std::cerr << "Invalid user type specified. Please try again." << std::endl;
                    break; // Exit this case to avoid creating a user with an invalid type
                }

                // Check if the user is admin or has necessary privileges
                // Will be done later
                adminActions.createUser(username, userType, initialCredit);
                break;
            }
            case 4: { // Delete User (Admin only)
                std::string username;

                std::cout << "Enter username to delete: ";
                std::getline(std::cin, username);

                adminActions.deleteUser(username);
                break;
            }
            case 5: // Add Game (Sell-standard or Admin only)
                std::cout << "Add Game selected.\n";
                // transactionProcessing.processSellTransaction(...);
                break;
            case 6: // Purchase Game (Buy-standard or Admin only)
                std::cout << "Purchase Game selected.\n";
                // transactionProcessing.processBuyTransaction(...);
                break;
            case 7: // Add Credit
                std::cout << "Add Credit selected.\n";
                // transactionProcessing.processAddCreditTransaction(...);
                break;
            case 8: // Refund (Admin only)
                std::cout << "Refund selected.\n";
                // adminActions.issueRefund(...);
                break;
            case 9: { // Display All Accounts (Admin only)
                if (userSession.getCurrentUserType() == UserType::Admin) {
                    adminActions.displayAllAccounts();
                } else {
                    std::cout << "This action is only available to admin users.\n";
                }
                break;
            }
            case 10:
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
