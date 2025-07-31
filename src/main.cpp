#include <cstdio>
#include <cstdint>
#include <cstring>
#include <vector>
#include <arm_neon.h>  // Для NEON-типов
#include "core_dump.hpp"

#define packed_struct __attribute__((packed))
#define noinline __attribute__((noinline))

#ifndef PADDING_SIZE
    #define PADDING_SIZE 1
#endif

// === Данные для симуляции проблемы === //
struct SHM
{
    uint32_t shm_version{0};            // Shared memory version (taken from library version MMmmpp)
    uint16_t version{0};                // Parameter XML version
    uint32_t checksum{0xDEADBEEF};      // Dictionary checksum
    bool config_updated{false};         // Unsaved configuration flag
} packed_struct; // 11 byte

struct ParameterData
{
    struct Attributes
    {
        bool date{};                    // Affects configuration date
        bool action_log{};              // Change is recorded to the action log
        bool event_log{};               // Change is recorded to the event log
    } packed_struct; // 3 byte

    uint16_t default_idx{1};            // Default idx
    uint16_t reg{0};                    // Register index starting from 0
    Attributes attributes{};            // Parameter attributes
    uint16_t objnum{0};                 // Parameter number
    uint8_t dummy[PADDING_SIZE]{};      // Padding
} packed_struct; // 10 byte

inline uint64_t GetDefaultValue(const std::uint64_t* defaults, const ParameterData& info)
{
    uint64x1_t val = vld1_u64(defaults + info.default_idx);
    return vget_lane_u64(val, 0);
}

noinline std::uint64_t GetDefault(const std::uint64_t* defaults, const ParameterData& info)
{
    return GetDefaultValue(defaults, info);
}

int main()
{
    // Enable core dump
    SetCoreDumpState(true);

    // Preparing data
    SHM shm{};
    ParameterData data{};
    std::vector<std::uint64_t> defaults{1u, 2u, 3u, 4u, 5u};
    auto size_shm = sizeof(shm);
    auto size_data = sizeof(data);
    auto size_defaults = sizeof(decltype(defaults)::value_type) * defaults.size();
    auto size = size_shm + size_data + size_defaults;

    printf("=== Padding size: %d ===\n", PADDING_SIZE);
    std::printf("Size SHM: %u\nSize data: %u\nSize defaults: %u\nSize all: %u\n", size_shm, size_data, size_defaults, size);

    void* raw_data = operator new(size + 100); // Allocate raw memory
    
    if (!raw_data)
    {
        return 1;
    }

    printf("=== Memory allocated ===\n");

    std::uint8_t* curr_ptr = reinterpret_cast<std::uint8_t*>(raw_data);

    std::memcpy(curr_ptr, reinterpret_cast<const std::uint8_t*>(&shm), size_shm);
    curr_ptr += size_shm;

    std::memcpy(curr_ptr, reinterpret_cast<const std::uint8_t*>(&data), size_data);
    curr_ptr += size_data;

    std::memcpy(curr_ptr, reinterpret_cast<const std::uint8_t*>(defaults.data()), size_defaults);
    curr_ptr += size_defaults;

    if (static_cast<std::size_t>(curr_ptr - reinterpret_cast<std::uint8_t*>(raw_data)) == size)
    {
        printf("=== Success copied ===\n");
    }

    // Checks
    {
        auto bytes = reinterpret_cast<std::uint8_t*>(raw_data);

        auto shm = reinterpret_cast<SHM*>(bytes);
        if (shm->checksum == 0xDEADBEEF)
        {
            printf("=== Success check #1 ===\n");
        }

        auto data = reinterpret_cast<ParameterData*>(bytes + size_shm);
        if (data->default_idx == 1)
        {
            printf("=== Success check #2 ===\n");
        }

        auto defaults = reinterpret_cast<std::uint64_t*>(bytes + size_shm + size_data);
        
        std::printf("Address: %p\n", defaults);

        std::size_t address{};
        std::memcpy(&address, &defaults, sizeof(*defaults));
        if (address % sizeof(std::uint64_t) != 0)
        {
            std::printf("=== ARM32 CRASH ===\n");
        }

        auto value = GetDefault(defaults, ParameterData{}); // BUS ERROR!

        std::printf("Default value[%u]: %llu\n", data->default_idx, value);
    }

    operator delete(raw_data);

    return 0;
}