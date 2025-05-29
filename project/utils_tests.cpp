#include <gtest/gtest.h>
#include <fstream>
#include <cstdio> 
#include "utils.h" 

TEST(UtilsTest, HashPassword_IsDeterministic) {
    std::string password = "secret";
    auto hash1 = hashPassword(password);
    auto hash2 = hashPassword(password);
    EXPECT_EQ(hash1, hash2);
    EXPECT_EQ(hash1.length(), 64); // SHA-256 hex длиной 64 символа
}

TEST(UtilsTest, IsValidDate_ValidCases) {
    EXPECT_TRUE(is_valid_date("2024-12-31"));
    EXPECT_TRUE(is_valid_date("1999-01-01"));
}

TEST(UtilsTest, IsValidDate_InvalidCases) {
    EXPECT_FALSE(is_valid_date("2024-13-01"));
    EXPECT_FALSE(is_valid_date("2024-00-01"));
    EXPECT_FALSE(is_valid_date("2024-12-32"));
    EXPECT_FALSE(is_valid_date("2024/12/31"));
    EXPECT_FALSE(is_valid_date("24-12-31"));
}

TEST(UtilsTest, IsValidDateTime_ValidCases) {
    EXPECT_TRUE(is_valid_datetime("2024-12-31 23:59"));
    EXPECT_TRUE(is_valid_datetime("2000-01-01 00:00"));
}

TEST(UtilsTest, IsValidDateTime_InvalidCases) {
    EXPECT_FALSE(is_valid_datetime("2024-12-31 24:00")); // Часы вне диапазона
    EXPECT_FALSE(is_valid_datetime("2024-12-31 23:60")); // Минуты вне диапазона
    EXPECT_FALSE(is_valid_datetime("2024-12-32 23:59")); // День вне диапазона
    EXPECT_FALSE(is_valid_datetime("2024-12-31T23:59")); // Неправильный формат
}

TEST(UtilsTest, ReadEnvFile_ReadsCorrectly) {
    const char* filename = "test.env";
    std::ofstream file(filename);
    file << "DB_HOST=localhost\nDB_PORT=5432\n";
    file.close();

    auto env = readEnvFile(filename);
    EXPECT_EQ(env["DB_HOST"], "localhost");
    EXPECT_EQ(env["DB_PORT"], "5432");

    std::remove(filename); // Очистка
}

TEST(UtilsTest, IsValidEmail_ValidAndInvalid) {
    std::string valid = "test@example.com";
    std::string invalid1 = "testexample.com";
    std::string invalid2 = "test@.com";
    std::string invalid3 = "test@com.";
    std::string invalid4 = "test@com";

    EXPECT_TRUE(isValidEmail(valid));
    EXPECT_FALSE(isValidEmail(invalid1));
    EXPECT_FALSE(isValidEmail(invalid2));
    EXPECT_FALSE(isValidEmail(invalid3));
    EXPECT_FALSE(isValidEmail(invalid4));
}

TEST(UtilsTest, ParseDateTime_CorrectConversion) {
    auto tp = parse_datetime("2024-12-31 23:59:59");

    // Преобразуем обратно в строку для проверки
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&tt);

    EXPECT_EQ(tm.tm_year + 1900, 2024);
    EXPECT_EQ(tm.tm_mon + 1, 12);
    EXPECT_EQ(tm.tm_mday, 31);
    EXPECT_EQ(tm.tm_hour, 23);
    EXPECT_EQ(tm.tm_min, 59);
    EXPECT_EQ(tm.tm_sec, 59);
}
