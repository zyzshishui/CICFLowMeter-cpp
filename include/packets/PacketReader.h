//
// Created by zyzshishui on 23-7-17.
//

#ifndef IDS_PACKETREADER_H
#define IDS_PACKETREADER_H


#include "Packet.h"
#include "PacketReader.h"
#include "TcpLayer.h"
#include "UdpLayer.h"
#include "IPv4Layer.h"
#include "IPv6Layer.h"
#include "PcapFileDevice.h"
#include "TextBasedProtocol.h"
#include "BasicPacketInfo.h"
#include "IdGenerator.h"
#include "MyUtils.h"

class PacketReader {

private:
    IdGenerator  generator = IdGenerator();
    pcpp::PcapFileReaderDevice pcapReader;
    long firstPacket = 0L;
    long lastPacket = 0L;
//    pcpp::TcpLayer  tcp = pcpp::TcpLayer(nullptr, 0, nullptr, nullptr);
//    pcpp::UdpLayer udp = pcpp::UdpLayer(nullptr, 0, nullptr, nullptr);
//    pcpp::IPv4Layer  ipv4 = pcpp::IPv4Layer(nullptr, 0, nullptr, nullptr);
//    pcpp::IPv6Layer  ipv6 = pcpp::IPv6Layer(nullptr, 0, nullptr, nullptr);
    bool readIP4 = true;
    bool readIP6 = false;
    std::string file;

//    pcpp::HeaderField hdr;    //TODO: 怎么初始化hdr

    void config(const std::string& filename);

public:
    PacketReader(const std::string &filename);
    PacketReader(const std::string &filename, bool readip4, bool readip6);  //, pcpp::HeaderField hdr
    bool getNextPacket(pcpp::RawPacket& rawPacket);
    BasicPacketInfo nextPacket(pcpp::RawPacket& rawPacket);
    BasicPacketInfo* getIpv4Info(const pcpp::Packet& packet);
    BasicPacketInfo* getIpv6Info(const pcpp::Packet packet);

};


#endif //IDS_PACKETREADER_H
