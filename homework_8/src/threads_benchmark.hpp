#pragma once
#include <vector>
#include <cstdint>

int benchmark_threads(const std::vector<uint8_t>& base, uint32_t iterations = 5000000);