#pragma once

#include <vector>
#include <cstdint>
#include <atomic>

uint32_t calculate_crc32(const std::vector<uint8_t>& data);

void worker(const std::vector<uint8_t>& base, uint32_t target_crc,
            uint32_t start, uint32_t end,
            std::vector<uint8_t>* result_ptr);

extern std::atomic<bool> found;


