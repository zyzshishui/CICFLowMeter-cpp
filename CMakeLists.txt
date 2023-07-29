cmake_minimum_required(VERSION 3.24)
project(ids)

set(CMAKE_CXX_STANDARD 17)

set(PCAPPLUSPLUS_INCLUDE_DIR "/usr/include/pcapplusplus")
set(PCAPPLUSPLUS_LIB_DIR "/usr/lib/pcapplusplus")

file(GLOB_RECURSE SOURCE src/*.cpp)

add_executable(ids ${SOURCE})
target_link_directories(ids PRIVATE ${PCAPPLUSPLUS_LIB_DIR})
target_link_libraries(${PROJECT_NAME} Pcap++ Packet++ Common++ pcap pthread)

target_include_directories(ids PRIVATE
        ${PCAPPLUSPLUS_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/flows
        ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/include/packets
        ${CMAKE_CURRENT_SOURCE_DIR}/include/csv
        )