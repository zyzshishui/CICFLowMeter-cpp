#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "MyUtils.h"
#include "PacketReader.h"
#include "BasicPacketInfo.h"
#include "FlowGenerator.h"
#include "FlowListener.h"
#include "FileNameUtil.h"
#include "FlowFeature.h"

namespace fs = std::filesystem;


static std::string DividingLine = "-------------------------------------------------------------------------------";


void readPcapFile(const std::string& inputFile, std::string outPath, long flowTimeout, long activityTimeout) {
    if (inputFile.empty() || outPath.empty()) {
        return;
    }

    std::string fileName = FileNameUtil::getName(inputFile);

    outPath = FileNameUtil::ensurePathSeparator(outPath);

    std::string saveFileFullPath = outPath + fileName + ".csv";

    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cout << "Error: Unable to open input file: " << inputFile << std::endl;
        return;
    }

    std::ofstream outFile(saveFileFullPath, std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "Error: Unable to open output file: " << saveFileFullPath << std::endl;
        return;
    }

    FlowGenerator flowGen(true, flowTimeout, activityTimeout);
    flowGen.addFlowListener(new FlowListener(fileName, outPath));
    bool readIP6 = false;
    bool readIP4 = true;
    PacketReader packetReader(inputFile, readIP4, readIP6);
    std::cout << "Working on... "  << fileName << std::endl;
    int nValid=0;
    int nTotal=0;
    int nDiscarded = 0;
    pcpp::RawPacket rawPacket;
    while (packetReader.getNextPacket(rawPacket)) {
        try {
            BasicPacketInfo basicPacketInfo = packetReader.nextPacket(rawPacket);
            nTotal++;
        if(basicPacketInfo.checkValid()) {
            flowGen.addPacket(basicPacketInfo);
            nValid++;
        } else {
            nDiscarded++;
        }
        } catch (std::exception& e){
            std::cerr << e.what() << std::endl;
        }
    }
    flowGen.dumpLabeledCurrentFlow(saveFileFullPath, FlowFeature::getHeader());
    long lines = countLines(saveFileFullPath);

    std::cout << fileName<<" is done. total " << lines << " flows." << std::endl;
    std::cout << "Packet stats: Total = " << nTotal << ", Valid = " << nValid << ", Discarded = " << nDiscarded << std::endl;
    std::cout << DividingLine << std::endl;
}

void readPcapDir(const std::string& inputPath, const std::string& outPath, long flowTimeout, long activityTimeout) {
    if (inputPath.empty()) {
        return;
    }

    int file_cnt = 0;
    for (const auto& entry : fs::directory_iterator(inputPath)) {
        if (entry.is_regular_file() && isPcapFile(entry)) {
            file_cnt++;
        }
    }

    std::cout << "found " << file_cnt << " pcap files" << std::endl;
    int cur = 0;
    for (const auto& entry : fs::directory_iterator(inputPath)) {
        if (entry.is_regular_file() && isPcapFile(entry)) {
            cur++;
            std::cout << "==> " << cur << " / " << file_cnt << std::endl;
            readPcapFile(entry.path().string(), outPath, flowTimeout, activityTimeout);
        }
    }

    std::cout << "Completed!" << std::endl;
}



int main() {
    long flowTimeout = 120000000L;
    long activityTimeout = 5000000L;
    std::string pcapPath = "/home/zyzshishui/ids/pcap";
    std::string outPath = "/home/zyzshishui/ids/csv";
    if(!fs::exists(outPath)) {
        std::cout << "Csv path do not exist. Creating " << outPath << std::endl;
        if (fs::create_directory(outPath)) {
            std::cout << "Successfully created " << outPath << std::endl;
        } else {
            std::cerr << "Failed to create directory: " << outPath << std::endl;
            return 1;
        }
    }

    std::ifstream in(pcapPath, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Error opening pcap file: " << pcapPath << std::endl;
        return 1;
    }
    in.close();

//    std::ofstream out(outPath);
//    if (!out.is_open() || out.fail()) {
//        std::cerr << "Error opening csv file: " << outPath << std::endl;
//        return 1;
//    }
//    out.close();
    readPcapDir(pcapPath, outPath, flowTimeout, activityTimeout);

    return 0;
}
