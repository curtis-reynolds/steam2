#ifndef CREDIT_UPDATER_H
#define CREDIT_UPDATER_H

#include "User.h"
#include "UserUpdater.h"
#include "SharedData.h"

class CreditUpdater
{
public:
    // Constructor that takes a filename for user data
    CreditUpdater(SharedData &sharedData, const std::string &userFilename) : userUpdater(userFilename), sharedData(sharedData) {}

    // Function to update the credit for a user
    void updateCreditForUser(const User *user, double newCredit)
    {
        // Use UserUpdater to update credit in the file
        if (userUpdater.openFile())
        {
            userUpdater.updateUserCredit(*user, newCredit);
            userUpdater.closeFile();
        }
        else
        {
            std::cerr << "Error: Unable to open the file for writing." << std::endl;
        }
        updateCreditInSharedData(user->getUsername(), newCredit);
    }

private:
    SharedData &sharedData;
    // UserUpdater instance for handling file operations
    UserUpdater userUpdater;

    // Function to update the credit in the SharedData object
    void updateCreditInSharedData(const std::string &username, double newCredit)
    {
        // Get the vector of users from SharedData
        std::vector<User> &users = sharedData.getUsers();

        // Find the user in the vector and update the credit
        for (User &user : users)
        {
            if (user.getUsername() == username)
            {
                user.setCredit(newCredit);
                break;
            }
        }
    }
};

#endif
