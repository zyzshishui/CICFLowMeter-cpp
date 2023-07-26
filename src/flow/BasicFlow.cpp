//
// Created by zyzshishui on 23-7-18.
//

#include "BasicFlow.h"
#include "BasicPacketInfo.h"
#include "DateFormatter.h"

using namespace boost::accumulators;
using acc_set = accumulator_set<double, features<tag::count, tag::sum, tag::mean, tag::variance, tag::min, tag::max>>;

const std::string BasicFlow::separator = ",";

BasicFlow::BasicFlow() {}

BasicFlow::BasicFlow(bool isBidirectional, BasicPacketInfo packet, pcpp::IPv4Address flowSrc, pcpp::IPv4Address flowDst,
                     int flowSrcPort, int flowDstPort, uint64_t activityTimeout)
        : fwdPktStats(fwdPktStats) {
    this->activityTimeout = activityTimeout;
    this->initParameters();
    this->isBidirectional = isBidirectional;
    this->firstPacket(packet);
    this->src = flowSrc;
    this->dst = flowDst;
    this->srcPort = flowSrcPort;
    this->dstPort = flowDstPort;
}

BasicFlow::BasicFlow(bool isBidirectional, BasicPacketInfo packet, uint64_t activityTimeout) {
    this->activityTimeout = activityTimeout;
    this->initParameters();
    this->isBidirectional = isBidirectional;
    this->firstPacket(packet);
}

BasicFlow::BasicFlow(BasicPacketInfo packet, uint64_t activityTimeout) {
    this->activityTimeout = activityTimeout;
    this->initParameters();
    this->isBidirectional = true;
    firstPacket(packet);
}

void BasicFlow::initParameters() {
    this->forward.clear();
    this->backward.clear();
    this->flowIAT = acc_set();
    this->forwardIAT = acc_set();
    this->backwardIAT = acc_set();
    this->flowActive = acc_set();
    this->flowIdle = acc_set();
    this->flowLengthStats = acc_set();
    this->fwdPktStats = acc_set();
    this->bwdPktStats = acc_set();
    this->flagCounts = std::unordered_map<std::string, int>();    //TODO: 原版是MutableInt
    this->initFlags();
    this->forwardBytes = 0;
    this->backwardBytes = 0;
    this->startActiveTime = 0;
    this->endActiveTime = 0;
    this->src = pcpp::IPv4Address::Zero;
    this->dst = pcpp::IPv4Address::Zero;
    this->fPSH_cnt = 0;
    this->bPSH_cnt = 0;
    this->fURG_cnt = 0;
    this->bURG_cnt = 0;
    this->fFIN_cnt = 0;
    this->bFIN_cnt = 0;
    this->fHeaderBytes = 0;
    this->bHeaderBytes = 0;
}

