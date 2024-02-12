#ifndef ADMINACTIONS_H
#define ADMINACTIONS_H

#include <string>
// Include other necessary components
#include "UserAccounts.h"
#include "TransactionProcessing.h"

class AdminActions {
private:
    UserAccounts& userAccounts;
    TransactionProcessing& transactionProcessing;

public:
    AdminActions(UserAccounts& userAccounts, TransactionProcessing& transactionProcessing);

    void createUser(const std::string& username, UserType userType, float initialCredit);
    void deleteUser(const std::string& username);
    void issueRefund(const std::string& buyerUsername, const std::string& sellerUsername, float amount);
    void addCredit(const std::string& username, float amount);
    void displayAllAccounts() const;

    // Add other administrative functions as necessary
};

#endif // ADMINACTIONS_H