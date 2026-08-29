#include "path_security.hpp"

bool isPathSafe(const std::string& path) {

    if (path.empty()) {
        return false;
    }

    // Absolute Unix path
    if (path.front() == '/') {
        return false;
    }

    // Absolute Windows path / drive path
    if (
        path.size() >= 2 &&
        (
            (path[0] >= 'A' && path[0] <= 'Z') ||
            (path[0] >= 'a' && path[0] <= 'z')
        ) &&
        path[1] == ':'
    ) {
        return false;
    }

    // Treat both '/' and '\\' as path separators.
    std::size_t start = 0;

    while (start < path.size()) {

        std::size_t end = start;

        while (
            end < path.size() &&
            path[end] != '/' &&
            path[end] != '\\'
        ) {
            ++end;
        }

        const std::string component =
            path.substr(start, end - start);

        if (component == "..") {
            return false;
        }

        if (end == path.size()) {
            break;
        }

        start = end + 1;
    }

    return true;
}