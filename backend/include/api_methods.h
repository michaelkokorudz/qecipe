#ifndef API_METHODS_H
#define API_METHODS_H

#include <vector>
#include <string>
#include <utility>

// User-related methods
bool AddUser(const std::string& username, const std::string& password);
bool CheckIfUserExists(const std::string& username);
bool loginUser(const std::string& username, const std::string& password);

// Fridge-related methods
bool AddFridgeItem(const std::string& username, const std::string& name, int quantity, const std::string& unit, const std::string& expirationDate);
std::vector<std::string> GetFridgeItems(const std::string& username);

// Spice-related methods
bool AddSpiceItem(const std::string& username, const std::string& name, int percentage, const std::string& unit, int referenceAmount, const std::string& expirationDate);
std::vector<std::string> GetSpiceItems(const std::string& username);

// Get quantity and units for a given item
std::pair<int, std::string> GetQuantityAndUnitsForItem(const std::string& username, const std::string& tableType, const std::string& itemName);


std::string GetExpirationDateForItem(const std::string& username, const std::string& tableType, const std::string& itemName);

#endif // API_METHODS_H
