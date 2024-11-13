#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <string>
#include <ctime>

class DateUtils {
public:
    static std::string formatExpirationDate(const std::time_t& expirationDate);
};

#endif
