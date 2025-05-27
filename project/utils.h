#pragma once

#include <regex>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>

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

inline std::unordered_map<std::string, std::string> readEnvFile(const std::string &filename)
{
    std::unordered_map<std::string, std::string> env;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл " + filename);
    }

    std::string line;
    while (std::getline(file, line))
    {
        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        env[key] = value;
    }

    return env;
}
