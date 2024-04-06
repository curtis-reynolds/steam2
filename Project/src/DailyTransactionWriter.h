#ifndef DAILY_TRANSACTION_WRITER_H
#define DAILY_TRANSACTION_WRITER_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

class DailyTransactionWriter
{
public:
    // Constructor that takes the filename as a parameter
    DailyTransactionWriter(const std::string &filename) : dailyTransactionFilename(filename) {}

    // Function to add a transaction to the daily transactions vector
    void addTransaction(const std::string &transactionString)
    {
        dailyTransactions.push_back(transactionString);
    }

    // Function to add a refund transaction to the daily transactions vector
    void addUserTransaction(const std::string &transactionCode, User &user)
    {
        std::string username = user.getUsername();
        username.resize(16, ' '); // Ensure the username is 16 characters long

        std::string userCode = getUserCodeFromType(user.getType());

        // Format the credit with leading zeros and ".00" suffix
        std::string formattedCredit = formatCredit(user.getCredit(), 9);

        std::ostringstream transactionFields;
        transactionFields << std::setw(16) << std::left << username
                          << std::setw(2) << std::left << userCode << " "
                          << formattedCredit << "\n";

        std::string transactionString = transactionCode + " " + transactionFields.str();

        addTransaction(transactionString);
    }

    // Function to add a refund transaction to the daily transactions vector
    void addRefundTransaction(std::string &buyerUsername, std::string &sellerUsername, double refundCredit)
    {
        sellerUsername.resize(15, '_'); // Ensure the seller username is 15 characters long
        buyerUsername.resize(15, '_');  // Ensure the buyer username is 15 characters long

        // Format the credit with leading zeros and ".00" suffix
        std::string formattedCredit = formatCredit(refundCredit, 9);

        std::ostringstream transactionFields;
        transactionFields << std::setw(15) << std::left << sellerUsername << " "
                          << std::setw(15) << std::left << buyerUsername << " "
                          << formattedCredit << "\n";

        std::string transactionString = "05 " + transactionFields.str();

        addTransaction(transactionString);
    }

    // Function to add a sell transaction to the daily transactions vector
    void addSellTransaction(Game &game)
    {
        std::string sellerUsername = game.getSellerName();
        sellerUsername.resize(13, ' '); // Ensure the seller username is 13 characters long

        std::string gameName = game.getGameName();
        gameName.resize(19, ' '); // Ensure the game name is 19 characters long

        // Format the credit with leading zeros and ".00" suffix
        std::string formattedCredit = formatCredit(game.getPrice(), 6);

        std::ostringstream transactionFields;
        transactionFields << std::setw(19) << std::left << gameName << " "
                          << std::setw(13) << std::left << sellerUsername << " "
                          << formattedCredit << "\n";

        std::string transactionString = "03 " + transactionFields.str();

        addTransaction(transactionString);
    }

    // Function to add a buy transaction to the daily transactions vector
    void addBuyTransaction(Game &game, std::string &buyerUsername)
    {
        std::string sellerUsername = game.getSellerName();
        sellerUsername.resize(15, ' '); // Ensure the seller username is 15 characters long

        std::string gameName = game.getGameName();
        gameName.resize(19, ' '); // Ensure the game name is 19 characters long

        buyerUsername.resize(14, ' '); // Ensure the buyer username is 14 characters long

        // Format the credit with leading zeros and ".00" suffix
        std::string formattedCredit = formatCredit(game.getPrice(), 6);

        std::ostringstream transactionFields;
        transactionFields << std::setw(19) << std::left << gameName << " "
                          << std::setw(15) << std::left << sellerUsername << " "
                          << std::setw(14) << std::left << buyerUsername << " "
                          << formattedCredit << "\n";

        std::string transactionString = "04 " + transactionFields.str();

        addTransaction(transactionString);
    }

    // Function to write the daily transaction file
    void writeDailyTransactionFile(User &user)
    {
        // Add end of session transaction
        addUserTransaction("00", user);

        // Open the daily transaction file in append mode
        std::ofstream dailyTransactionFile = openFile();

        // Write each transaction from the dailyTransactions vector to the file
        for (const std::string &transaction : dailyTransactions)
        {
            dailyTransactionFile << transaction;
        }

        // Close the file
        closeFile(dailyTransactionFile);
    }

private:
    // Member variable to store the daily transaction filename
    std::string dailyTransactionFilename;

    // Vector to store daily transactions
    std::vector<std::string> dailyTransactions;

    // Function to open the daily transaction file in append mode
    std::ofstream openFile()
    {
        // Open the daily transaction file in append mode
        std::ofstream fileStream(dailyTransactionFilename, std::ios::app);
        if (!fileStream.is_open())
        {
            std::cerr << "Error: Unable to open the daily transaction file for writing." << std::endl;
        }
        return fileStream;
    }

    // Function to close the daily transaction file
    void closeFile(std::ofstream &fileStream)
    {
        // Close the daily transaction file
        if (fileStream.is_open())
        {
            fileStream.close();
        }
    }

    std::string formatCredit(double credit, int length)
    {
        // Format the credit with leading zeros and ".00" suffix
        std::ostringstream formattedCreditStream;
        formattedCreditStream << std::fixed << std::setw(length) << std::setfill('0') << std::setprecision(2) << credit;
        std::string formattedCredit = formattedCreditStream.str();

        return formattedCredit;
    }
};

#endif
