#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>



void ensureDatabaseDirectoryExists();

bool tableExists(const std::string& tableName);

// Get the full database path
std::string getDatabasePath();

// Generate a table name based on the username and table type
std::string generateTableName(const std::string& username, const std::string& tableType);

// Initialize the core database structure
void initializeDatabase();

// User management
bool addUser(const std::string& username, const std::string& password); // Add a new user
bool userExists(const std::string& username);                          // Check if a user exists

// Create user-specific tables
bool createUserSpecificTables(const std::string& username);

std::string getUsernameById(int userId);

// Fridge item management
bool saveFridgeItem(const std::string& username, const std::string& name, int quantity, const std::string& unit, const std::string& expirationDate);
bool getFridgeItems(const std::string& username, std::vector<std::string>& items);

// Spice item management
bool saveSpiceItem(const std::string& username, const std::string& name, int percentage, const std::string& unit, int referenceAmount, const std::string& expirationDate);
bool getSpiceItems(const std::string& username, std::vector<std::string>& items);

#endif // DATABASE_H
