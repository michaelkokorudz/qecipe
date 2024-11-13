#include "item.h"

Item::Item(const std::string& name, int quantity, const std::string& unit, const std::time_t& expirationDate)
    : name(name), quantity(quantity), unit(unit), expirationDate(expirationDate) {}

std::string Item::getName() const {
    return name;
}

int Item::getQuantity() const {
    return quantity;
}

std::string Item::getUnit() const {
    return unit;
}

std::time_t Item::getExpirationDate() const {
    return expirationDate;
}

void Item::updateQuantity(int quantity) {
    this->quantity += quantity;
}
