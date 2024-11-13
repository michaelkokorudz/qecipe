#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <ctime>

class Item {
public:
    Item(const std::string& name, int quantity, const std::string& unit, const std::time_t& expirationDate);

    std::string getName() const;
    int getQuantity() const;
    std::string getUnit() const;
    std::time_t getExpirationDate() const;
    void updateQuantity(int quantity);

private:
    std::string name;
    int quantity;
    std::string unit;
    std::time_t expirationDate;
    std::string storageLocation;
};

#endif
