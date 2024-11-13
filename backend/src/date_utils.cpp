#include "date_utils.h"
#include <iomanip>
#include <sstream>

std::string DateUtils::formatExpirationDate(const std::time_t& expirationDate) {
    std::tm* tm = std::localtime(&expirationDate);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d");
    return oss.str();
}
