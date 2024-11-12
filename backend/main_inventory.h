#ifndef MAIN_INVENTORY_H
#define MAIN_INVENTORY_H

#include "fridge_inventory.h"
#include "spice_pantry.h"
#include <unordered_map>
#include <string>
#include <ctime>
#include <vector>



class Inventory{

    public:
        void addFridgeItem(Item item);
        void addPantryItem(Item item);
        void addSpiceItem(SpiceItem spiceItem);



    private:
       FridgeInventory fridgeInv;
       SpicePantry spicePan;


};


#endif