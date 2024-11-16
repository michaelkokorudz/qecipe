#include "spice_pantry.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Constructor
SpicePantry::SpicePantry() {}

// Add or replace a spice for a user
void SpicePantry::addSpice(int userId, const std::string& name, int percentage, const std::string& unit, int referenceAmount) {
    auto& spices = userSpices[userId];
    auto it = spices.find(name);
    if (it == spices.end()) {
        spices.emplace(name, SpiceItem(name, percentage, unit, referenceAmount));
    } else {
        it->second = SpiceItem(name, percentage, unit, referenceAmount);
    }
}

// Update the percentage of an existing spice for a user
void SpicePantry::updateSpice(int userId, const std::string& name, int percentage) {
    if (userSpices.find(userId) != userSpices.end() && userSpices[userId].find(name) != userSpices[userId].end()) {
        userSpices[userId][name].updatePercentage(percentage);
    } else {
        std::cerr << "Spice not found for user " << userId << ": " << name << std::endl;
    }
}

// Display all spices for a user
void SpicePantry::displaySpices(int userId) const {
    if (userSpices.find(userId) != userSpices.end()) {
        for (const auto& [name, spice] : userSpices.at(userId)) {
            std::cout << formatSpiceDisplay(spice) << std::endl;
        }
    } else {
        std::cout << "No spices found for user " << userId << "." << std::endl;
    }
}

// Helper to format spice display
std::string SpicePantry::formatSpiceDisplay(const SpiceItem& item) const {
    std::ostringstream oss;
    oss << std::left << std::setw(20) << item.getName()
        << std::setw(10) << item.getPercentage() << "%"
        << std::setw(10) << item.getUnit();
    return oss.str();
}
