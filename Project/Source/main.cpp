#include <iostream>
#include <string>
#include "UserSession.h"
#include "UserAccounts.h"
#include "GameInventory.h"
#include "TransactionProcessing.h"
#include "AdminActions.h"
#include "Types.h"
#include <limits>

void displayMenu() {
    std::cout << "\n===== Steam 2 - Menu =====\n";
    std::cout << "1. Login\n";
    std::cout << "2. Logout\n";
    std::cout << "3. Create User (Admin only)\n";
    std::cout << "4. Delete User (Admin only)\n";
    std::cout << "5. Add Game (Sell-standard or Admin only)\n";
    std::cout << "6. Purchase Game (Buy-standard or Admin only)\n";
    std::cout << "7. Add Credit\n";
    std::cout << "8. Refund (Admin only)\n";
    std::cout << "9. Exit\n";
    std::cout << "==========================================\n";
    std::cout << "Select an option: ";
}

int main() {
    // Initialization of necessary components
    UserAccounts userAccounts;
    GameInventory gameInventory;
    TransactionProcessing transactionProcessing(userAccounts, gameInventory);
    AdminActions adminActions(userAccounts, transactionProcessing);
    UserSession userSession;

    bool running = true;
    int choice;

    while (running) {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

        switch (choice) {
            case 1: // Login
                userSession.login();
                break;
            case 2: // Logout
                userSession.logout();
                break;
            case 3: {// Create User (Admin only)
                std::cout << "Create User selected.\n";
                std::string username;
                std::string userTypeStr;
                float initialCredit;
                UserType userType;

                std::cout << "Enter username: ";
                std::getline(std::cin, username);
                std::cout << "Enter user type (Admin, FullStandard, BuyStandard, SellStandard): ";
                std::getline(std::cin, userTypeStr);
                std::cout << "Enter initial credit: ";
                std::cin >> initialCredit;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (userTypeStr == "Admin") {
                    userType = UserType::Admin;
                } else if (userTypeStr == "FullStandard") {
                    userType = UserType::FullStandard;
                } else if (userTypeStr == "BuyStandard") {
                    userType = UserType::BuyStandard;
                } else if (userTypeStr == "SellStandard") {
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
            case 9: // Exit
                running = false;
                std::cout << "Exiting Steam 2..." << std::endl;
                break;
            default:
                std::cout << "Invalid option, please try again." << std::endl;
        }
    }
    return 0;
}
