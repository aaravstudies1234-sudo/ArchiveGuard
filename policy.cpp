#include "policy.hpp"

PolicyResult evaluatePolicy(const ArchiveStats& stats,const SecurityPolicy& policy){
    PolicyResult result;
    if (stats.uncompressedSize > policy.maxUncompressedSize){
        result.verdict = SecurityVerdict::Block;
        result.violations.push_back({
            "Total uncompressed size exceeds the configured limit."
        });
    }
    if (stats.fileCount > policy.maxFileCount){
        result.verdict = SecurityVerdict::Block;
        result.violations.push_back({
            "File count exceeds the configured limit."
        });
    }
    if (stats.largestFile > policy.maxSingleFileSize){
        result.verdict = SecurityVerdict::Block;
        result.violations.push_back({
            "Largest file exceeds the configured limit."
        });
    }
    return result;
}