//
// Created by zyzshishui on 23-7-20.
//

#include <iostream>
#include <string>
#include "utils/FileNameUtil.h"

int FileNameUtil::indexOfLastSeparator(const std::string& filename) {
    int lastUnixPos = filename.find_last_of('/'); // ASCII value of '/'
    int lastWindowsPos = filename.find_last_of('\\'); // ASCII value of '\\'
    return std::max(lastUnixPos, lastWindowsPos);
}

void FileNameUtil::failIfNullBytePresent(const std::string& path) {
    size_t len = path.length();
    for (size_t i = 0; i < len; ++i) {
        if (path[i] == '\0') {
            throw std::invalid_argument("Null byte present in file/path name. There are no known legitimate use cases for such data, but several injection attacks may use it");
        }
    }
}

std::string FileNameUtil::ensurePathSeparator(const std::string& path) {
    char sep = '/'; // change this to '\\' for Windows
    if (!path.empty() && path.back() != sep) {
        return path + sep;
    }
    return path;
}


std::string FileNameUtil::getName(const std::string& filename) {
    if (filename.empty()) {
        return "";
    } else {
        failIfNullBytePresent(filename);
        int index = indexOfLastSeparator(filename);
        if (index != -1) {
            return filename.substr(index + 1);
        } else {
            return filename;
        }
    }
}