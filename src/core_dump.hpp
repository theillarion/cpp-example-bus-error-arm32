#pragma once

#include <cstdint>
#include <limits>

/**
 * @brief Set state (enable/disable) of core dump
 * @param enable - turn it on or off
 * @param limit - max size of core dumps in bytes
 * @param flags - bit mask of memory mapping types (see man core 5)
 * @return true if success enable/disable
 */
bool SetCoreDumpState(bool enable, std::uint64_t limit = std::numeric_limits<std::uint64_t>::max(),
        std::uint8_t flags = 0x33);
