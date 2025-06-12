#pragma once

#include <fstream>
#include <unordered_map>
#include <string>
#include <openssl/sha.h>
#include <iomanip>


inline std::string hashPassword(const std::string &password)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char *>(password.c_str()), password.size(), hash);

    std::ostringstream ss;
    for (unsigned char c : hash)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return ss.str();
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

inline int isValidEmail(const std::string &email)
{
    auto atPos = email.find("@");
    auto dotPos = email.find(".");

    return atPos != std::string::npos && dotPos != std::string::npos && atPos > 0 && dotPos > atPos + 1 && dotPos < email.size() - 1;
}