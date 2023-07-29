//
// Created by zyzshishui on 23-7-23.
//

#include "MyUtils.h"

bool isPcapFile(const std::filesystem::directory_entry& entry) {
    return std::filesystem::is_regular_file(entry) && entry.path().extension() == ".pcap";
}

long countLines(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return 0;
    }

    long lineNumber = 0;
    std::string line;
    while (std::getline(file, line)) {
        lineNumber++;
    }

    return lineNumber-1;
}

std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType) {
    switch (protocolType) {
        case pcpp::Ethernet:
            return "Ethernet";
        case pcpp::IPv4:
            return "IPv4";
        case pcpp::IPv6:
            return "IPv6";
        case pcpp::IP:
            return "IP";
        case pcpp::TCP:
            return "TCP";
        case pcpp::UDP:
            return "UDP";
        case pcpp::HTTPRequest:     //TODO:决定几个http要不要合并
            return "HTTP Request";
        case pcpp::HTTPResponse:
            return "HTTP Response";
        case pcpp::HTTP:
            return "HTTP";
        case pcpp::ARP:
            return "ARP";
        case pcpp::VLAN:
            return "VLAN";
        case pcpp::ICMP:
            return "ICMP";
        case pcpp::PPPoESession:
            return "PPPoE Session";
        case pcpp::PPPoEDiscovery:
            return "PPPoE Discovery";
        case pcpp::PPPoE:
            return "PPPoE";
        case pcpp::DNS:
            return "DNS";
        case pcpp::MPLS:
            return "MPLS";
        case pcpp::GREv0:
            return "GREv0";
        case pcpp::GREv1:
            return "GREv1";
        case pcpp::GRE:
            return "GRE";
        case pcpp::PPP_PPTP:
            return "PPP for PPTP";
        case pcpp::SSL:
            return "SSL/TLS";
        case pcpp::SLL:
            return "SLL";
        case pcpp::DHCP:
            return "DHCP/BOOTP";
        case pcpp::NULL_LOOPBACK:
            return "Null/Loopback";
        case pcpp::IGMP:
            return "IGMP";
        case pcpp::IGMPv1:
            return "IGMPv1";
        case pcpp::IGMPv2:
            return "IGMPv2";
        case pcpp::IGMPv3:
            return "IGMPv3";
        case pcpp::GenericPayload:
            return "Generic Payload";
        case pcpp::VXLAN:
            return "VXLAN";
        case pcpp::SIPRequest:
            return "SIP Request";
        case pcpp::SIPResponse:
            return "SIP Response";
        case pcpp::SIP:
            return "SIP";
        case pcpp::SDP:
            return "SDP";
        case pcpp::PacketTrailer:
            return "Packet Trailer";
        case pcpp::Radius:
            return "RADIUS";
        case pcpp::GTPv1:
            return "GTPv1/GTP";
        case pcpp::EthernetDot3:
            return "IEEE 802.3 Ethernet";
        case pcpp::BGP:
            return "Border Gateway Protocol (BGP)";
        case pcpp::SSH:
            return "SSHv2";
        case pcpp::AuthenticationHeader:
            return "IPSec Authentication Header (AH)";
        case pcpp::ESP:
            return "IPSec Encapsulating Security Payload (ESP)";
        case pcpp::IPSec:
            return "IPSec";
        case pcpp::DHCPv6:
            return "DHCPv6";
        case pcpp::NTP:
            return "Network Time (NTP)";
        default:
            return "Unknown";
    }
}

std::string printTcpFlags(pcpp::TcpLayer* tcpLayer) {
    std::string result = "";
    if (tcpLayer->getTcpHeader()->synFlag == 1)
        result += "SYN ";
    if (tcpLayer->getTcpHeader()->ackFlag == 1)
        result += "ACK ";
    if (tcpLayer->getTcpHeader()->pshFlag == 1)
        result += "PSH ";
    if (tcpLayer->getTcpHeader()->cwrFlag == 1)
        result += "CWR ";
    if (tcpLayer->getTcpHeader()->urgFlag == 1)
        result += "URG ";
    if (tcpLayer->getTcpHeader()->eceFlag == 1)
        result += "ECE ";
    if (tcpLayer->getTcpHeader()->rstFlag == 1)
        result += "RST ";
    if (tcpLayer->getTcpHeader()->finFlag == 1)
        result += "FIN ";

    return result;
}

std::string printTcpOptionType(pcpp::TcpOptionType optionType) {
    switch (optionType)
    {
        case pcpp::PCPP_TCPOPT_NOP:
            return "NOP";
        case pcpp::PCPP_TCPOPT_TIMESTAMP:
            return "Timestamp";
        default:
            return "Other";
    }
}

std::string printHttpMethod(pcpp::HttpRequestLayer::HttpMethod httpMethod) {
    switch (httpMethod)
    {
        case pcpp::HttpRequestLayer::HttpGET:
            return "GET";
        case pcpp::HttpRequestLayer::HttpPOST:
            return "POST";
        default:
            return "Other";
    }
}
