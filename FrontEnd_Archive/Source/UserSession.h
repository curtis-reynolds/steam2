#ifndef USERSESSION_H
#define USERSESSION_H

#include <string>
#include <vector>
#include "UserAccounts.h"
#include "Types.h"

class UserSession {
private:
    bool loggedIn;
    std::string currentUser;
    UserType currentUserType;
    UserAccounts& userAccounts; 

public:
    std::vector<std::string> transactionLogs; // Store transaction logs here
    UserSession(UserAccounts& accounts);
    void login();
    std::string logout();
    bool isLoggedIn() const;
    std::string getCurrentUser() const;
    UserType getCurrentUserType() const;
    void recordTransaction(const std::string& transaction);
};

#endif // USERSESSION_H
