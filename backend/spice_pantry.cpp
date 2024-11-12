#include "spice_pantry.h"
#include <iostream>
#include <sstream>

SpicePantry::SpicePantry() {}

void SpicePantry::addSpice(const std::string& name, int percentage, const std::string& unit, int referenceAmount) {
    spices[name] = SpiceItem(name, percentage, unit, referenceAmount);
}

void SpicePantry::updateSpice(const std::string& name, int percentage) {
    auto it = spices.find(name);
    if (it != spices.end()) {
        it->second.updatePercentage(percentage);
    } else {
        std::cout << "Spice " << name << " not found in the pantry.\n";
    }
}

void SpicePantry::displaySpices() const {
    std::cout << "Current Spice Pantry:\n";
    for (const auto& [name, spice] : spices) {
        std::cout << formatSpiceDisplay(spice) << "\n";
    }
}

//helper function to format spice display information
std::string SpicePantry::formatSpiceDisplay(const SpiceItem& item) const {
    std::ostringstream oss;
    int currentAmount = item.calculateCurrentAmount();
    oss << "- " << item.getName() << ": " << item.getPercentage() << "% remaining ("
        << currentAmount << " " << item.getUnit() << " of " << item.getReferenceAmount()
        << " " << item.getUnit() << " total)";
    return oss.str();
}
