#include <iomanip>
#include <iostream>
#include <string>
#include <cstdint>
#include <zip.h>
#include "archive_stats.hpp"
#include "policy.hpp"

int main(const int argc, const char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: archiveguard <archive.zip>\n";
        return 1;
    }

    const std::string archivePath = argv[1];
    int errorCode = 0;

    zip_t *archive = zip_open(
        archivePath.c_str(),
        ZIP_RDONLY,
        &errorCode
    );

    if (archive == nullptr){
        zip_error_t error;
        zip_error_init_with_code(&error, errorCode);
        std::cerr << "ArchiveGuard: failed to open archive: " << zip_error_strerror(&error) << std::endl;
        zip_error_fini(&error);
        return 1;
    }

    ArchiveStats stats;

    const zip_int64_t entryCount = zip_get_num_entries(archive, ZIP_FL_UNCHANGED);
    if (entryCount < 0) {
        std::cerr << "ArchiveGuard: failed to read archive entries" << std::endl;
        zip_close(archive);
        return 1;
    }


    for (zip_uint64_t i = 0; i < static_cast<zip_uint64_t>(entryCount); ++i){
        zip_stat_t entryStat;
        zip_stat_init(&entryStat);
        if(zip_stat_index(archive, i, ZIP_FL_UNCHANGED, &entryStat) != 0){
            std::cerr << "ArchiveGuard: failed to read entry " << i << std::endl;
            continue;
        }
        if(entryStat.name != nullptr && entryStat.name[0] != '\0' && entryStat.name[std::string(entryStat.name).size() - 1] == '/'){
            continue;
        }
        stats.fileCount++;
        stats.compressedSize += entryStat.comp_size;
        stats.uncompressedSize += entryStat.size;
        if (entryStat.size > stats.largestFile)
        {
            stats.largestFile = entryStat.size;
        }
    }

    zip_close(archive);
    double compressionRatio = 0.0;
    if (stats.compressedSize > 0){
        compressionRatio = static_cast<double>(stats.uncompressedSize) / static_cast<double>(stats.compressedSize);
    }

    constexpr SecurityPolicy policy;
    const SecurityVerdict verdict = evaluatePolicy(stats, policy);

    std::cout << std::endl;
    std::cout << "ArchiveGuard v0.1" << std::endl;
    std::cout << "=================" << std::endl;
    std::cout << std::endl;
    std::cout << "Archive: " << "  " << archivePath << std::endl;
    std::cout << std::endl;
    std::cout << "Archive statistics" << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "Files:              " << stats.fileCount << std::endl;
    std::cout << "Compressed size:    " << stats.compressedSize << " bytes" << std::endl;
    std::cout << "Uncompressed size:  " << stats.uncompressedSize << " bytes" << std::endl;
    std::cout << "Largest file:       " << stats.largestFile << " bytes" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Compression ratio:  " << compressionRatio << "x" << std::endl;

    std::cout << std::endl;
    std::cout << "Security policy" << std::endl;
    std::cout << "---------------" << std::endl;
    std::cout << "Maximum expanded size: " << policy.maxUncompressedSize << " bytes" << std::endl;
    std::cout << "Maximum file count:    " << policy.maxFileCount << std::endl;
    std::cout << "Maximum single file:   " << policy.maxSingleFileSize << " bytes" << std::endl;
    std::cout << std::endl;

    if (verdict == SecurityVerdict::Allow){
        std::cout << "Security verdict: ALLOW" << std::endl;
    }
    else{
        std::cout << "Security verdict: BLOCK\n" << std::endl;
    }

    std::cout << std::endl;
    return 0;
}