#ifndef INVENTORY_H
#define INVENTORY_H

#include "fridge_inventory.h"
#include "spice_pantry.h"

class Inventory {
public:
    Inventory(int expirationAlertDays);

    // User-specific methods for fridge and spice inventory
    void addFridgeItem(int userId, const std::string& name, int quantity, const std::string& unit, const std::time_t& expirationDate);
    void addSpiceItem(int userId, const std::string& name, int percentage, const std::string& unit, int referenceAmount);

    void displayFridgeItems(int userId) const;
    void displaySpiceItems(int userId) const;

private:
    FridgeInventory fridgeInventory;
    SpicePantry spicePantry;
};

#endif
