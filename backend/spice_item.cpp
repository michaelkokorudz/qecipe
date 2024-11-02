#include "spice_item.h"

SpiceItem::SpiceItem(const std::string& name, int percentage, const std::string& unit, int referenceAmount)
    : Item(name, (percentage * referenceAmount) / 100, unit, 0),  // No expiration date for spices
      percentage(percentage), referenceAmount(referenceAmount) {}

int SpiceItem::getPercentage() const {
    return percentage;
}

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
    // Update the base class quantity to reflect the current amount based on percentage
    updateQuantity((newPercentage * referenceAmount) / 100 - getQuantity());
}
