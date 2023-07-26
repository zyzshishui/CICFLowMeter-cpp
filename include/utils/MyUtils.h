//
// Created by zyzshishui on 23-7-16.
//

#ifndef IDS_MYUTILS_H
#define IDS_MYUTILS_H

#include <iostream>
#include <filesystem>
#include <string>
#include <TcpLayer.h>
#include <HttpLayer.h>
#include <fstream>
#include "Packet.h"


bool isPcapFile(const std::filesystem::directory_entry& entry);

long countLines(const std::string& fileName);

std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType);

std::string printTcpFlags(pcpp::TcpLayer* tcpLayer);

std::string printTcpOptionType(pcpp::TcpOptionType optionType);

std::string printHttpMethod(pcpp::HttpRequestLayer::HttpMethod httpMethod);


#endif //IDS_MYUTILS_H
