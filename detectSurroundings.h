#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "sensor_msgs/PointCloud2.h"
#include "sensor_msgs/PointCloud.h"
#include "sensor_msgs/point_cloud_conversion.h"
#include "autoware_msgs/DetectedObjectArray.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/asio.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/thread.hpp>
#include <string>
#include "projects.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

struct message {
	int speed;
	int latitude;
	int longitude;
	int time;

};

struct socket_message{
	long timestamp;
	std::vector<int> speed;
	std::vector<int> latitude;
	std::vector<int> longitude;
	std::vector<int> time;

private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize( Archive& ar, unsigned int ver){
			ar & timestamp;
			ar & speed;
			ar & latitude;
			ar & longitude;
			ar & time;
		}
};



void init(ros::NodeHandle n);
void sendToRouter();
void calcEgovehicleState();
void timeCalc();
void callback(const geometry_msgs::PoseStamped msg);
void callback_objects(const autoware_msgs::DetectedObjectArray msg);
void sampleCallback(autoware_msgs::DetectedObjectArray msg);
std::string paramOrganize(std::string param);


ros::NodeHandle *n;
double speed;
double longitude;
double latitude;
float generationUnixTime;
long generationUnixTimeSec;
long generationUnixTimeNSec;
geometry_msgs::PoseStamped nowPose;
geometry_msgs::PoseStamped prevPose;

boost::thread *mThreadReceiveFromRouter;

PJ *p_proj;
std::ofstream delay_output_file;
std::ofstream one_two_delay_file;

int sockfd;

socket_message s_message;

struct timeval myTime;