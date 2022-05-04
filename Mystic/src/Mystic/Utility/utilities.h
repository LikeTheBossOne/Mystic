#pragma once
#include <climits>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <unordered_map>
#include "guiddef.h"

// Ensure it has 128 bits
static_assert(sizeof(_GUID) == 128 / CHAR_BIT, "GUID");

// Specialize std::hash
namespace std {
    template<> struct hash<GUID>
    {
        size_t operator()(const GUID& guid) const noexcept {
            const std::uint64_t* p = reinterpret_cast<const std::uint64_t*>(&guid);
            std::hash<std::uint64_t> hash;
            return hash(p[0]) ^ hash(p[1]);
        }
    };
}