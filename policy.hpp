#pragma once

#include <cstdint>

struct SecurityPolicy
{
    std::uint64_t maxUncompressedSize = 2ULL * 1024 * 1024 * 1024;
    std::uint64_t maxFileCount = 100'000;
    std::uint64_t maxSingleFileSize = 1ULL * 1024 * 1024 * 1024;
};

enum class SecurityVerdict
{
    Allow,
    Block
};