void BasicFlow::firstPacket(BasicPacketInfo packetInfo) {
    updateFlowBulk(packetInfo);
    detectUpdateSubflows(packetInfo);
    checkFlags(packetInfo);
    this->flowStartTime = packetInfo.getTimeStamp();
    this->flowLastSeen = packetInfo.getTimeStamp();
    this->startActiveTime = packetInfo.getTimeStamp();
    this->endActiveTime = packetInfo.getTimeStamp();
    this->flowLengthStats(static_cast<double>(packetInfo.getPayloadBytes()));

    if (this->src == pcpp::IPv4Address::Zero) {
        this->src = packetInfo.getSrc();
        this->srcPort = packetInfo.getSrcPort();
    }
    if (this->dst == pcpp::IPv4Address::Zero) {
        this->dst = packetInfo.getDst();
        this->dstPort = packetInfo.getDstPort();
    }
    if (this->src == packetInfo.getSrc()) {
        this->min_seg_size_forward = packetInfo.getHeaderBytes();
        this->Init_Win_bytes_forward = packetInfo.getTCPWindow();
        this->flowLengthStats(static_cast<double>(packetInfo.getPayloadBytes()));
        this->fwdPktStats(static_cast<double>(packetInfo.getPayloadBytes()));
        this->fHeaderBytes = packetInfo.getHeaderBytes();
        this->forwardLastSeen = packetInfo.getTimeStamp();
        this->forwardBytes += packetInfo.getPayloadBytes();
        this->forward.push_back(packetInfo);
        if (packetInfo.hasFlagPSH()) {
            this->fPSH_cnt++;
        }
        if (packetInfo.hasFlagURG()) {
            this->fURG_cnt++;
        }
    } else {
        this->Init_Win_bytes_backward = packetInfo.getTCPWindow();
        this->flowLengthStats(static_cast<double>(packetInfo.getPayloadBytes()));
        this->bwdPktStats(static_cast<double>(packetInfo.getPayloadBytes()));
        this->bHeaderBytes = packetInfo.getHeaderBytes();
        this->backwardLastSeen = packetInfo.getTimeStamp();
        this->backwardBytes += packetInfo.getPayloadBytes();
        this->backward.push_back(packetInfo);
        if (packetInfo.hasFlagPSH()) {
            this->bPSH_cnt++;
        }
        if (packetInfo.hasFlagURG()) {
            this->bURG_cnt++;
        }
    }
    this->protocol = packetInfo.getProtocol();
    this->flowId = packetInfo.getFlowId();
}

void BasicFlow::addPacket(BasicPacketInfo packet) {
    updateFlowBulk(packet);
    detectUpdateSubflows(packet);
    checkFlags(packet);
    long currentTimestamp = packet.getTimeStamp();
    if (isBidirectional) {
        this->flowLengthStats(static_cast<double>(packet.getPayloadBytes()));

        if (this->src == packet.getSrc()) {
            if (packet.getPayloadBytes() >= 1) {
                this->Act_data_pkt_forward++;
            }
            this->fwdPktStats(static_cast<double>(packet.getPayloadBytes()));
            this->fHeaderBytes += packet.getHeaderBytes();
            this->forward.push_back(packet);
            this->forwardBytes += packet.getPayloadBytes();
            if (this->forward.size() > 1)
                this->forwardIAT(currentTimestamp - this->forwardLastSeen);
            this->forwardLastSeen = currentTimestamp;
            this->min_seg_size_forward = std::min(packet.getHeaderBytes(), this->min_seg_size_forward);
        } else {
            this->bwdPktStats(static_cast<double>(packet.getPayloadBytes()));
            this->Init_Win_bytes_backward = packet.getTCPWindow();
            this->bHeaderBytes += packet.getHeaderBytes();
            this->backward.push_back(packet);
            this->backwardBytes += packet.getPayloadBytes();
            if (this->backward.size() > 1)
                this->backwardIAT(currentTimestamp - this->backwardLastSeen);
            this->backwardLastSeen = currentTimestamp;
        }
    } else {
        if (packet.getPayloadBytes() >= 1) {
            this->Act_data_pkt_forward++;
        }
        this->fwdPktStats(static_cast<double>(packet.getPayloadBytes()));
        this->flowLengthStats(static_cast<double>(packet.getPayloadBytes()));
        this->fHeaderBytes += packet.getHeaderBytes();
        this->forward.push_back(packet);
        this->forwardBytes += packet.getPayloadBytes();
        this->forwardIAT(currentTimestamp - this->forwardLastSeen);
        this->forwardLastSeen = currentTimestamp;
        this->min_seg_size_forward = std::min(packet.getHeaderBytes(), this->min_seg_size_forward);
    }

    this->flowIAT(packet.getTimeStamp() - this->flowLastSeen);
    this->flowLastSeen = packet.getTimeStamp();
}

double BasicFlow::getfPktsPerSecond() {
    uint64_t duration = this->flowLastSeen - this->flowStartTime;
    if (duration > 0) {
        return (this->forward.size() / (static_cast<double>(duration) / 1000000.0));
    } else {
        return 0.0;
    }
}

