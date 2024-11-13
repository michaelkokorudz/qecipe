#ifndef FRIDGE_INVENTORY_H
#define FRIDGE_INVENTORY_H

#include "item.h"
#include <unordered_map>
#include <string>
#include <ctime>
#include <vector>

class FridgeInventory {
public:
    FridgeInventory(int expirationAlertDays);

    // User-specific methods
    void addItem(int userId, const std::string& name, int quantity, const std::string& unit, const std::time_t& expirationDate);
    void removeItem(int userId, const std::string& name, int quantity);
    std::vector<Item> checkExpiringItems(int userId) const;
    void displayItems(int userId) const;

private:
    std::unordered_map<int, std::unordered_map<std::string, Item>> userItems;  // Maps userId to their items
    int expirationAlertDays;

    bool isExpiringSoon(const Item& item) const;
    std::string formatExpirationDate(const std::time_t& expirationDate) const;
};

#endif
