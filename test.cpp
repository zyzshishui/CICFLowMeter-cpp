#include <iostream>
#include <TcpLayer.h>
#include <UdpLayer.h>
#include <EthLayer.h>
#include <SystemUtils.h>
#include <HttpLayer.h>
#include "IPv4Layer.h"
#include "Packet.h"
#include "PcapFileDevice.h"
#include "utils/MyUtils.h"

int main(int argc, char* argv[])
{
    // 打开pcap
    pcpp::IFileReaderDevice* reader = pcpp::IFileReaderDevice::getReader("/home/zyzshishui/CLionProjects/ids/Thursday500.pcap");

    if (reader == NULL)
    {
        std::cerr << "Cannot determine reader for file type" << std::endl;
        return 1;
    }
    if (!reader->open())
    {
        std::cerr << "Error opening the pcap file" << std::endl;
        return 1;
    }

    // 读取包
    pcpp::RawPacket rawPacket;
    while (reader->getNextPacket(rawPacket))
    {
        // 解析包
        pcpp::Packet parsedPacket(&rawPacket);

        // 确定层
        for (pcpp::Layer* curLayer = parsedPacket.getFirstLayer(); curLayer != NULL; curLayer = curLayer->getNextLayer())
        {
            std::cout
                    << "Layer type: " << getProtocolTypeAsString(curLayer->getProtocol()) << "; " // get layer type
                    << "Total data: " << curLayer->getDataLen() << " [bytes]; " // get total length of the layer
                    << "Layer data: " << curLayer->getHeaderLen() << " [bytes]; " // get the header length of the layer
                    << "Layer payload: " << curLayer->getLayerPayloadSize() << " [bytes]" // get the payload length of the layer (equals total length minus header length)
                    << std::endl;
        }

        // 分层处理
        if (parsedPacket.isPacketOfType(pcpp::Ethernet)) {
            auto *ethernetLayer = parsedPacket.getLayerOfType<pcpp::EthLayer>();
            if (ethernetLayer == nullptr) {
                std::cerr << "Something went wrong, couldn't find Ethernet layer" << std::endl;
                return 1;
            }
            std::cout
                    << std::endl
                    << "Source MAC address: " << ethernetLayer->getSourceMac() << std::endl
                    << "Destination MAC address: " << ethernetLayer->getDestMac() << std::endl
                    << "Ether type = 0x" << std::hex << pcpp::netToHost16(ethernetLayer->getEthHeader()->etherType) << std::endl;
        } else if (parsedPacket.isPacketOfType(pcpp::IPv4)) {
            auto* ipLayer = parsedPacket.getLayerOfType<pcpp::IPv4Layer>();
            if (ipLayer == nullptr)
            {
                std::cerr << "Something went wrong, couldn't find IPv4 layer" << std::endl;
                return 1;
            }
            std::cout << std::endl
                      << "Source IP address: " << ipLayer->getSrcIPAddress() << std::endl
                      << "Destination IP address: " << ipLayer->getDstIPAddress() << std::endl
                      << "IP ID: 0x" << std::hex << pcpp::netToHost16(ipLayer->getIPv4Header()->ipId) << std::endl
                      << "TTL: " << std::dec << (int)ipLayer->getIPv4Header()->timeToLive << std::endl;
        } else if (parsedPacket.isPacketOfType(pcpp::TCP)) {
            auto* tcpLayer = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
            if (tcpLayer == nullptr)
            {
                std::cerr << "Something went wrong, couldn't find TCP layer" << std::endl;
                return 1;
            }
            std::cout << std::endl
                      << "Source TCP port: " << tcpLayer->getSrcPort() << std::endl
                      << "Destination TCP port: " << tcpLayer->getDstPort() << std::endl
                      << "Window size: " << pcpp::netToHost16(tcpLayer->getTcpHeader()->windowSize) << std::endl
                      << "TCP flags: " << printTcpFlags(tcpLayer) << std::endl;
            std::cout << "TCP options: ";
            for (pcpp::TcpOption tcpOption = tcpLayer->getFirstTcpOption(); tcpOption.isNotNull(); tcpOption = tcpLayer->getNextTcpOption(tcpOption))
            {
                std::cout << printTcpOptionType(tcpOption.getTcpOptionType()) << " ";
            }
            std::cout << std::endl;
        } else if (parsedPacket.isPacketOfType(pcpp::UDP)) {
            auto* udpLayer = parsedPacket.getLayerOfType<pcpp::UdpLayer>();
            if (udpLayer != nullptr)
            {
                // extract source and dest ports
                uint16_t srcPort = udpLayer->getUdpHeader()->portSrc;
                uint16_t destPort = udpLayer->getUdpHeader()->portDst;
                // print source and dest ports
                std::cout
                        << "Source Port is " << srcPort << "; "
                        << "Dest Port is " << destPort
                        << std::endl;
            }
        } else if (parsedPacket.isPacketOfType(pcpp::HTTPRequest)) {
            auto* httpRequestLayer = parsedPacket.getLayerOfType<pcpp::HttpRequestLayer>();
            if (httpRequestLayer == nullptr)
            {
                std::cerr << "Something went wrong, couldn't find HTTP request layer" << std::endl;
                return 1;
            }
            std::cout << std::endl
                      << "HTTP method: " << printHttpMethod(httpRequestLayer->getFirstLine()->getMethod()) << std::endl
                      << "HTTP URI: " << httpRequestLayer->getFirstLine()->getUri() << std::endl;
            std::cout
                    << "HTTP host: " << httpRequestLayer->getFieldByName(PCPP_HTTP_HOST_FIELD)->getFieldValue() << std::endl
                    << "HTTP user-agent: " << httpRequestLayer->getFieldByName(PCPP_HTTP_USER_AGENT_FIELD)->getFieldValue() << std::endl
                    << "HTTP cookie: " << httpRequestLayer->getFieldByName(PCPP_HTTP_COOKIE_FIELD)->getFieldValue() << std::endl;
            std::cout << "HTTP full URL: " << httpRequestLayer->getUrl() << std::endl;
        }
        else if (parsedPacket.isPacketOfType(pcpp::ARP))
        {
            std::cout << "ARP Packet" << std::endl;
        }
        else if (parsedPacket.isPacketOfType(pcpp::ICMP))
        {
            std::cout << "ICMP Packet" << std::endl;
        }
        else
        {
            std::cout << "Unknown Protocol" << std::endl;
        }
    }


    // 关闭文件
    reader->close();

    return 0;
}