double BasicFlow::getbPktsPerSecond() {
    uint64_t duration = this->flowLastSeen - this->flowStartTime;
    if (duration > 0) {
        return (this->backward.size() / (static_cast<double>(duration) / 1000000.0));
    } else {
        return 0.0;
    }
}

double BasicFlow::getDownUpRatio() {
    if (this->forward.size() > 0) {
        return (static_cast<double>(this->backward.size()) / this->forward.size());
    }
    return 0.0;
}

double BasicFlow::getAvgPacketSize() {
    if (this->packetCount() > 0) {
        return (sum(this->flowLengthStats) / this->packetCount());
    }
    return 0.0;
}

double BasicFlow::fAvgSegmentSize() {
    if (this->forward.size() != 0) {
        return (sum(this->fwdPktStats) / static_cast<double>(this->forward.size()));
    }
    return 0.0;
}

double BasicFlow::bAvgSegmentSize() {
    if (this->backward.size() != 0) {
        return (sum(this->bwdPktStats) / static_cast<double>(this->backward.size()));
    }
    return 0.0;
}

void BasicFlow::initFlags() {
    flagCounts["FIN"] = 0;        //MutableInt();
    flagCounts["SYN"] = 0;        //MutableInt();
    flagCounts["RST"] = 0;        //MutableInt();
    flagCounts["PSH"] = 0;        //MutableInt();
    flagCounts["ACK"] = 0;        //MutableInt();
    flagCounts["URG"] = 0;        //MutableInt();
    flagCounts["CWR"] = 0;        //MutableInt();
    flagCounts["ECE"] = 0;        //MutableInt();
}

void BasicFlow::checkFlags(BasicPacketInfo packet) {
    if (packet.hasFlagFIN()) {
        flagCounts["FIN"]++;   //.increment();
    }
    if (packet.hasFlagSYN()) {
        flagCounts["SYN"]++;   //.increment();
    }
    if (packet.hasFlagRST()) {
        flagCounts["RST"]++;   //.increment();
    }
    if (packet.hasFlagPSH()) {
        flagCounts["PSH"]++;   //.increment();
    }
    if (packet.hasFlagACK()) {
        flagCounts["ACK"]++;   //.increment();
    }
    if (packet.hasFlagURG()) {
        flagCounts["URG"]++;   //.increment();
    }
    if (packet.hasFlagCWR()) {
        flagCounts["CWR"]++;   //.increment();
    }
    if (packet.hasFlagECE()) {
        flagCounts["ECE"]++;   //.increment();
    }
}

uint64_t BasicFlow::getSflow_fbytes() {
    if (sfCount <= 0) return 0;
    return this->forwardBytes / sfCount;
}

uint64_t BasicFlow::getSflow_fpackets() {
    if (sfCount <= 0) return 0;
    return this->forward.size() / sfCount;
}

uint64_t BasicFlow::getSflow_bbytes() {
    if (sfCount <= 0) return 0;
    return this->backwardBytes / sfCount;
}

uint64_t BasicFlow::getSflow_bpackets() {
    if (sfCount <= 0) return 0;
    return this->backward.size() / sfCount;
}

void BasicFlow::detectUpdateSubflows(BasicPacketInfo packetInfo) {
    sfLastPacketTS = packetInfo.getTimeStamp();
    sfAcHelper = packetInfo.getTimeStamp();

    if (((packetInfo.getTimeStamp() - sfLastPacketTS) / static_cast<double>(1000000)) > 1.0) {
        sfCount++;
        uint64_t lastSFduration = packetInfo.getTimeStamp() - sfAcHelper;
        updateActiveIdleTime(packetInfo.getTimeStamp(), this->activityTimeout);
        sfAcHelper = packetInfo.getTimeStamp();
    }

    sfLastPacketTS = packetInfo.getTimeStamp();
}

void BasicFlow::updateFlowBulk(BasicPacketInfo packet) {
    if (src == packet.getSrc()) {
        updateForwardBulk(packet, blastBulkTS);
    } else {
        updateBackwardBulk(packet, flastBulkTS);
    }
}

