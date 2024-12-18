#include "spice_item.h"

// Default constructor
SpiceItem::SpiceItem() : Item(), percentage(0), referenceAmount(0) {}

// Parameterized constructor
SpiceItem::SpiceItem(const std::string& name, int percentage, const std::string& unit, int referenceAmount)
    : Item(name, (percentage * referenceAmount) / 100, unit, 0),  // No expiration date for spices
      percentage(percentage), referenceAmount(referenceAmount) {}

// Getter for percentage
int SpiceItem::getPercentage() const {
    return percentage;
}

// Getter for reference amount
int SpiceItem::getReferenceAmount() const {
    return referenceAmount;
}

// Calculates the actual current amount based on percentage and reference amount
int SpiceItem::calculateCurrentAmount() const {
    return (percentage * referenceAmount) / 100;
}

// Updates the remaining percentage and quantity
void SpiceItem::updatePercentage(int newPercentage) {
    percentage = newPercentage;
    updateQuantity((newPercentage * referenceAmount) / 100);  // Update base class quantity
}
