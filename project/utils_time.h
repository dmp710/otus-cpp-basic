#include <regex>
#include <iomanip>
#include <sstream>
#include <chrono>


inline std::chrono::system_clock::time_point parse_datetime(const std::string &datetime) {
    std::tm tm = {};
    std::istringstream ss(datetime);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
    
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

// Проверка формата YYYY-MM-DD
inline bool is_valid_date(const std::string &date)
{
    static const std::regex pattern(R"(^\d{4}-\d{2}-\d{2}$)");

    if (!std::regex_match(date, pattern))
        return false;

    int year, month, day;
    char extra;

    if (sscanf(date.c_str(), "%d-%d-%d%c", &year, &month, &day, &extra) != 3)
        return false;

    if (month < 1 || month > 12 || day < 1 || day > 31)
        return false;

    return true;
}

// Проверка формата YYYY-MM-DD HH:MM
inline bool is_valid_datetime(const std::string &datetime)
{
    static const std::regex pattern(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}$)");

    if (!std::regex_match(datetime, pattern))
        return false;

    int year, month, day, hour, minute;
    char extra;

    if (sscanf(datetime.c_str(), "%d-%d-%d %d:%d%c", &year, &month, &day, &hour, &minute, &extra) != 5)
        return false;

    if (month < 1 || month > 12 || day < 1 || day > 31 ||
        hour < 0 || hour > 23 || minute < 0 || minute > 59)
        return false;

    return true;
}
