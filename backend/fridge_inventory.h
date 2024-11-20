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

    void addItem(const std::string& name, int quantity, const std::string& unit, const std::time_t& expirationDate);
    void removeItem(const std::string& name, int quantity);
    std::vector<Item> checkExpiringItems() const;
    void displayItems() const;

private:
    std::unordered_map<std::string, Item> items;  
    int expirationAlertDays;

    bool isExpiringSoon(const Item& item) const;
    std::string formatExpirationDate(const std::time_t& expirationDate) const;
};

#endif