void BasicFlow::updateForwardBulk(BasicPacketInfo packet, long tsOflastBulkInOther) {
    long size = packet.getPayloadBytes();
    if (tsOflastBulkInOther > fbulkStartHelper) fbulkStartHelper = 0;
    if (size <= 0) return;

    packet.getPayloadPacket();

    if (fbulkStartHelper == 0) {
        fbulkStartHelper = packet.getTimeStamp();
        fbulkPacketCountHelper = 1;
        fbulkSizeHelper = size;
        flastBulkTS = packet.getTimeStamp();
    } else {
        if (((packet.getTimeStamp() - flastBulkTS) / static_cast<double>(1000000)) > 1.0) {
            fbulkStartHelper = packet.getTimeStamp();
            flastBulkTS = packet.getTimeStamp();
            fbulkPacketCountHelper = 1;
            fbulkSizeHelper = size;
        } else {
            fbulkPacketCountHelper += 1;
            fbulkSizeHelper += size;

            if (fbulkPacketCountHelper == 4) {
                fbulkStateCount += 1;
                fbulkPacketCount += fbulkPacketCountHelper;
                fbulkSizeTotal += fbulkSizeHelper;
                fbulkDuration += packet.getTimeStamp() - fbulkStartHelper;
            } else if (fbulkPacketCountHelper > 4) {
                fbulkPacketCount += 1;
                fbulkSizeTotal += size;
                fbulkDuration += packet.getTimeStamp() - flastBulkTS;
            }

            flastBulkTS = packet.getTimeStamp();
        }
    }
}

void BasicFlow::updateBackwardBulk(BasicPacketInfo packet, long tsOflastBulkInOther) {
    long size = packet.getPayloadBytes();
    if (tsOflastBulkInOther > bbulkStartHelper) bbulkStartHelper = 0;
    if (size <= 0) return;

    packet.getPayloadPacket();

    if (bbulkStartHelper == 0) {
        bbulkStartHelper = packet.getTimeStamp();
        bbulkPacketCountHelper = 1;
        bbulkSizeHelper = size;
        blastBulkTS = packet.getTimeStamp();
    } else {
        if (((packet.getTimeStamp() - blastBulkTS) / static_cast<double>(1000000)) > 1.0) {
            bbulkStartHelper = packet.getTimeStamp();
            blastBulkTS = packet.getTimeStamp();
            bbulkPacketCountHelper = 1;
            bbulkSizeHelper = size;
        } else {
            bbulkPacketCountHelper += 1;
            bbulkSizeHelper += size;

            if (bbulkPacketCountHelper == 4) {
                bbulkStateCount += 1;
                bbulkPacketCount += bbulkPacketCountHelper;
                bbulkSizeTotal += bbulkSizeHelper;
                bbulkDuration += packet.getTimeStamp() - bbulkStartHelper;
            } else if (bbulkPacketCountHelper > 4) {
                bbulkPacketCount += 1;
                bbulkSizeTotal += size;
                bbulkDuration += packet.getTimeStamp() - blastBulkTS;
            }

            blastBulkTS = packet.getTimeStamp();
        }
    }
}


double BasicFlow::fbulkDurationInSecond() {
    return fbulkDuration / static_cast<double>(1000000000);
}

long BasicFlow::fAvgBytesPerBulk() {
    if (fbulkStateCount != 0)
        return fbulkSizeTotal / fbulkStateCount;
    return 0;
}

long BasicFlow::fAvgPacketsPerBulk() {
    if (fbulkStateCount != 0)
        return fbulkPacketCount / fbulkStateCount;
    return 0;
}

long BasicFlow::fAvgBulkRate() {
    if (fbulkDuration != 0)
        return static_cast<long>(fbulkSizeTotal / fbulkDurationInSecond());
    return 0;
}

double BasicFlow::bbulkDurationInSecond() {
    return bbulkDuration / static_cast<double>(1000000);
}

