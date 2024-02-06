#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>

namespace Utilities {

// Splits a string into a vector of strings based on the given delimiter
std::vector<std::string> splitString(const std::string& str, char delimiter);

// Trims whitespace from the beginning and end of a string
std::string trim(const std::string& str);

// Checks if a string is a valid number
bool isNumber(const std::string& s);

// Converts a string to a float, with error handling
float stringToFloat(const std::string& s);

} // namespace Utilities

#endif // UTILITIES_H
