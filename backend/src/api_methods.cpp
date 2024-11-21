#include "api_methods.h"
#include "database.h"
#include <vector>
#include <string>
#include <iostream>
#include "sqlite3.h"

bool AddUser(const std::string& username, const std::string& password) {
    if (userExists(username)) {
        std::cerr << "User already exists: " << username << std::endl;
        return false;
    }
    if (addUser(username, password)) {
        if (createUserSpecificTables(username)) {
            return true;
        } else {
            std::cerr << "Failed to create user-specific tables for: " << username << std::endl;
            return false;
        }
    }
    return false;
}

// Check if a user exists in the database
bool CheckIfUserExists(const std::string& username) {
    return userExists(username);
}

// Verify user login
bool loginUser(const std::string& username, const std::string& password) {
    sqlite3* db;
    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const char* sql = "SELECT password FROM User WHERE username = ?;";
    sqlite3_stmt* stmt;

    bool isAuthenticated = false;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string storedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (storedPassword == password) {
                isAuthenticated = true;
            }
        } else {
            std::cerr << "User not found: " << username << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
    return isAuthenticated;
}

// Add a fridge item for a user
bool AddFridgeItem(const std::string& username, const std::string& name, int quantity, const std::string& unit, const std::string& expirationDate) {
    return saveFridgeItem(username, name, quantity, unit, expirationDate);
}

// Retrieve all fridge items for a user
std::vector<std::string> GetFridgeItems(const std::string& username) {
    std::vector<std::string> items;
    if (!getFridgeItems(username, items)) {
        std::cerr << "Failed to retrieve fridge items for user: " << username << std::endl;
    }
    return items;
}

// Add a spice item for a user
bool AddSpiceItem(const std::string& username, const std::string& name, int percentage, const std::string& unit, int referenceAmount, const std::string& expirationDate) {
    return saveSpiceItem(username, name, percentage, unit, referenceAmount, expirationDate);
}

// Retrieve all spice items for a user
std::vector<std::string> GetSpiceItems(const std::string& username) {
    std::vector<std::string> items;
    if (!getSpiceItems(username, items)) {
        std::cerr << "Failed to retrieve spice items for user: " << username << std::endl;
    }
    return items;
}

// Get quantity and unit for a specific item
std::pair<int, std::string> GetQuantityAndUnitsForItem(const std::string& username, const std::string& tableType, const std::string& itemName) {
    std::pair<int, std::string> result(-1, "");  // Default invalid values
    sqlite3* db;
    std::string dbPath = getDatabasePath();
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return result;
    }

    std::string tableName = generateTableName(username, tableType);
    std::string sql = "SELECT quantity, unit FROM " + tableName + " WHERE name = ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, itemName.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int quantity = sqlite3_column_int(stmt, 0);
            std::string unit = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            result = {quantity, unit};
        } else {
            std::cerr << "Item not found: " << itemName << " in table " << tableName << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
    return result;
}

// Retrieve the expiration date for a specific item in either Fridge or Pantry
std::string GetExpirationDateForItem(const std::string& username, const std::string& tableType, const std::string& itemName) {
    sqlite3* db;
    std::string dbPath = getDatabasePath();
    std::string expirationDate = "Not found";  // Default response if item is not found

    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return expirationDate;
    }

    std::string tableName = generateTableName(username, tableType);
    std::string sql = "SELECT expiration_date FROM " + tableName + " WHERE name = ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, itemName.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            expirationDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        } else {
            std::cerr << "Item not found: " << itemName << " in table " << tableName << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
    return expirationDate;
}

