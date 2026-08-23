#pragma once
#include <cstdint>

struct ArchiveStats{
    std::uint64_t compressedSize = 0;
    std::uint64_t uncompressedSize = 0;
    std::uint64_t fileCount = 0;
    std::uint64_t largestFile = 0;
};