
cmake_minimum_required(VERSION 2.8)

SET(PROJECT_VERSION_MAJOR "1")
SET(PROJECT_VERSION_MINOR "5")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")
SET(CMAKE_VERBOSE_MAKEFILE "ON")
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules) 
SET(CMAKE_INSTALL_PREFIX "/usr")

set(autoware_sender_SRCS
    checkMessage.cpp
)

add_executable (autoware_sender ${autoware_sender_SRCS})
target_link_libraries (autoware_sender boost_thread boost_system boost_serialization)