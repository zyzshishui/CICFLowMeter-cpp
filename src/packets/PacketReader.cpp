#include <iostream>
#include <Packet.h>
#include <IPv4Layer.h>
#include "PacketReader.h"
#include "BasicPacketInfo.h"


PacketReader::PacketReader(const std::string &filename)
:pcapReader(filename) {
    config(filename);
}

PacketReader::PacketReader(const std::string &filename, bool readip4, bool readip6)
        : pcapReader(filename), readIP4(readip4), readIP6(readip6) {
    config(filename);
}

void PacketReader::config(const std::string& filename) {
    if (!pcapReader.open()) {
        throw std::runtime_error("Error while opening pcap file");
    }
}

bool PacketReader::getNextPacket(pcpp::RawPacket& rawPacket) {
    return pcapReader.getNextPacket(rawPacket);
}

BasicPacketInfo PacketReader::nextPacket(pcpp::RawPacket& rawPacket) {
    BasicPacketInfo packetInfo = BasicPacketInfo();

    try {
        pcpp::Packet packet(&rawPacket);
        packet.computeCalculateFields();

        if (packet.isPacketOfType(pcpp::IPv4)) {
            packetInfo = *getIpv4Info(packet);
        }
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return packetInfo;
}

BasicPacketInfo* PacketReader::getIpv4Info(const pcpp::Packet& packet) {
    try {
        const pcpp::Packet& parsedPacket(packet);
        auto* ipv4Layer = parsedPacket.getLayerOfType<pcpp::IPv4Layer>();
        if (ipv4Layer != nullptr) {
            BasicPacketInfo* packetInfo = new BasicPacketInfo(generator);
            packetInfo->setSrc(ipv4Layer->getSrcIPv4Address());
            packetInfo->setDst(ipv4Layer->getDstIPv4Address());
            packetInfo->setTimeStamp(packet.getRawPacket()->getPacketTimeStamp().tv_nsec * 1000);

            if (firstPacket == 0L)
                firstPacket = packet.getRawPacket()->getPacketTimeStamp().tv_nsec * 1000000;
            lastPacket = packet.getRawPacket()->getPacketTimeStamp().tv_nsec * 1000000;

            if (parsedPacket.isPacketOfType(pcpp::TCP)) {
                auto* tcpLayer = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
                packetInfo->setTCPWindow(tcpLayer->getTcpHeader()->windowSize);
                packetInfo->setSrcPort(tcpLayer->getTcpHeader()->portSrc);
                packetInfo->setDstPort(tcpLayer->getTcpHeader()->portDst);
                packetInfo->setProtocol(6);
                packetInfo->setFlagFIN(tcpLayer->getTcpHeader()->finFlag);
                packetInfo->setFlagPSH(tcpLayer->getTcpHeader()->pshFlag);
                packetInfo->setFlagURG(tcpLayer->getTcpHeader()->urgFlag);
                packetInfo->setFlagSYN(tcpLayer->getTcpHeader()->synFlag);
                packetInfo->setFlagACK(tcpLayer->getTcpHeader()->ackFlag);
                packetInfo->setFlagECE(tcpLayer->getTcpHeader()->eceFlag);
                packetInfo->setFlagCWR(tcpLayer->getTcpHeader()->cwrFlag);
                packetInfo->setFlagRST(tcpLayer->getTcpHeader()->rstFlag);
                packetInfo->setPayloadBytes(tcpLayer->getLayerPayloadSize());
                packetInfo->setHeaderBytes(tcpLayer->getHeaderLen());
            } else if (parsedPacket.isPacketOfType(pcpp::UDP)) {
                auto* udpLayer = parsedPacket.getLayerOfType<pcpp::UdpLayer>();
                packetInfo->setSrcPort(udpLayer->getUdpHeader()->portSrc);
                packetInfo->setDstPort(udpLayer->getUdpHeader()->portDst);
                packetInfo->setPayloadBytes(udpLayer->getLayerPayloadSize());
                packetInfo->setHeaderBytes(udpLayer->getHeaderLen());
                packetInfo->setProtocol(17);
            } else {
                /* Handle other packet types */
                // TODO: 原代码没实现（PacketReader.java #158）
            }
            packetInfo->setValid();
            return packetInfo;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }
    return nullptr;
}