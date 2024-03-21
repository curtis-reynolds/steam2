#include <iostream>
#include <vector>
#include "TransactionHandler.h"
#include "FileReader.h"
#include "User.h"
#include "SharedData.h"

// Updated to use command-line arguments
int main(int argc, char *argv[])
{
    // Check if the correct number of arguments is passed
    if (argc < 5)
    {
        std::cerr << "Usage: " << argv[0] << " <users_filename>" << std::endl;
        return 1; // Return with error code
    }

    // Assign the users file name from command-line arguments
    std::string currentAccountsFilename = argv[1];
    std::string availableGamesFilename = argv[2];
    std::string gamesCollectionFilename = argv[3];
    std::string transactionsOutFilename = argv[4];

    // Create an instance of SharedData to manage shared data
    SharedData sharedData;

    // Create an instance of TransactionHandler, providing SharedData and the filename for user data
    TransactionHandler handler(sharedData, currentAccountsFilename, availableGamesFilename, gamesCollectionFilename, transactionsOutFilename);

    // Main program loop
    while (true)
    {
        std::string transactionCode;

        // Read transaction code from the user
        std::cout << "Enter transaction code (or 'exit' to quit): ";
        std::cin >> transactionCode;

        // Check for exit condition
        if (transactionCode == "exit")
        {
            break; // Exit the loop and end the program
        }

        // Handle transactions using the handler
        handler.handleTransaction(transactionCode);
    }

    return 0; // End the program
}
