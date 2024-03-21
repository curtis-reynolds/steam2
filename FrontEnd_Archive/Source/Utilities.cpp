#include "Utilities.h"
#include <algorithm>
#include <iostream>
#include <sstream>

// Define the Utilities namespace to contain all utility functions.
namespace Utilities {

// Splits a string into a vector of strings based on a specified delimiter.
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    // Initialize a vector to hold the split parts of the string.
    std::vector<std::string> tokens;
    // Use a string stream to facilitate the splitting process.
    std::istringstream tokenStream(str);
    std::string token;
    // Extract each part of the string separated by the delimiter and add it to the vector.
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens; // Return the vector of split strings.
}

// Trims leading and trailing whitespace from a string.
std::string trim(const std::string& str) {
    // Find the first character position after excluding leading spaces.
    size_t first = str.find_first_not_of(' ');
    // Return the original string if no non-space character is found.
    if (std::string::npos == first) {
        return str;
    }
    // Find the last character position before excluding trailing spaces.
    size_t last = str.find_last_not_of(' ');
    // Return a substring that excludes leading and trailing spaces.
    return str.substr(first, (last - first + 1));
}

// Checks if a given string represents a valid number.
bool isNumber(const std::string& s) {
    // Check if the string is not empty and all characters are digits.
    // std::find_if looks for the first character that is not a digit.
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) {
        return !std::isdigit(c);
    }) == s.end(); // True if all characters are digits, false otherwise.
}

// Converts a string to a floating-point number and handles possible exceptions.
float stringToFloat(const std::string& s) {
    try {
        // Attempt to convert the string to float using std::stof.
        return std::stof(s);
    } catch (const std::invalid_argument& e) {
        // Catch and handle the case where the string cannot be converted to a float.
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        // Catch and handle the case where the converted float is out of range.
        std::cerr << "Out of range: " << e.what() << std::endl;
    }
    return 0.0f; // Return 0.0f by default if conversion fails.
}

} // namespace Utilities
