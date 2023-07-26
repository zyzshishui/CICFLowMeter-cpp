//
// Created by zyzshishui on 23-7-22.
//

#include "DateFormatter.h"
#include <iostream>
#include <ctime>

std::string DateFormatter::parseDateFromLong(long long time, const std::string& format) {
    try {
        std::time_t timestamp = static_cast<std::time_t>(time / 1000);
        std::tm* timeinfo = std::localtime(&timestamp);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), format.c_str(), timeinfo);
        return buffer;
    } catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return "dd/MM/yyyy hh:mm:ss";
    }
}

std::string DateFormatter::convertMilliseconds2String(long long time, const char *format) {
    if (format == nullptr) {
        format = "%d/%m/%Y %H:%M:%S %p";
    }

    std::time_t timestamp = static_cast<std::time_t>(time / 1000);
    std::tm* timeinfo = std::localtime(&timestamp);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), format, timeinfo);
    return std::string(buffer);
}