long BasicFlow::bAvgBytesPerBulk() {
    if (bbulkStateCount != 0)
        return bbulkSizeTotal / bbulkStateCount;
    return 0;
}

long BasicFlow::bAvgPacketsPerBulk() {
    if (bbulkStateCount != 0)
        return bbulkPacketCount / bbulkStateCount;
    return 0;
}

long BasicFlow::bAvgBulkRate() {
    if (bbulkDuration != 0)
        return static_cast<long>(bbulkSizeTotal / bbulkDurationInSecond());
    return 0;
}

void BasicFlow::updateActiveIdleTime(long currentTime, long threshold) {
    if ((currentTime - endActiveTime) > threshold) {
        if ((endActiveTime - startActiveTime) > 0) {
            flowActive(endActiveTime - startActiveTime);
        }
        flowIdle(currentTime - endActiveTime);
        startActiveTime = currentTime;
        endActiveTime = currentTime;
    } else {
        endActiveTime = currentTime;
    }
}

void BasicFlow::endActiveIdleTime(long currentTime, long threshold, long flowTimeOut, bool isFlagEnd) {
    flowActive(endActiveTime - startActiveTime);

    if (!isFlagEnd && ((flowTimeOut - (endActiveTime - flowStartTime)) > 0)) {
        flowIdle(flowTimeOut - (endActiveTime - flowStartTime));
    }
}

std::string BasicFlow::dumpFlowBasedFeatures() {
    std::string dump;
    // Build the flows-based features dump string
    return dump;
}

int BasicFlow::packetCount() const {
    if (isBidirectional) {
        return forward.size() + backward.size();
    } else {
        return forward.size();
    }
}

std::vector<BasicPacketInfo> BasicFlow::getForward() {
    return forward;
}

void BasicFlow::setForward(const std::vector<BasicPacketInfo>& forward) {
    this->forward = forward;
}

std::vector<BasicPacketInfo> BasicFlow::getBackward() {
    return backward;
}

void BasicFlow::setBackward(const std::vector<BasicPacketInfo>& backward) {
    this->backward = backward;
}

void BasicFlow::setBidirectional(bool isBidirectional) {
    this->isBidirectional = isBidirectional;
}

pcpp::IPv4Address BasicFlow::getSrc() {
    return src;
}

void BasicFlow::setSrc(pcpp::IPv4Address src) {
    this->src = src;
}

pcpp::IPv4Address BasicFlow::getDst() {
    return dst;
}

void BasicFlow::setDst(pcpp::IPv4Address dst) {
    this->dst = dst;
}

int BasicFlow::getSrcPort() const {
    return srcPort;
}

void BasicFlow::setSrcPort(int srcPort) {
    this->srcPort = srcPort;
}

int BasicFlow::getDstPort() const {
    return dstPort;
}

void BasicFlow::setDstPort(int dstPort) {
    this->dstPort = dstPort;
}

int BasicFlow::getProtocol() const {
    return protocol;
}

std::string BasicFlow::getProtocolStr() const {
    switch (protocol) {
        case 6:
            return "TCP";
        case 17:
            return "UDP";
    }
    return "UNKNOWN";
}

void BasicFlow::setProtocol(int protocol) {
    this->protocol = protocol;
}

long BasicFlow::getFlowStartTime() const {
    return flowStartTime;
}

void BasicFlow::setFlowStartTime(long flowStartTime) {
    this->flowStartTime = flowStartTime;
}

std::string BasicFlow::getFlowId() const {
    return flowId;
}

void BasicFlow::setFlowId(const std::string& flowId) {
    this->flowId = flowId;
}

long BasicFlow::getLastSeen() const {
    return flowLastSeen;
}

void BasicFlow::setLastSeen(long lastSeen) {
    this->flowLastSeen = lastSeen;
}

long BasicFlow::getStartActiveTime() const {
    return startActiveTime;
}

void BasicFlow::setStartActiveTime(long startActiveTime) {
    this->startActiveTime = startActiveTime;
}

