#ifndef SPICE_ITEM_H
#define SPICE_ITEM_H

#include "item.h"
#include <string>

class SpiceItem : public Item {
public:
    SpiceItem(const std::string& name, int percentage, const std::string& unit, int referenceAmount);

    int getPercentage() const;
    int getReferenceAmount() const;
    int calculateCurrentAmount() const;

    void updatePercentage(int newPercentage);

private:
    int percentage;       // Remaining percentage (1–100)
    int referenceAmount;  // The reference amount when full (e.g., 100 grams)
};

#endif
