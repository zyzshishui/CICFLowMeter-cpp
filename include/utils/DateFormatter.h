//
// Created by zyzshishui on 23-7-22.
//

#ifndef IDS_DATEFORMATTER_H
#define IDS_DATEFORMATTER_H

#include <string>

class DateFormatter {
public:
    static std::string parseDateFromLong(long long time, const std::string& format);
    static std::string convertMilliseconds2String(long long time, const char *format);
};

#endif //IDS_DATEFORMATTER_H
