#include "policy.hpp"
#include "path_security.hpp"

PolicyResult evaluatePolicy(
    const ArchiveStats& stats,
    const SecurityPolicy& policy
) {
    PolicyResult result;

    if (stats.uncompressedSize > policy.maxUncompressedSize) {
        result.verdict = SecurityVerdict::Block;
        result.violations.push_back({
            "Total uncompressed size exceeds the configured limit."
        });
    }

    if (stats.fileCount > policy.maxFileCount) {
        result.verdict = SecurityVerdict::Block;
        result.violations.push_back({
            "File count exceeds the configured limit."
        });
    }

    if (stats.largestFile > policy.maxSingleFileSize) {
        result.verdict = SecurityVerdict::Block;
        result.violations.push_back({
            "Largest file exceeds the configured limit."
        });
    }

    for (const auto& entry : stats.entries) {

        // Archive path traversal protection
        if (!isPathSafe(entry.name)) {
            result.verdict = SecurityVerdict::Block;
            result.violations.push_back({
                "Unsafe archive path: " + entry.name
            });
        }

        // Compression-ratio protection
        if (entry.compressedSize > 0) {
            const double ratio =
                static_cast<double>(entry.uncompressedSize) /
                static_cast<double>(entry.compressedSize);

            if (ratio > policy.maxCompressionRatio) {
                result.verdict = SecurityVerdict::Block;
                result.violations.push_back({
                    "Compression ratio exceeds the configured limit: " +
                    entry.name
                });
            }
        }
    }

    return result;
}