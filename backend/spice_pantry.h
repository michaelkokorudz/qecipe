#ifndef SPICE_PANTRY_H
#define SPICE_PANTRY_H

#include "spice_item.h"
#include <unordered_map>
#include <string>
#include <vector>

class SpicePantry {
public:
    SpicePantry();

    void addSpice(const std::string& name, int percentage, const std::string& unit, int referenceAmount);
    void updateSpice(const std::string& name, int percentage);
    void displaySpices() const;

private:
    std::unordered_map<std::string, SpiceItem> spices;

    std::string formatSpiceDisplay(const SpiceItem& item) const;
};

#endif
