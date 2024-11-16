#ifndef DATABASE_H
#define DATABASE_H

#include <string>

// Function to save an item to the fridge inventory in the database
bool saveFridgeItem(int userId, const std::string& name, int quantity, const std::string& unit, const std::string& expirationDate);

// Function to retrieve fridge items for a specific user from the database
// (Assume we use a vector of items or some other container to hold results; here we’ll declare it simply)
bool getFridgeItems(int userId, std::vector<std::string>& items);  // Example, replace std::vector<std::string> with actual type

// Function to save an item to the spice inventory in the database
bool saveSpiceItem(int userId, const std::string& name, int percentage, const std::string& unit, int referenceAmount);

// Function to retrieve spice items for a specific user from the database
bool getSpiceItems(int userId, std::vector<std::string>& items);  // Example, replace std::vector<std::string> with actual type

#endif // DATABASE_H

