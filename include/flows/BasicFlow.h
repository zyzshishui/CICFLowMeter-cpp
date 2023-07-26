//
// Created by zyzshishui on 23-7-18.
//

#ifndef IDS_BASICFLOW_H
#define IDS_BASICFLOW_H

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include "../packets/BasicPacketInfo.h"
#include "boost/accumulators/accumulators.hpp"
#include "boost/accumulators/statistics.hpp"

using namespace boost::accumulators;
using acc_set = accumulator_set<double, features<tag::count, tag::sum, tag::mean, tag::variance, tag::min, tag::max>>;

//class MutableInt {
//public:
//    int value;
//    MutableInt() : value(0) {}
//
//    void increment() {
//        value++;
//    }
//};

class BasicFlow {
private:
    static const std::string separator;
    acc_set fwdPktStats;
    acc_set bwdPktStats;
    std::vector<BasicPacketInfo> forward;
    std::vector<BasicPacketInfo> backward;

    uint64_t forwardBytes;
    uint64_t backwardBytes;
    uint64_t fHeaderBytes;
    uint64_t bHeaderBytes;

    bool isBidirectional;

    std::unordered_map<std::string, int> flagCounts;    //TODO: 原版是MutableInt

    int fPSH_cnt;
    int bPSH_cnt;
    int fURG_cnt;
    int bURG_cnt;
    int fFIN_cnt;
    int bFIN_cnt;

    uint64_t Act_data_pkt_forward;
    uint64_t min_seg_size_forward;
    int Init_Win_bytes_forward;
    int Init_Win_bytes_backward;

    pcpp::IPv4Address src;
    pcpp::IPv4Address dst;
    int srcPort;
    int dstPort;
    int protocol;
    long flowStartTime;
    long startActiveTime;
    long endActiveTime;
    std::string flowId;

    acc_set flowIAT;
    acc_set forwardIAT;
    acc_set backwardIAT;
    acc_set flowLengthStats;
    acc_set flowActive;
    acc_set flowIdle;

    long flowLastSeen;
    long forwardLastSeen;
    long backwardLastSeen;
    uint64_t activityTimeout;

    long sfLastPacketTS;
    int sfCount;
    long sfAcHelper;

    long flastBulkTS;
    long blastBulkTS;
    long fbulkStartHelper;
    long bbulkStartHelper;
    long fbulkPacketCountHelper = 0;
    long bbulkPacketCountHelper = 0;
    long fbulkSizeHelper = 0;
    long bbulkSizeHelper = 0;
    long fbulkStateCount = 0;
    long bbulkStateCount = 0;
    long fbulkPacketCount = 0;
    long bbulkPacketCount = 0;
    long fbulkSizeTotal = 0;
    long bbulkSizeTotal = 0;
    long fbulkDuration = 0;
    long bbulkDuration = 0;


public:
    BasicFlow();
    BasicFlow(bool isBidirectional, BasicPacketInfo packet, pcpp::IPv4Address flowSrc, pcpp::IPv4Address flowDst,
              int flowSrcPort, int flowDstPort, uint64_t activityTimeout);
    BasicFlow(bool isBidirectional, BasicPacketInfo packet, uint64_t activityTimeout);
    BasicFlow(BasicPacketInfo packet, uint64_t activityTimeout);

    void initParameters();
    void firstPacket(BasicPacketInfo packetInfo);
    void addPacket(BasicPacketInfo packet);
    double getfPktsPerSecond();
    double getbPktsPerSecond();
    double getDownUpRatio();
    double getAvgPacketSize();
    double fAvgSegmentSize();
    double bAvgSegmentSize();
    void initFlags();
    void checkFlags(BasicPacketInfo packet);
    uint64_t getSflow_fbytes();
    uint64_t getSflow_fpackets();
    uint64_t getSflow_bbytes();
    uint64_t getSflow_bpackets();
    void detectUpdateSubflows(BasicPacketInfo packetInfo);
    void updateFlowBulk(BasicPacketInfo packet);
    void updateForwardBulk(BasicPacketInfo packet, long tsOflastBulkInOther);
    void updateBackwardBulk(BasicPacketInfo packet, long tsOflastBulkInOther);
    double fbulkDurationInSecond();
    long fAvgBytesPerBulk();
    long fAvgPacketsPerBulk();
    long fAvgBulkRate();
    double bbulkDurationInSecond();
    long bAvgBytesPerBulk();
    long bAvgPacketsPerBulk();
    long bAvgBulkRate();
    void updateActiveIdleTime(long currentTime, long threshold);
    void endActiveIdleTime(long currentTime, long threshold, long flowTimeOut, bool isFlagEnd);
    std::string dumpFlowBasedFeatures();
    int packetCount() const;
    std::vector<BasicPacketInfo> getForward();
    void setForward(const std::vector<BasicPacketInfo>& forward);

