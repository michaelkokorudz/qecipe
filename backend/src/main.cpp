#include <iostream>
#include <vector>
#include <string>
#include "api_methods.h" // Include your API methods
#include "database.h"    // Include your database utilities

void testAddUser() {
    std::cout << "Testing AddUser...\n";
    if (AddUser("testuser", "password123")) {
        std::cout << "✅ User added successfully.\n";
    } else {
        std::cout << "❌ User already exists or an error occurred.\n";
    }
}

void testLoginUser() {
    std::cout << "Testing loginUser...\n";
    if (loginUser("testuser", "password123")) {
        std::cout << "✅ Login successful.\n";
    } else {
        std::cout << "❌ Login failed: Invalid credentials or user not found.\n";
    }
}

void testAddSpiceItem() {
    std::cout << "Testing AddSpiceItem...\n";
    if (AddSpiceItem("testuser", "Salt", 50, "grams", 100, "2024-06-15")) {
        std::cout << "✅ Pantry item 'Salt' added successfully.\n";
    } else {
        std::cout << "❌ Failed to add pantry item 'Salt'.\n";
    }
}

void testGetExpirationDateForItem() {
    std::cout << "Testing GetExpirationDateForItem...\n";

    std::string fridgeExpirationDate = GetExpirationDateForItem("testuser", "Fridge", "Milk");
    if (fridgeExpirationDate != "Not found") {
        std::cout << "✅ Fridge Item 'Milk' Expiration Date: " << fridgeExpirationDate << "\n";
    } else {
        std::cout << "❌ Fridge Item 'Milk' not found or an error occurred.\n";
    }

    std::string pantryExpirationDate = GetExpirationDateForItem("testuser", "Spice", "Salt");
    if (pantryExpirationDate != "Not found") {
        std::cout << "✅ Pantry Item 'Salt' Expiration Date: " << pantryExpirationDate << "\n";
    } else {
        std::cout << "❌ Pantry Item 'Salt' not found or an error occurred.\n";
    }
}

void testAddFridgeItem() {
    std::cout << "Testing AddFridgeItem...\n";
    if (AddFridgeItem("testuser", "Milk", 2, "L", "2024-12-31")) {
        std::cout << "✅ Fridge item 'Milk' added successfully.\n";
    } else {
        std::cout << "❌ Failed to add fridge item 'Milk'.\n";
    }
}

void testGetFridgeItems() {
    std::cout << "Testing GetFridgeItems...\n";
    std::vector<std::string> items = GetFridgeItems("testuser");
    if (!items.empty()) {
        std::cout << "✅ Fridge items:\n";
        for (const auto& item : items) {
            std::cout << "   - " << item << "\n";
        }
    } else {
        std::cout << "❌ No items found or an error occurred.\n";
    }
}

void testGetQuantityAndUnits() {
    std::cout << "Testing GetQuantityAndUnitsForItem...\n";
    auto [quantity, unit] = GetQuantityAndUnitsForItem("testuser", "Fridge", "Milk");
    if (quantity != -1) {
        std::cout << "✅ Item: Milk\n";
        std::cout << "   - Quantity: " << quantity << "\n";
        std::cout << "   - Unit: " << unit << "\n";
    } else {
        std::cout << "❌ Item not found.\n";
    }
}

int main() {
    // Initialize the database
    std::cout << "Initializing database...\n";
    initializeDatabase();

    // Run tests
    std::cout << "==============================\n";
    testAddUser();
    std::cout << "==============================\n";
    testLoginUser();
    std::cout << "==============================\n";
    testAddFridgeItem();
    std::cout << "==============================\n";
    testAddSpiceItem(); // Add the "Salt" item
    std::cout << "==============================\n";
    testGetFridgeItems();
    std::cout << "==============================\n";
    testGetQuantityAndUnits();
    std::cout << "==============================\n";
    testGetExpirationDateForItem();
    std::cout << "==============================\n";

    return 0;
}
