#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "general.h"
#include "UserUpdater.h"
#include "FileReader.h"
#include "CreditUpdater.h"
#include "SharedData.h"

struct refundResult
{
    std::string buyerUsername;
    std::string sellerUsername;
    double creditAmount;
};

class UserManager
{
public:
    // Constructor that takes a SharedData reference and a filename for user data
    UserManager(SharedData &sharedData, const std::string &userFilename)
        : userUpdater(userFilename),
          creditUpdater(sharedData, userFilename),
          fileReader(userFilename),
          sharedData(sharedData),
          users(sharedData.getUsers()),
          currentUser(sharedData.getCurrentUser())
    {
        // Attempt to open the file for reading and read user data
        if (fileReader.openFile())
        {
            fileReader.readUsers(users);
            fileReader.closeFile();
        }
        else
        {
            std::cerr << "Error: Unable to open the file for reading." << std::endl;
        }
    }

    // Function to create a new user
    User createUser()
    {
        std::string username = getUsername();
        int userType = getType();

        // Create a new user object with the provided information
        User newUser(username, userType, 0.0);

        // Add the new user to the vector of users
        users.push_back(newUser);

        // Open the file and write the new user information
        if (userUpdater.openFile())
        {
            userUpdater.addUser(newUser);
            userUpdater.closeFile();
            std::cout << "User created successfully." << std::endl;
        }
        else
        {
            std::cerr << "Error: Unable to open the file for writing." << std::endl;
        }

        return newUser;
    }

    // Function to delete a user
    User deleteUser()
    {
        // Check if the current user is an admin
        if (currentUser.getType() != Admin)
        {
            std::cout << "User unauthorized to perform this action." << std::endl;
            // Return a default-constructed User object to indicate an error
            return User("", 0, 0.0);
        }

        // Get the username to be deleted
        std::string usernameToDelete;
        std::cout << "Enter the username to delete: ";
        std::cin >> usernameToDelete;

        // Check if the provided username exists and is not the current user's username
        auto userToDelete = std::find_if(users.begin(), users.end(), [usernameToDelete](const User &user)
                                         { return user.getUsername() == usernameToDelete; });

        if (userToDelete == users.end() || usernameToDelete == currentUser.getUsername())
        {
            std::cout << "Invalid username or attempting to delete the current user account." << std::endl;
            // Return a default-constructed User object to indicate an error
            return User("", 0, 0.0);
        }

        // Remove the user account
        User deletedUser = *userToDelete;

        if (userUpdater.openFile())
        {
            userUpdater.deleteUser(deletedUser);
            userUpdater.closeFile();
        }
        else
        {
            std::cerr << "Error: Unable to open the file for writing." << std::endl;
            return User("", 0, 0.0);
        }

        users.erase(userToDelete);

        std::cout << "User deleted successfully." << std::endl;

        return deletedUser;
    }

    // Function to process a refund
    refundResult refund()
    {
        std::string buyerUsername;
        std::string sellerUsername;
        double creditAmount;

        // Get the buyer's username from the user
        std::cout << "Enter the buyer's username: ";
        std::cin.ignore(); // Ignore the newline character from the previous input
        std::getline(std::cin, buyerUsername);

        // Get the seller's username from the user
        std::cout << "Enter the seller's username: ";
        std::getline(std::cin, sellerUsername);

        // Find the buyer and seller users
        User *buyer = sharedData.getUserByUsername(buyerUsername);
        User *seller = sharedData.getUserByUsername(sellerUsername);

        // Check if the buyer and seller exist
        if (buyer == nullptr || seller == nullptr)
        {
            std::cout << "Error: Buyer or seller does not exist." << std::endl;
            return {"", "", 0.0};
        }

        // Get the amount of credit to transfer
        std::cout << "Enter the amount of credit to transfer: ";
        std::cin >> creditAmount;

        // Check if the seller has enough credit
        if (999999.99 < creditAmount)
        {
            std::cout << "Error: The maximum amount of credit to refund is $999,999.99." << std::endl;
            return {"", "", 0.0};
        }

        // Check if the seller has enough credit
        if (seller->getCredit() < creditAmount)
        {
            std::cout << "Error: The seller does not have enough credit to refund." << std::endl;
            return {"", "", 0.0};
        }

        // Transfer credit from seller to buyer
        double buyerNewCredit = buyer->getCredit() + creditAmount;
        double sellerNewCredit = seller->getCredit() - creditAmount;

        // Update credits
        creditUpdater.updateCreditForUser(buyer, buyerNewCredit);
        creditUpdater.updateCreditForUser(seller, sellerNewCredit);

        std::cout << "Refund successful. Transferred " << creditAmount << " credits from " << seller->getUsername()
                  << " to " << buyer->getUsername() << "." << std::endl;

        return {buyerUsername, sellerUsername, creditAmount};
    }

