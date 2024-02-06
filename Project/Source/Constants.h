#include <cstddef>
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {

// File paths
const char* const USER_ACCOUNTS_FILE = "user_accounts.txt";
const char* const GAME_INVENTORY_FILE = "game_inventory.txt";
const char* const TRANSACTION_LOG_FILE = "transaction_log.txt";

// User constraints
const size_t MAX_USERNAME_LENGTH = 15;
const float MAX_USER_CREDIT = 999999.99f;

// Game constraints
const size_t MAX_GAME_NAME_LENGTH = 25;
const float MAX_GAME_PRICE = 999.99f;

// Transaction codes
const char* const LOGIN = "login";
const char* const LOGOUT = "logout";
const char* const CREATE = "create";
const char* const DELETE = "delete";
const char* const SELL = "sell";
const char* const BUY = "buy";
const char* const REFUND = "refund";
const char* const ADD_CREDIT = "addcredit";

// Add other constants as needed

} // namespace Constants

#endif // CONSTANTS_H
