
cmake_minimum_required(VERSION 2.8)

SET(PROJECT_VERSION_MAJOR "1")
SET(PROJECT_VERSION_MINOR "5")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")
SET(CMAKE_VERBOSE_MAKEFILE "ON")
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules) 
SET(CMAKE_INSTALL_PREFIX "/usr")

set(pub_SRCS
   pub.cpp
)

set(sub_SRCS
    sub.cpp
)

set(autoware_pub_SRCS
    getSelfPositionAndPub.cpp
)

include_directories(/opt/ros/kinetic/include)
include_directories(/home/oi/Autoware/ros/install/autoware_msgs/include)
link_directories(/opt/ros/kinetic/lib)
add_executable (pub ${pub_SRCS})
add_executable (sub ${sub_SRCS})
add_executable (autoware_pub ${autoware_pub_SRCS})
target_link_libraries (pub roscpp rosconsole rostime roscpp_serialization)
target_link_libraries (sub roscpp rosconsole rostime roscpp_serialization)
target_link_libraries (autoware_pub roscpp rosconsole rostime roscpp_serialization proj boost_thread boost_system boost_serialization)