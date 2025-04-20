#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <zlib.h>
#include <atomic>
#include "../src/worker.hpp"

extern std::atomic<bool> found;

TEST(WorkerTest, FindsCorrectSuffix) {
    found = false;

    // Базовые данные
    std::vector<uint8_t> base = {'T', 'E', 'S', 'T'};
    std::string append = "He-he-he";
    base.insert(base.end(), append.begin(), append.end());

    uint32_t target_crc;
    std::vector<uint8_t> with_suffix = base;

    // Добавим заведомо подходящий суффикс
    std::vector<uint8_t> known_suffix = {0x12, 0x34, 0x56, 0x78};
    with_suffix.insert(with_suffix.end(), known_suffix.begin(), known_suffix.end());

    target_crc = crc32(0, with_suffix.data(), with_suffix.size());

    std::vector<uint8_t> result;

    // worker должен найти 0x12345678
    uint32_t val = 0x12345678;
    worker(base, target_crc, val, val + 1, &result);

    ASSERT_FALSE(result.empty());
    ASSERT_EQ(result.size(), base.size() + 4);
    ASSERT_EQ(result[result.size() - 4], 0x12);
    ASSERT_EQ(result[result.size() - 3], 0x34);
    ASSERT_EQ(result[result.size() - 2], 0x56);
    ASSERT_EQ(result[result.size() - 1], 0x78);

    // Проверим CRC
    uint32_t crc = crc32(0, result.data(), result.size());
    ASSERT_EQ(crc, target_crc);
}