long BasicFlow::getEndActiveTime() const {
    return endActiveTime;
}

void BasicFlow::setEndActiveTime(long endActiveTime) {
    this->endActiveTime = endActiveTime;
}

long BasicFlow::getFlowDuration() const {
    return flowLastSeen - flowStartTime;
}

long BasicFlow::getTotalFwdPackets() const {
    return count(fwdPktStats);
}

long BasicFlow::getTotalBackwardPackets() const {
    return count(bwdPktStats);
}

double BasicFlow::getTotalLengthofFwdPackets() const {
    return sum(fwdPktStats);
}

double BasicFlow::getTotalLengthofBwdPackets() const {
    return sum(bwdPktStats);
}

double BasicFlow::getFwdPacketLengthMax() const {
    return (count(fwdPktStats) > 0) ? max(fwdPktStats) : 0;
}

double BasicFlow::getFwdPacketLengthMin() const {
    return (count(fwdPktStats) > 0) ? min(fwdPktStats) : 0;
}

double BasicFlow::getFwdPacketLengthMean() const {
    return (count(fwdPktStats) > 0) ? mean(fwdPktStats) : 0;
}

double BasicFlow::getFwdPacketLengthStd() const {
    return (count(fwdPktStats) > 0) ? sqrt(variance(fwdPktStats)) : 0;
}

double BasicFlow::getBwdPacketLengthMax() const {
    return (count(bwdPktStats) > 0) ? max(bwdPktStats) : 0;
}

double BasicFlow::getBwdPacketLengthMin() const {
    return (count(bwdPktStats) > 0) ? min(bwdPktStats) : 0;
}

double BasicFlow::getBwdPacketLengthMean() const {
    return (count(bwdPktStats) > 0) ? mean(bwdPktStats) : 0;
}

double BasicFlow::getBwdPacketLengthStd() const {
    return (count(bwdPktStats) > 0) ? sqrt(variance(bwdPktStats)) : 0;
}

double BasicFlow::getFlowBytesPerSec() const {
    return ((double)(forwardBytes + backwardBytes)) / ((double)getFlowDuration() / 1000000L);
}

double BasicFlow::getFlowPacketsPerSec() const {
    return ((double)packetCount()) / ((double)getFlowDuration() / 1000000L);
}

double BasicFlow::getFlowIATMean() const {
    return mean(flowIAT);
}

double BasicFlow::getFlowIATStd() const {
    return sqrt(variance(flowIAT));
}

double BasicFlow::getFlowIATMax() const {
    return max(flowIAT);
}

double BasicFlow::getFlowIATMin() const {
    return min(flowIAT);
}

double BasicFlow::getFwdIATTotal() const {
    return (forward.size() > 1) ? sum(forwardIAT) : 0;
}

double BasicFlow::getFwdIATMean() const {
    return (forward.size() > 1) ? mean(forwardIAT) : 0;
}

double BasicFlow::getFwdIATStd() const {
    return (forward.size() > 1) ? sqrt(variance(forwardIAT)) : 0;
}

double BasicFlow::getFwdIATMax() const {
    return (forward.size() > 1) ? max(forwardIAT) : 0;
}

double BasicFlow::getFwdIATMin() const {
    return (forward.size() > 1) ? min(forwardIAT) : 0;
}

double BasicFlow::getBwdIATTotal() const {
    return (backward.size() > 1) ? sum(backwardIAT) : 0;
}

double BasicFlow::getBwdIATMean() const {
    return (backward.size() > 1) ? mean(backwardIAT) : 0;
}

double BasicFlow::getBwdIATStd() const {
    return (backward.size() > 1) ? sqrt(variance(backwardIAT)) : 0;
}

double BasicFlow::getBwdIATMax() const {
    return (backward.size() > 1) ? max(backwardIAT) : 0;
}

double BasicFlow::getBwdIATMin() const {
    return (backward.size() > 1) ? min(backwardIAT) : 0;
}

