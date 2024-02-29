#ifndef TRANSACTIONPROCESSING_H
#define TRANSACTIONPROCESSING_H

#include <string>
#include <vector>
// Include other necessary components
#include "UserAccounts.h"
#include "GameInventory.h"

class TransactionProcessing {
private:
    // References to other system components
    UserAccounts& userAccounts;
    GameInventory& gameInventory;
    

public:
    TransactionProcessing(UserAccounts& userAccounts, GameInventory& gameInventory);
    void processTransaction(const std::string& transactionCode, const std::vector<std::string>& args);
    void processRefund(const std::string& buyerUsername, const std::string& sellerUsername, float amount);
    std::vector<std::string> transactionLogs;

    // Individual transaction processing methods
    void processSellTransaction(const std::vector<std::string>& args);
    void processBuyTransaction(const std::vector<std::string>& args);
    void processRefundTransaction(const std::vector<std::string>& args);
    void processAddCreditTransaction(const std::vector<std::string>& args);
    void recordTransaction(const std::string& transaction);
};

#endif // TRANSACTIONPROCESSING_H
