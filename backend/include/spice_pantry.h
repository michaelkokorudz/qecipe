#ifndef SPICE_PANTRY_H
#define SPICE_PANTRY_H

#include "spice_item.h"
#include <unordered_map>
#include <string>
#include <vector>

class SpicePantry {
public:
    SpicePantry();
    
    // User-specific methods
    void addSpice(int userId, const std::string& name, int percentage, const std::string& unit, int referenceAmount);
    void updateSpice(int userId, const std::string& name, int percentage);
    void displaySpices(int userId) const;

private:
    std::unordered_map<int, std::unordered_map<std::string, SpiceItem>> userSpices;  // Maps userId to their spices
    std::string formatSpiceDisplay(const SpiceItem& item) const;
};

#endif