int BasicFlow::getFwdPSHFlags() const {
    return fPSH_cnt;
}

int BasicFlow::getBwdPSHFlags() const {
    return bPSH_cnt;
}

int BasicFlow::getFwdURGFlags() const {
    return fURG_cnt;
}

int BasicFlow::getBwdURGFlags() const {
    return bURG_cnt;
}

int BasicFlow::getFwdFINFlags() const {
    return fFIN_cnt;
}

int BasicFlow::getBwdFINFlags() const {
    return bFIN_cnt;
}

int BasicFlow::setFwdFINFlags() {
    fFIN_cnt++;
    return fFIN_cnt;
}

int BasicFlow::setBwdFINFlags() {
    bFIN_cnt++;
    return bFIN_cnt;
}

long BasicFlow::getFwdHeaderLength() const {
    return fHeaderBytes;
}

long BasicFlow::getBwdHeaderLength() const {
    return bHeaderBytes;
}

double BasicFlow::getMinPacketLength() const {
    return (forward.size() > 0 || backward.size() > 0) ? min(flowLengthStats) : 0;
}

double BasicFlow::getMaxPacketLength() const {
    return (forward.size() > 0 || backward.size() > 0) ? max(flowLengthStats) : 0;
}

double BasicFlow::getPacketLengthMean() const {
    return (forward.size() > 0 || backward.size() > 0) ? mean(flowLengthStats) : 0;
}

double BasicFlow::getPacketLengthStd() const {
    return (forward.size() > 0 || backward.size() > 0) ? sqrt(variance(flowLengthStats)) : 0;
}

double BasicFlow::getPacketLengthVariance() const {
    return (forward.size() > 0 || backward.size() > 0) ? sqrt(variance(flowLengthStats)) : 0;
}

int BasicFlow::getFlagCount(const std::string& key) const {
    auto it = flagCounts.find(key);
    if (it != flagCounts.end()) {
        return it->second;
    }
    return 0;
}

int BasicFlow::getInit_Win_bytes_forward() const {
    return Init_Win_bytes_forward;
}

int BasicFlow::getInit_Win_bytes_backward() const {
    return Init_Win_bytes_backward;
}

long BasicFlow::getAct_data_pkt_forward() const {
    return Act_data_pkt_forward;
}

long BasicFlow::getmin_seg_size_forward() const {
    return min_seg_size_forward;
}

double BasicFlow::getActiveMean() const {
    return (count(flowActive) > 0) ? mean(flowActive) : 0;
}

double BasicFlow::getActiveStd() const {
    return (count(flowActive) > 0) ? sqrt(variance(flowActive)) : 0;
}

double BasicFlow::getActiveMax() const {
    return (count(flowActive) > 0) ? max(flowActive) : 0;
}

double BasicFlow::getActiveMin() const {
    return (count(flowActive) > 0) ? min(flowActive) : 0;
}

double BasicFlow::getIdleMean() const {
    return (count(flowIdle) > 0) ? mean(flowIdle) : 0;
}

double BasicFlow::getIdleStd() const {
    return (count(flowIdle) > 0) ? sqrt(variance(flowIdle)) : 0;
}

double BasicFlow::getIdleMax() const {
    return (count(flowIdle) > 0) ? max(flowIdle) : 0;
}

double BasicFlow::getIdleMin() const {
    return (count(flowIdle) > 0) ? min(flowIdle) : 0;
}

std::string BasicFlow::getLabel() const {
    return "NeedManualLabel";
}