    std::vector<BasicPacketInfo> getBackward();
    void setBackward(const std::vector<BasicPacketInfo>& backward);

    void setBidirectional(bool isBidirectional);

    pcpp::IPv4Address getSrc();
    void setSrc(pcpp::IPv4Address src);

    pcpp::IPv4Address getDst();
    void setDst(pcpp::IPv4Address dst);

    int getSrcPort() const;
    void setSrcPort(int srcPort);

    int getDstPort() const;
    void setDstPort(int dstPort);

    int getProtocol() const;
    std::string getProtocolStr() const;
    void setProtocol(int protocol);

    long getFlowStartTime() const;
    void setFlowStartTime(long flowStartTime);

    std::string getFlowId() const;
    void setFlowId(const std::string& flowId);

    long getLastSeen() const;
    void setLastSeen(long lastSeen);

    long getStartActiveTime() const;
    void setStartActiveTime(long startActiveTime);

    long getEndActiveTime() const;
    void setEndActiveTime(long endActiveTime);

    std::string getSrcIP() const;
    std::string getDstIP() const;
    std::string getTimeStamp() const;
    long getFlowDuration() const;
    long getTotalFwdPackets() const;
    long getTotalBackwardPackets() const;
    double getTotalLengthofFwdPackets() const;
    double getTotalLengthofBwdPackets() const;
    double getFwdPacketLengthMax() const;
    double getFwdPacketLengthMin() const;
    double getFwdPacketLengthMean() const;
    double getFwdPacketLengthStd() const;
    double getBwdPacketLengthMax() const;
    double getBwdPacketLengthMin() const;
    double getBwdPacketLengthMean() const;
    double getBwdPacketLengthStd() const;
    double getFlowBytesPerSec() const;
    double getFlowPacketsPerSec() const;

    double getFlowIATMean() const;
    double getFlowIATStd() const;
    double getFlowIATMax() const;
    double getFlowIATMin() const;

    double getFwdIATTotal() const;
    double getFwdIATMean() const;
    double getFwdIATStd() const;
    double getFwdIATMax() const;
    double getFwdIATMin() const;

    double getBwdIATTotal() const;
    double getBwdIATMean() const;
    double getBwdIATStd() const;
    double getBwdIATMax() const;
    double getBwdIATMin() const;

    int getFwdPSHFlags() const;
    int getBwdPSHFlags() const;
    int getFwdURGFlags() const;
    int getBwdURGFlags() const;
    int getFwdFINFlags() const;
    int getBwdFINFlags() const;
    int setFwdFINFlags();
    int setBwdFINFlags();

    long getFwdHeaderLength() const;
    long getBwdHeaderLength() const;

    double getMinPacketLength() const;
    double getMaxPacketLength() const;
    double getPacketLengthMean() const;
    double getPacketLengthStd() const;
    double getPacketLengthVariance() const;

    int getFlagCount(const std::string& key) const;

    int getInit_Win_bytes_forward() const;
    int getInit_Win_bytes_backward() const;
    long getAct_data_pkt_forward() const;
    long getmin_seg_size_forward() const;

    double getActiveMean() const;
    double getActiveStd() const;
    double getActiveMax() const;
    double getActiveMin() const;

    double getIdleMean() const;
    double getIdleStd() const;
    double getIdleMax() const;
    double getIdleMin() const;

    std::string getLabel() const;

    std::string dumpFlowBasedFeaturesEx() const;

};




#endif //IDS_BASICFLOW_H
