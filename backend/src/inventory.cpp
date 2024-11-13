#include "inventory.h"
#include "date_utils.h"
#include <iostream>

Inventory::Inventory(int expirationAlertDays)
    : fridgeInventory(expirationAlertDays), spicePantry() {}

void Inventory::addFridgeItem(int userId, const std::string& name, int quantity, const std::string& unit, const std::time_t& expirationDate) {
    fridgeInventory.addItem(userId, name, quantity, unit, expirationDate);
}

void Inventory::addSpiceItem(int userId, const std::string& name, int percentage, const std::string& unit, int referenceAmount) {
    spicePantry.addSpice(userId, name, percentage, unit, referenceAmount);
}

void Inventory::displayFridgeItems(int userId) const {
    fridgeInventory.displayItems(userId);
}

void Inventory::displaySpiceItems(int userId) const {
    spicePantry.displaySpices(userId);
}
