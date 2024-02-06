#ifndef USERSESSION_H
#define USERSESSION_H

#include <string>
#include "Types.h"

class UserSession {
private:
    bool loggedIn;
    std::string currentUser;
    UserType currentUserType;

public:
    UserSession();
    void login();
    void logout();
    bool isLoggedIn() const;
    std::string getCurrentUser() const;
    UserType getCurrentUserType() const;
};

#endif // USERSESSION_H
