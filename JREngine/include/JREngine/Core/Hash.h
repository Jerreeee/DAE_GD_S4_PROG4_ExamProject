#pragma once
#include <cstdint>
#include <string_view>

namespace JRE
{
    constexpr uint64_t FNV1a64(std::string_view str) noexcept
    {
        uint64_t h = 14695981039346656037ULL;
        for (unsigned char c : str) { h ^= c; h *= 1099511628211ULL; }
        return h;
    }
}
