#pragma once

#include <vector>
#include <cstdint>
#include <atomic>

void worker(std::vector<uint8_t> base, uint32_t target_crc,
            uint64_t start, uint64_t end,
            std::vector<uint8_t>* result_ptr);

extern std::atomic<bool> found;


