//
// Created by zyzshishui on 23-7-18.
//

#include <fstream>
#include <iostream>
#include "FlowGenerator.h"
#include "FlowListener.h"

using acc_set = accumulator_set<double, features<tag::count, tag::sum, tag::mean, tag::variance, tag::min, tag::max>>;


FlowGenerator::FlowGenerator(bool bidirectional, long flowTimeout, long activityTimeout)
        : bidirectional(bidirectional), flowTimeOut(flowTimeout),
          flowActivityTimeOut(activityTimeout), finishedFlowCount(0) {
    init();
}

void FlowGenerator::init()
{
    currentFlows.clear();
    finishedFlows.clear();
    IPAddresses.clear();
    finishedFlowCount = 0;
}

void FlowGenerator::addFlowListener(FlowListener *listener) {
    mListener = listener;
}

void FlowGenerator::addPacket(BasicPacketInfo packetInfo) {
    if (packetInfo.getId() == 0) {
        return;
    }

    BasicFlow flow;
    long currentTimestamp = packetInfo.getTimeStamp();
    std::string id;

    if (currentFlows.find(packetInfo.fwdFlowId()) != currentFlows.end() ||
        currentFlows.find(packetInfo.bwdFlowId()) != currentFlows.end()) {
        if (currentFlows.find(packetInfo.fwdFlowId()) != currentFlows.end()) {
            id = packetInfo.fwdFlowId();
        } else {
            id = packetInfo.bwdFlowId();
        }

        flow = currentFlows[id];

        if ((currentTimestamp - flow.getFlowStartTime()) > flowTimeOut) {
            if (flow.packetCount() > 1) {
                if (mListener != nullptr) {
                    mListener->onFlowGenerated(flow);
                } else {
                    finishedFlows[getFlowCount()] = flow;
                }
            }
            currentFlows.erase(id);
            currentFlows[id] = BasicFlow(bidirectional, packetInfo, flow.getSrc(), flow.getDst(),
                                         flow.getSrcPort(), flow.getDstPort(), flowActivityTimeOut);

            int cfsize = currentFlows.size();
            if (cfsize % 50 == 0) {
                std::cout << "Timeout current has " << cfsize << " flows" << std::endl;
            }

        } else if (packetInfo.hasFlagFIN()) {
            if ((flow.getSrc() == packetInfo.getSrc() && flow.setFwdFINFlags() == 1) ||
                    (flow.getSrc() == packetInfo.getDst() && flow.setBwdFINFlags() == 1)) {
                if ((flow.getFwdFINFlags() + flow.getBwdFINFlags()) == 2) {
                    flow.addPacket(packetInfo);
                    if (mListener != nullptr) {
                        mListener->onFlowGenerated(flow);
                    } else {
                        finishedFlows[getFlowCount()] = flow;
                    }
                    currentFlows.erase(id);
                } else {
                    flow.updateActiveIdleTime(currentTimestamp, flowActivityTimeOut);
                    flow.addPacket(packetInfo);
                    currentFlows[id] = flow;
                }
            } else {
                if (flow.getFwdFINFlags() > 1) std::cout << "Forward flow received " << flow.getFwdFINFlags() << " FIN packets" << std::endl;
                else std::cout << "Backward flow received " << flow.getBwdFINFlags() << " FIN packets" << std::endl;
            }
        } else if (packetInfo.hasFlagRST()) {
            std::cout << "FlagRST current has " << currentFlows.size() << " flows" << std::endl;
            flow.addPacket(packetInfo);
            if (mListener != nullptr) {
                mListener->onFlowGenerated(flow);
            } else {
                finishedFlows[getFlowCount()] = flow;
            }
            currentFlows.erase(id);
        } else {
            if ((flow.getSrc() == packetInfo.getSrc()) && (flow.getFwdFINFlags() == 0)) {
                flow.updateActiveIdleTime(currentTimestamp, flowActivityTimeOut);
                flow.addPacket(packetInfo);
                currentFlows[id] = flow;
            } else if (flow.getBwdFINFlags() == 0) {
                flow.updateActiveIdleTime(currentTimestamp, flowActivityTimeOut);
                flow.addPacket(packetInfo);
                currentFlows[id] = flow;
            } else {
                std::cout << "FLOW already closed! fwdFIN " << flow.getFwdFINFlags()
                          << " bwdFIN " << flow.getBwdFINFlags() << std::endl;
            }
        }
    } else {
        currentFlows[packetInfo.fwdFlowId()] = BasicFlow(bidirectional, packetInfo, flowActivityTimeOut);
    }
}

//void FlowGenerator::dumpFlowBasedFeatures(const std::string& path, const std::string& filename,
//                                          const std::string& header)
//{
//    BasicFlow flow;
//
//    try {
//        std::ofstream output(path + filename);
//        output << header << "\n";
//
//        for (const auto& pair : finishedFlows) {
//            const BasicFlow& flow = pair.second;
//            if (flow.packetCount() > 1) {
//                output << flow.dumpFlowBasedFeaturesEx() << "\n";
//            }
//        }
//
//        for (const auto& pair : currentFlows) {
//            const BasicFlow& flow = pair.second;
//            if (flow.packetCount() > 1) {
//                output << flow.dumpFlowBasedFeaturesEx() << "\n";
//            }
//        }
//
//        output.close();
//    } catch (const std::exception& e) {
//        std::cerr << e.what() << std::endl;
//    }
//}

int FlowGenerator::dumpLabeledFlowBasedFeatures(const std::string& path, const std::string& filename,
                                                const std::string& header)      //TODO: NOT CHECKED YET
{
    BasicFlow flow;
    int total = 0;
    int zeroPkt = 0;

    try {
        std::ofstream output(path + filename);
        output << header << "\n";

        for (const auto& pair : finishedFlows) {
            const BasicFlow& flow = pair.second;
            if (flow.packetCount() > 1) {
                output << flow.dumpFlowBasedFeaturesEx() << "\n";
                total++;
            } else {
                zeroPkt++;
            }
        }

        for (const auto& pair : currentFlows) {
            const BasicFlow& flow = pair.second;
            if (flow.packetCount() > 1) {
                output << flow.dumpFlowBasedFeaturesEx() << "\n";
                total++;
            } else {
                zeroPkt++;
            }
        }

        output.close();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "dumpLabeledFlow finishedFlows -> " << zeroPkt << "," << total << std::endl;
    return total;
}

long FlowGenerator::dumpLabeledCurrentFlow(const std::string& fileFullPath, const std::string& header)
{
    std::ofstream output(fileFullPath, std::ios_base::app);
    int total = 0;

    if (!output) {
        std::cerr << "Failed to open file: " << fileFullPath << std::endl;
        return total;
    }

    try {
        output << header << "\n";
        for (const auto& pair : currentFlows) {
            const BasicFlow& flow = pair.second;
            if (flow.packetCount() > 1) {
                output << flow.dumpFlowBasedFeaturesEx() << "\n";
                total++;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    output.close();
    return total;
}

int FlowGenerator::getFlowCount()
{
    return ++finishedFlowCount;
}
