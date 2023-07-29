//
// Created by zyzshishui on 23-7-18.
//

#ifndef IDS_FLOWGENERATOR_H
#define IDS_FLOWGENERATOR_H


#include "BasicPacketInfo.h"
#include "BasicFlow.h"
#include "FlowListener.h"
#include <unordered_map>
#include <vector>

class FlowGenerator {
private:
    FlowListener* mListener = nullptr;
    std::unordered_map<std::string, BasicFlow> currentFlows;
    std::unordered_map<int, BasicFlow> finishedFlows;
    std::unordered_map<std::string, std::vector<BasicPacketInfo>> IPAddresses;

    bool bidirectional;
    long flowTimeOut = 0;
    long flowActivityTimeOut = 0;
    int finishedFlowCount = 0;

public:
    FlowGenerator(bool bidirectional, long flowTimeout, long activityTimeout);

    void init();

    void addFlowListener(FlowListener* listener);

    void addPacket(BasicPacketInfo packetInfo);

//    void dumpFlowBasedFeatures(const std::string& path, const std::string& filename, const std::string& header);

    int dumpLabeledFlowBasedFeatures(const std::string& path, const std::string& filename, const std::string& header);

    long dumpLabeledCurrentFlow(const std::string& fileFullPath, const std::string& header);

private:
    int getFlowCount();
};



#endif //IDS_FLOWGENERATOR_H
