#include <iostream>
#include "packets/BasicPacketInfo.h"
#include "utils/MyUtils.h"
#include "IdGenerator.h"


BasicPacketInfo::BasicPacketInfo() {

}

BasicPacketInfo::BasicPacketInfo(const pcpp::IPv4Address src, pcpp::IPv4Address dst, int srcPort, int dstPort,
                                 int protocol, long timeStamp, IdGenerator& generator)
        : id(0), src(src), dst(dst), srcPort(srcPort), dstPort(dstPort), protocol(protocol),
          timeStamp(timeStamp), payloadBytes(0), flagFIN(false), flagPSH(false),
          flagURG(false), flagECE(false), flagSYN(false), flagACK(false), flagCWR(false),
          flagRST(false), TCPWindow(0), headerBytes(0), payloadPacket(0)
{
    generateFlowId();
}

BasicPacketInfo::BasicPacketInfo(IdGenerator& generator)
{
    id = generator.nextId();
}

std::string BasicPacketInfo::generateFlowId()
{
    bool forward = true;

    if (src != dst) {
        if (dst < src) {
            forward = false;
        }

    }

    if (forward) {
        flowId = getSourceIP() + "-" + getDestinationIP() + "-" + std::to_string(srcPort) + "-"
                 + std::to_string(dstPort) + "-" + std::to_string(protocol);
    } else {
        flowId = getDestinationIP() + "-" + getSourceIP() + "-" + std::to_string(dstPort) + "-"
                 + std::to_string(srcPort) + "-" + std::to_string(protocol);
    }

    return flowId;
}

std::string BasicPacketInfo::fwdFlowId()
{
    flowId = getSourceIP() + "-" + getDestinationIP() + "-" + std::to_string(srcPort) + "-"
             + std::to_string(dstPort) + "-" + std::to_string(protocol);

    return flowId;
}

std::string BasicPacketInfo::bwdFlowId()
{
    flowId = getDestinationIP() + "-" + getSourceIP() + "-" + std::to_string(dstPort) + "-"
             + std::to_string(srcPort) + "-" + std::to_string(protocol);

    return flowId;
}

int BasicPacketInfo::getPayloadPacket()
{
    return ++payloadPacket;
}

std::string BasicPacketInfo::getSourceIP()
{
    return src.toString();
}

std::string BasicPacketInfo::getDestinationIP()
{
    return dst.toString();
}

uint64_t BasicPacketInfo::getId() const
{
    return id;
}

void BasicPacketInfo::setId(uint64_t id)
{
    this->id = id;
}

pcpp::IPv4Address BasicPacketInfo::getSrc() const {
    return src;
}

void BasicPacketInfo::setSrc(pcpp::IPv4Address src)
{
    this->src = src;
}

pcpp::IPv4Address BasicPacketInfo::getDst() const {
    return dst;
}

void BasicPacketInfo::setDst(pcpp::IPv4Address dst)
{
    this->dst = dst;
}

int BasicPacketInfo::getSrcPort() const
{
    return srcPort;
}

void BasicPacketInfo::setSrcPort(int srcPort)
{
    this->srcPort = srcPort;
}

int BasicPacketInfo::getDstPort() const
{
    return dstPort;
}

void BasicPacketInfo::setDstPort(int dstPort)
{
    this->dstPort = dstPort;
}

int BasicPacketInfo::getProtocol() const
{
    return protocol;
}

void BasicPacketInfo::setProtocol(int protocol)
{
    this->protocol = protocol;
}

long BasicPacketInfo::getTimeStamp() const
{
    return timeStamp;
}

void BasicPacketInfo::setTimeStamp(long timeStamp)
{
    this->timeStamp = timeStamp;
}

const std::string& BasicPacketInfo::getFlowId() const
{
    return flowId;
}

void BasicPacketInfo::setFlowId(const std::string& flowId)
{
    this->flowId = flowId;
}

bool BasicPacketInfo::isForwardPacket(const pcpp::IPv4Address sourceIP) const
{
    return src == sourceIP;
}

uint64_t BasicPacketInfo::getPayloadBytes() const
{
    return payloadBytes;
}

void BasicPacketInfo::setPayloadBytes(uint64_t payloadBytes)
{
    this->payloadBytes = payloadBytes;
}

uint64_t BasicPacketInfo::getHeaderBytes() const
{
    return headerBytes;
}

void BasicPacketInfo::setHeaderBytes(uint64_t headerBytes)
{
    this->headerBytes = headerBytes;
}

bool BasicPacketInfo::hasFlagFIN() const
{
    return flagFIN;
}

void BasicPacketInfo::setFlagFIN(bool flagFIN)
{
    this->flagFIN = flagFIN;
}

bool BasicPacketInfo::hasFlagPSH() const
{
    return flagPSH;
}

void BasicPacketInfo::setFlagPSH(bool flagPSH)
{
    this->flagPSH = flagPSH;
}

bool BasicPacketInfo::hasFlagURG() const
{
    return flagURG;
}

void BasicPacketInfo::setFlagURG(bool flagURG)
{
    this->flagURG = flagURG;
}

bool BasicPacketInfo::hasFlagECE() const
{
    return flagECE;
}

void BasicPacketInfo::setFlagECE(bool flagECE)
{
    this->flagECE = flagECE;
}

bool BasicPacketInfo::hasFlagSYN() const
{
    return flagSYN;
}

void BasicPacketInfo::setFlagSYN(bool flagSYN)
{
    this->flagSYN = flagSYN;
}

bool BasicPacketInfo::hasFlagACK() const
{
    return flagACK;
}

void BasicPacketInfo::setFlagACK(bool flagACK)
{
    this->flagACK = flagACK;
}

bool BasicPacketInfo::hasFlagCWR() const
{
    return flagCWR;
}

void BasicPacketInfo::setFlagCWR(bool flagCWR)
{
    this->flagCWR = flagCWR;
}

bool BasicPacketInfo::hasFlagRST() const
{
    return flagRST;
}

void BasicPacketInfo::setFlagRST(bool flagRST)
{
    this->flagRST = flagRST;
}

int BasicPacketInfo::getTCPWindow() const
{
    return TCPWindow;
}

void BasicPacketInfo::setTCPWindow(int TCPWindow)
{
    this->TCPWindow = TCPWindow;
}

void BasicPacketInfo::setValid() {
    isValid = true;
}

bool BasicPacketInfo::checkValid() {
    return isValid;
}

