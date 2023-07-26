//
// Created by zyzshishui on 23-7-18.
//

#ifndef IDS_BASICPACKETINFO_H
#define IDS_BASICPACKETINFO_H


#include <cstdint>
#include <vector>
#include <string>
#include "IdGenerator.h"
#include "MyUtils.h"

class BasicPacketInfo {
private:
    uint64_t id;
    pcpp::IPv4Address src;
    pcpp::IPv4Address dst;
    int srcPort;
    int dstPort;
    int protocol;
    long timeStamp;
    uint64_t payloadBytes;
    std::string flowId;
    bool flagFIN;
    bool flagPSH;
    bool flagURG;
    bool flagECE;
    bool flagSYN;
    bool flagACK;
    bool flagCWR;
    bool flagRST;
    int TCPWindow;
    uint64_t headerBytes;
    int payloadPacket;
    bool isValid = false;

public:
    BasicPacketInfo();

    BasicPacketInfo(const pcpp::IPv4Address src, const pcpp::IPv4Address dst, int srcPort, int dstPort,
                    int protocol, long timeStamp, IdGenerator& generator);

    explicit BasicPacketInfo(IdGenerator& generator);

    std::string generateFlowId();

    std::string fwdFlowId();

    std::string bwdFlowId();

    int getPayloadPacket();

    std::string getSourceIP();

    std::string getDestinationIP();

    uint64_t getId() const;

    void setId(uint64_t id);

    pcpp::IPv4Address getSrc() const;

    void setSrc(pcpp::IPv4Address src);

    pcpp::IPv4Address getDst() const;

    void setDst(pcpp::IPv4Address dst);

    int getSrcPort() const;

    void setSrcPort(int srcPort);

    int getDstPort() const;

    void setDstPort(int dstPort);

    int getProtocol() const;

    void setProtocol(int protocol);

    long getTimeStamp() const;

    void setTimeStamp(long timeStamp);

    const std::string& getFlowId() const;

    void setFlowId(const std::string& flowId);

    bool isForwardPacket(const pcpp::IPv4Address sourceIP) const;

    uint64_t getPayloadBytes() const;

    void setPayloadBytes(uint64_t payloadBytes);

    uint64_t getHeaderBytes() const;

    void setHeaderBytes(uint64_t headerBytes);

    bool hasFlagFIN() const;

    void setFlagFIN(bool flagFIN);

    bool hasFlagPSH() const;

    void setFlagPSH(bool flagPSH);

    bool hasFlagURG() const;

    void setFlagURG(bool flagURG);

    bool hasFlagECE() const;

    void setFlagECE(bool flagECE);

    bool hasFlagSYN() const;

    void setFlagSYN(bool flagSYN);

    bool hasFlagACK() const;

    void setFlagACK(bool flagACK);

    bool hasFlagCWR() const;

    void setFlagCWR(bool flagCWR);

    bool hasFlagRST() const;

    void setFlagRST(bool flagRST);

    int getTCPWindow() const;

    void setTCPWindow(int TCPWindow);

    void setValid();

    bool checkValid();
};


#endif //IDS_BASICPACKETINFO_H
