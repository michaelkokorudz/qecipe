#include "fridge_inventory.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>  


FridgeInventory::FridgeInventory(int expirationAlertDays)
    : expirationAlertDays(expirationAlertDays) {}

void FridgeInventory::addItem(const std::string& name, int quantity, const std::string& unit, const std::time_t& expirationDate) {
    auto it = items.find(name);
    if (it != items.end()) {
        it->second.updateQuantity(quantity);  // Update quantity if item exists
    } else {
        items[name] = Item(name, quantity, unit, expirationDate);  // Add new item
    }
}

void FridgeInventory::removeItem(const std::string& name, int quantity) {
    auto it = items.find(name);
    if (it != items.end()) {
        int currentQuantity = it->second.getQuantity();
        if (quantity >= currentQuantity) {
            items.erase(it);  // Remove item completely if quantity is depleted
        } else {
            it->second.updateQuantity(-quantity);  // Reduce quantity
        }
    } else {
        std::cout << "Item " << name << " not found in the fridge.\n";
    }
}

std::vector<Item> FridgeInventory::checkExpiringItems() const {
    std::vector<Item> expiringItems;
    for (const auto& [name, item] : items) {
        if (isExpiringSoon(item)) {
            expiringItems.push_back(item);
        }
    }
    return expiringItems;
}

void FridgeInventory::displayItems() const {
    std::cout << "Current Fridge Inventory:\n";
    for (const auto& [name, item] : items) {
        std::cout << "- " << item.getName() << ": " << item.getQuantity() << " " << item.getUnit()
                  << " (Expires on: " << formatExpirationDate(item.getExpirationDate()) << ")\n";
    }
}

bool FridgeInventory::isExpiringSoon(const Item& item) const {
    std::time_t now = std::time(nullptr);
    double daysUntilExpiration = std::difftime(item.getExpirationDate(), now) / (60 * 60 * 24);
    return daysUntilExpiration <= expirationAlertDays;
}

std::string FridgeInventory::formatExpirationDate(const std::time_t& expirationDate) const {
    std::tm* tm = std::localtime(&expirationDate);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d");
    return oss.str();
}
