//
// Created by zyzshishui on 23-7-20.
//

#ifndef IDS_FLOWLISTENER_H
#define IDS_FLOWLISTENER_H

#include <iostream>


class FlowListener {
private:
    std::string fileName;
    std::string outPath;
    long cnt = 0;

public:
    FlowListener(const std::string& fileName, const std::string& outPath) : fileName(fileName), outPath(outPath), cnt(0) {}

    void onFlowGenerated(const BasicFlow &flow);
};


#endif //IDS_FLOWLISTENER_H
