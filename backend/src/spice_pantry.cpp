#include "spice_pantry.h"
#include <iostream>
#include <sstream>

SpicePantry::SpicePantry() {}

// Adds or updates a spice item for a specific user
void SpicePantry::addSpice(int userId, const std::string& name, int percentage, const std::string& unit, int referenceAmount) {
    auto& spices = userSpices[userId];  // Get or create the user's spice map
    spices[name] = SpiceItem(name, percentage, unit, referenceAmount);  // Add or replace the spice item for the user
}

// Updates the percentage of a spice item for a specific user
void SpicePantry::updateSpice(int userId, const std::string& name, int percentage) {
    auto it = userSpices.find(userId);
    if (it != userSpices.end()) {
        auto& spices = it->second;
        auto spiceIt = spices.find(name);
        if (spiceIt != spices.end()) {
            spiceIt->second.updatePercentage(percentage);  // Update the spice percentage for the user
        } else {
            std::cout << "Spice " << name << " not found in the pantry for user " << userId << ".\n";
        }
    } else {
        std::cout << "No spices found for user " << userId << ".\n";
    }
}

// Displays all spices for a specific user
void SpicePantry::displaySpices(int userId) const {
    auto it = userSpices.find(userId);
    if (it != userSpices.end() && !it->second.empty()) {
        std::cout << "Spice Pantry for User " << userId << ":\n";
        for (const auto& [name, spice] : it->second) {
            std::cout << formatSpiceDisplay(spice) << "\n";
        }
    } else {
        std::cout << "No spices found in the pantry for User " << userId << ".\n";
    }
}

// Helper function to format spice display information
std::string SpicePantry::formatSpiceDisplay(const SpiceItem& item) const {
    std::ostringstream oss;
    int currentAmount = item.calculateCurrentAmount();
    oss << "- " << item.getName() << ": " << item.getPercentage() << "% remaining ("
        << currentAmount << " " << item.getUnit() << " of " << item.getReferenceAmount()
        << " " << item.getUnit() << " total)";
    return oss.str();
}
