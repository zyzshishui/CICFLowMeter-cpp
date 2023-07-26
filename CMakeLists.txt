cmake_minimum_required(VERSION 3.25)
project(ids)

set(CMAKE_CXX_STANDARD 17)

set(PCAPPLUSPLUS_INCLUDE_DIR "/usr/include/pcapplusplus")
set(PCAPPLUSPLUS_LIB_DIR "/usr/lib/pcapplusplus")
include_directories(${PCAPPLUSPLUS_INCLUDE_DIR})
link_directories(${PCAPPLUSPLUS_LIB_DIR})

add_executable(ids src/main.cpp include/utils/MyUtils.h src/packets/PacketReader.cpp include/packets/PacketReader.h src/packets/BasicPacketInfo.cpp include/packets/BasicPacketInfo.h src/flow/FlowGenerator.cpp include/flows/FlowGenerator.h src/flow/BasicFlow.cpp include/flows/BasicFlow.h src/utils/FileNameUtil.cpp include/utils/FileNameUtil.h src/flow/FlowListener.cpp include/flows/FlowListener.h src/flow/FlowFeature.cpp include/flows/FlowFeature.h src/csv/InsertCsvRow.cpp include/csv/InsertCsvRow.h src/utils/IdGenerator.cpp include/utils/IdGenerator.h src/utils/DateFormatter.cpp include/utils/DateFormatter.h src/utils/MyUtils.cpp)

target_link_libraries(${PROJECT_NAME} Pcap++ Packet++ Common++ pcap pthread)

target_include_directories(ids PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flows
        ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/include/packets
        ${CMAKE_CURRENT_SOURCE_DIR}/include/csv
        )
