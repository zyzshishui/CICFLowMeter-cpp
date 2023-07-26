//
// Created by zyzshishui on 23-7-20.
//

#ifndef IDS_FILENAMEUTIL_H
#define IDS_FILENAMEUTIL_H


class FileNameUtil {
private:
    static int indexOfLastSeparator(const std::string &filename);

    static void failIfNullBytePresent(const std::string &path);

public:
    static std::string ensurePathSeparator(const std::string &path);

    static std::string getName(const std::string &filename);


};


#endif //IDS_FILENAMEUTIL_H
