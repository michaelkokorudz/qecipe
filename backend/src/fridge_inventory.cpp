#include "fridge_inventory.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>  

FridgeInventory::FridgeInventory(int expirationAlertDays)
    : expirationAlertDays(expirationAlertDays) {}

// Adds or updates an item specific to a user
void FridgeInventory::addItem(int userId, const std::string& name, int quantity, const std::string& unit, const std::time_t& expirationDate) {
    auto& items = userItems[userId];  // Get or create the user's item map
    auto it = items.find(name);
    if (it != items.end()) {
        it->second.updateQuantity(quantity);  // Update quantity if item exists
    } else {
        items[name] = Item(name, quantity, unit, expirationDate);  // Add new item for user
    }
}

// Removes or updates the quantity of an item specific to a user
void FridgeInventory::removeItem(int userId, const std::string& name, int quantity) {
    auto& items = userItems[userId];
    auto it = items.find(name);
    if (it != items.end()) {
        int currentQuantity = it->second.getQuantity();
        if (quantity >= currentQuantity) {
            items.erase(it);  // Remove item completely if quantity is depleted
        } else {
            it->second.updateQuantity(-quantity);  // Reduce quantity
        }
    } else {
        std::cout << "Item " << name << " not found in the fridge for user " << userId << ".\n";
    }
}

// Returns items that are expiring soon for a specific user
std::vector<Item> FridgeInventory::checkExpiringItems(int userId) const {
    std::vector<Item> expiringItems;
    auto it = userItems.find(userId);
    if (it != userItems.end()) {
        for (const auto& [name, item] : it->second) {
            if (isExpiringSoon(item)) {
                expiringItems.push_back(item);
            }
        }
    }
    return expiringItems;
}

// Displays items for a specific user
void FridgeInventory::displayItems(int userId) const {
    auto it = userItems.find(userId);
    if (it != userItems.end() && !it->second.empty()) {
        std::cout << "Fridge Inventory for User " << userId << ":\n";
        for (const auto& [name, item] : it->second) {
            std::cout << "- " << item.getName() << ": " << item.getQuantity() << " " << item.getUnit()
                      << " (Expires on: " << formatExpirationDate(item.getExpirationDate()) << ")\n";
        }
    } else {
        std::cout << "No items found in the fridge for User " << userId << ".\n";
    }
}

// Checks if an item is expiring soon based on alert days
bool FridgeInventory::isExpiringSoon(const Item& item) const {
    std::time_t now = std::time(nullptr);
    double daysUntilExpiration = std::difftime(item.getExpirationDate(), now) / (60 * 60 * 24);
    return daysUntilExpiration <= expirationAlertDays;
}

// Formats the expiration date for display
std::string FridgeInventory::formatExpirationDate(const std::time_t& expirationDate) const {
    std::tm* tm = std::localtime(&expirationDate);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d");
    return oss.str();
}