    // Function to add credit to a user account
    User *addCredit()
    {
        std::string username;
        double creditAmount;

        // Get the username from the user
        std::cout << "Enter the username: ";
        std::cin.ignore(); // Ignore the newline character from the previous input
        std::getline(std::cin, username);

        // Check if the username is valid
        User *user = sharedData.getUserByUsername(username);
        if (user == nullptr)
        {
            std::cout << "Error: Username does not exist in the system." << std::endl;
            return nullptr;
        }

        // Get the amount of credit to add
        std::cout << "Enter the amount of credit to add: ";
        std::cin >> creditAmount;

        // Check if the credit amount is valid
        if (creditAmount > 1000.00)
        {
            std::cout << "Error: Maximum $1000.00 can be added to an account in a given session." << std::endl;
            return nullptr;
        }

        // Update the user's credit
        double newCredit = user->getCredit() + creditAmount;
        creditUpdater.updateCreditForUser(user, newCredit);

        std::cout << "Credit added successfully. New credit for user " << username << ": " << newCredit << std::endl;

        return user;
    }

    void listUsers()
    {
        // Display header with column names
        std::cout << "User Information:" << std::endl;
        std::cout << std::setw(20) << std::left << "Username"
                  << std::setw(15) << std::left << "User Type"
                  << std::setw(10) << std::left << "Credit" << std::endl;

        // Iterate through each user
        for (const User &user : users)
        {
            // Display user information
            std::cout << std::setw(20) << std::left << user.getUsername()
                      << std::setw(15) << std::left << getFullUserType(user.getType())
                      << std::setw(10) << std::fixed << std::setprecision(2) << user.getCredit() << std::endl;

            // Add a divider line between each user
            std::cout << std::string(45, '-') << std::endl;
        }
    }

private:
    // Reference to the current user in shared data
    User &currentUser;

    // Reference to the vector of users in shared data
    std::vector<User> &users;

    // UserUpdater and FileReader objects for handling file operations
    UserUpdater userUpdater;
    FileReader fileReader;

    // CreditUpdater instance for handling credit-related operations
    CreditUpdater creditUpdater;

    // Reference to the shared data object
    SharedData &sharedData;

    // Function to get a valid username from the user
    std::string getUsername()
    {
        std::string username;
        while (true)
        {
            std::cout << "Enter username: ";
            std::cin >> username;

            if (isUsernameValid(username))
                break;
        }

        return username;
    }

    // Function to get a valid user type from the user
    int getType()
    {
        int userType;
        while (true)
        {
            std::cout << "Enter user type (1-admin, 2-full-standard, 3-buy-standard, 4-sell-standard): ";
            std::cin >> userType;

            // If invalid input, clear the input buffer and try again
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
            }

            // If the input is a valid user type, break out of the loop
            if (userType == 1 || userType == 2 || userType == 3 || userType == 4)
                break;
        }

        return userType;
    }

    // Function to check if a username is valid
    bool isUsernameValid(std::string &username)
    {
        // Check the length of the username
        if (username.length() > 15)
        {
            std::cout << "Username length should be less than 15 characters" << std::endl;
            return false;
        }

        // Check if the username contains underscores
        if (username.find('_') != std::string::npos)
        {
            std::cout << "Username should not contain underscores" << std::endl;
            return false;
        }

        // Check if the username is already taken
        for (const User &user : users)
        {
            if (user.getUsername() == username)
            {
                std::cout << "This username is already taken" << std::endl;
                return false;
            }
        }

        // If all checks pass, the username is valid
        return true;
    }
};

#endif
