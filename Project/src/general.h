#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Enumeration representing different user types
enum UserType
{
    Admin,        // Administrator
    FullStandard, // Full Standard User
    BuyStandard,  // Buy-Only Standard User
    SellStandard,  // Sell-Only Standard User
    AccountManager // Account Manager User
};

// Array of user type codes corresponding to the UserType enum
const char *userTypes[] = {"AA", "FS", "BS", "SS", "AM"};

// Function to map a user type code to the corresponding UserType enum value
int getUserTypeFromCode(std::string code)
{
    if (code == userTypes[Admin])
        return Admin;
    else if (code == userTypes[FullStandard])
        return FullStandard;
    else if (code == userTypes[BuyStandard])
        return BuyStandard;
    else if (code == userTypes[SellStandard])
        return SellStandard;
    else if (code == userTypes[AccountManager])
        return AccountManager;

    // Return an invalid value if the code doesn't match any known user type
    return -1;
}

// Function to map a UserType enum value to corresponding user type code
std::string getUserCodeFromType(int type)
{
    if (type == Admin)
        return userTypes[Admin];
    else if (type == FullStandard)
        return userTypes[FullStandard];
    else if (type == BuyStandard)
        return userTypes[BuyStandard];
    else if (type == SellStandard)
        return userTypes[SellStandard];
    else if (type == AccountManager)
        return userTypes[AccountManager];

    // Return an invalid value if the code doesn't match any known user type
    return "";
}

// Function to get a string of the user type
std::string getFullUserType(int type)
{
    if (type == Admin)
        return "Admin";
    else if (type == FullStandard)
        return "Full-Standard";
    else if (type == BuyStandard)
        return "Buy-Standard";
    else if (type == SellStandard)
        return "Sell-Standard";
    else if (type == AccountManager)
        return "Account-Manager";

    // Return an invalid value if the code doesn't match any known user type
    return "";
}

#endif
