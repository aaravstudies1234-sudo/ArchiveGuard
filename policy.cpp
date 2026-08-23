#include "policy.hpp"

SecurityVerdict evaluatePolicy(const ArchiveStats& stats, const SecurityPolicy& policy){
    if (stats.uncompressedSize > policy.maxUncompressedSize){
        return SecurityVerdict::Block;
    }
    if (stats.fileCount > policy.maxFileCount){
        return SecurityVerdict::Block;
    }
    if (stats.largestFile > policy.maxSingleFileSize){
        return SecurityVerdict::Block;
    }
    return SecurityVerdict::Allow;
}