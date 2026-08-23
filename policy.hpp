#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "archive_stats.hpp"

struct SecurityPolicy{
    std::uint64_t maxUncompressedSize = 2ULL * 1024 * 1024 * 1024;
    std::uint64_t maxFileCount = 100'000;
    std::uint64_t maxSingleFileSize = 1ULL * 1024 * 1024 * 1024;
};

enum class SecurityVerdict{
    Allow,
    Warn,
    Block
};

struct PolicyViolation{
    std::string message;
};

struct PolicyResult{
    SecurityVerdict verdict = SecurityVerdict::Allow;
    std::vector<PolicyViolation> violations;
};

PolicyResult evaluatePolicy(
    const ArchiveStats& stats,
    const SecurityPolicy& policy
);