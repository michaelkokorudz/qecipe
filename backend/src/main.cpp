#include "inventory.h"
#include "database.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <sqlite3.h>

int main() {
    // Initialize database
    initializeDatabase();

    // Display the database before any operations
    std::cout << "Database State Before Operations:\n";

    // Initialize Inventory instance
    Inventory inventory(30);

    // Test user data
    std::string username = "ben10000";
    std::string password = "secure1password";

    // Add a new user and create user-specific tables
    if (addUser(username, password)) {
        std::cout << "User '" << username << "' added successfully.\n";
    } else {
        std::cerr << "Failed to add user '" << username << "' or user already exists.\n";
    }

    if (!createUserSpecificTables(username)) {
        std::cerr << "Failed to create user-specific tables for '" << username << "'.\n";
        return 1;
    }

    // Add milk to fridge
    if (saveFridgeItem(username, "Milk", 2, "Liters", "2024-12-01")) {
        std::cout << "Milk added to the fridge for user '" << username << "'.\n";
    } else {
        std::cerr << "Failed to add Milk to the fridge for user '" << username << "'.\n";
    }

    // Check if user exists
    if (userExists(username)) {
        std::cout << "User '" << username << "' exists. Adding Cheese to their fridge.\n";

        // Add cheese to fridge
        if (saveFridgeItem(username, "Cheese", 1, "Kg", "2024-11-30")) {
            std::cout << "Cheese added to the fridge for user '" << username << "'.\n";
        } else {
            std::cerr << "Failed to add Cheese to the fridge for user '" << username << "'.\n";
        }
    } else {
        std::cerr << "User '" << username << "' does not exist. Cannot add Cheese.\n";
    }

    // Display fridge items for the user
    std::vector<std::string> fridgeItems;
    if (getFridgeItems(username, fridgeItems)) {
        std::cout << "Fridge Items for User '" << username << "':\n";
        for (const auto& item : fridgeItems) {
            std::cout << item << "\n";
        }
    } else {
        std::cerr << "Failed to retrieve fridge items for user '" << username << "'.\n";
    }

    // Display the database after operations
    std::cout << "Database State After Operations:\n";
    return 0;
}