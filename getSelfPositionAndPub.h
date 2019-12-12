#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "autoware_msgs/DetectedObjectArray.h"
#include "sensor_msgs/PointCloud2.h"
#include "sensor_msgs/PointCloud.h"
#include "sensor_msgs/point_cloud_conversion.h"
#include "visualization_msgs/MarkerArray.h"
#include "tf2_msgs/TFMessage.h"
#include "tf2/LinearMath/Quaternion.h"
// #include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "tf2/LinearMath/Matrix3x3.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "projects.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <boost/thread.hpp>
#include <sys/time.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

struct message {
	int speed;
	float latitude;
	float longitude;
	int time;
};

struct request{
	float timestamp;

private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize( Archive& ar, unsigned int ver){
			ar & timestamp;
		}
};


struct socket_message{
	// long timestamp;
	std::vector<int> speed;
	std::vector<int> latitude;
	std::vector<int> longitude;
	std::vector<int> time;

private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize( Archive& ar, unsigned int ver){
			// ar & timestamp;
			ar & speed;
			ar & latitude;
			ar & longitude;
			ar & time;
		}
};

void sendRequestToRouter();


PJ *p_proj;
geometry_msgs::PoseStamped nowPose;
geometry_msgs::PoseStamped prevPose;

ros::Publisher chatter_pub;

std::vector<geometry_msgs::PoseStamped> poses;

tf2::Quaternion rotated_position;
tf2::Quaternion original_position;

double roll, yaw, pitch;

float lat, lon;

float speed, longitude, latitude, generationUnixTime;

boost::thread* mThreadReceive;

socket_message s_message;
socket_message r_message;

std::vector<geometry_msgs::Point32> box_line;
sensor_msgs::ChannelFloat32 channel;

std::ofstream delay_output_file;

int sock_fd;