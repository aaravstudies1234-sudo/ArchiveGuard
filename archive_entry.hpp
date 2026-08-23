#pragma once
#include <cstdint>
#include <string>
struct ArchiveEntry{
    std::string name;
    std::uint64_t compressedSize = 0;
    std::uint64_t uncompressedSize = 0;
    bool isDirectory = false;
};