std::string BasicFlow::dumpFlowBasedFeaturesEx() const {
    std::stringstream dump;
    dump << flowId << separator;                                              // 1
    dump << src << separator;                                // 2
    dump << getSrcPort() << separator;                                        // 3
    dump << dst << separator;                                // 4
    dump << getDstPort() << separator;                                        // 5
    dump << getProtocol() << separator;                                       // 6

    std::string starttime = DateFormatter::convertMilliseconds2String(flowStartTime, "%d/%m/%Y %H:%M:%S %p");  //TODO: wrong
    dump << starttime << separator;                                           // 7

    long flowDuration = flowLastSeen - flowStartTime;
    dump << flowDuration << separator;                                        // 8

    dump << count(fwdPktStats) << separator;                                   // 9
    dump << count(bwdPktStats) << separator;                                   // 10
    dump << sum(fwdPktStats) << separator;                                 // 11
    dump << sum(bwdPktStats) << separator;                                 // 12

    if (count(fwdPktStats) > 0) {
        dump << max(fwdPktStats) << separator;                                // 13
        dump << min(fwdPktStats) << separator;                                // 14
        dump << mean(fwdPktStats) << separator;                               // 15
        dump << sqrt(variance(fwdPktStats)) << separator;                      // 16
    } else {
        dump << 0 << separator;
        dump << 0 << separator;
        dump << 0 << separator;
        dump << 0 << separator;
    }

    if (count(bwdPktStats) > 0) {
        dump << max(bwdPktStats) << separator;                                // 17
        dump << min(bwdPktStats) << separator;                                // 18
        dump << mean(bwdPktStats) << separator;                               // 19
        dump << sqrt(variance(bwdPktStats)) << separator;                      // 20
    } else {
        dump << 0 << separator;
        dump << 0 << separator;
        dump << 0 << separator;
        dump << 0 << separator;
    }

    dump << ((double)(forwardBytes + backwardBytes)) / ((double)flowDuration / 1000000L) << separator;  // 21
    dump << ((double)packetCount()) / ((double)flowDuration / 1000000L) << separator;  // 22
    dump << mean(flowIAT) << separator;                                    // 23
    dump << sqrt(variance(flowIAT)) << separator;                           // 24
    dump << max(flowIAT) << separator;                                     // 25
    dump << min(flowIAT) << separator;                                     // 26

    if (forward.size() > 1) {
        dump << sum(forwardIAT) << separator;                               // 27
        dump << mean(forwardIAT) << separator;                              // 28
        dump << sqrt(variance(forwardIAT)) << separator;                     // 29
        dump << max(forwardIAT) << separator;                               // 30
        dump << min(forwardIAT) << separator;                               // 31
    } else {
        dump << 0 << separator;
        dump << 0 << separator;
        dump << 0 << separator;
        dump << 0 << separator;
        dump << 0 << separator;
    }

    if (backward.size() > 1) {
        dump << sum(backwardIAT) << separator;                              // 32
        dump << mean(backwardIAT) << separator;                             // 33
        dump << sqrt(variance(backwardIAT)) << separator;                    // 34
        dump << max(backwardIAT) << separator;                              // 35
        dump << min(backwardIAT) << separator;                              // 36
    } else {
        dump << 0 << separator;
        dump << 0 << separator;
        dump << 0 << separator;
        dump << 0 << separator;
        dump << 0 << separator;
    }

    dump << fPSH_cnt << separator;                                             // 37
    dump << bPSH_cnt << separator;                                             // 38
    dump << fURG_cnt << separator;                                             // 39
    dump << bURG_cnt << separator;                                             // 40

    dump << fHeaderBytes << separator;                                         // 41
    dump << bHeaderBytes << separator;                                         // 42
    dump << ((forward.size() > 0 || backward.size() > 0) ? min(flowLengthStats) : 0) << separator;  // 43
    dump << ((forward.size() > 0 || backward.size() > 0) ? max(flowLengthStats) : 0) << separator;  // 44
    dump << ((forward.size() > 0 || backward.size() > 0) ? mean(flowLengthStats) : 0) << separator;  // 45
    dump << ((forward.size() > 0 || backward.size() > 0) ? sqrt(variance(flowLengthStats)) : 0) << separator;  // 46
    dump << ((forward.size() > 0 || backward.size() > 0) ? variance(flowLengthStats) : 0) << separator;  // 47


    // ... continue dumping other features

    return dump.str();
}

