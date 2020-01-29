#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "sensor_msgs/PointCloud2.h"
#include "sensor_msgs/PointCloud.h"
#include "sensor_msgs/point_cloud_conversion.h"
#include "autoware_msgs/DetectedObjectArray.h"
#include "tf2_msgs/TFMessage.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
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
#include <random>
#include "visualization_msgs/MarkerArray.h"
// #include "tf2_geometry_msgs/tf2_geometry_msgs.h"

#include <sstream>
#include <cmath>
#include <unistd.h>
#include <cstdlib>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



struct socket_message_sender{
	long timestamp;
	std::vector<int> speed;
	std::vector<int> latitude;
	std::vector<int> longitude;
	std::vector<int> time;
	std::vector<int> stationid;

private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize( Archive& ar, int version){
			ar & timestamp;
			ar & speed;
			ar & latitude;
			ar & longitude;
			ar & time;
			ar & stationid;
		}
};

struct socket_message_receiver{
	long timestamp;
	float lat;
	float lon;
	std::vector<int> speed;
	std::vector<int> latitude;
	std::vector<int> longitude;
	std::vector<int> time;

private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize( Archive& ar, unsigned int ver){
			ar & timestamp;
			ar & lat;
			ar & lon;
			ar & speed;
			ar & latitude;
			ar & longitude;
			ar & time;
		}
};




/*
	receiver only
*/
std::vector<geometry_msgs::Point32> createConvexHull(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
sensor_msgs::ChannelFloat32 createChannel(std::string name);
std::vector<geometry_msgs::Point32> createLine();
autoware_msgs::DetectedObjectArray createObjectArray(std::vector<float> X, std::vector<float> Y);
std::string paramOrganize(std::string param);
projUV ll2xy( std::string lat, std::string lon );
void sendBackToRouter();
void output_file_config();
void receiveFromRouterAtReceiver();

ros::Publisher chatter_pub;
tf2::Quaternion rotated_position;
tf2::Quaternion original_position;
float lat, lon;
std::vector<geometry_msgs::Point32> box_line;
sensor_msgs::ChannelFloat32 channel;
int sock_fd;
int flag;
socket_message_receiver r_message;
boost::thread* mThreadReceive;


/*
	sender only
*/
void sendToRouter();
void calcEgovehicleState();
void timeCalc();
void createSocket(std::string router_addr);
void createFolder();
void callbackDetectionObjects(const autoware_msgs::DetectedObjectArray msg);
void receiveFromRouterAtSender();

long generationUnixTimeSec;
long generationUnixTimeNSec;
int sockfd;
boost::thread *mThreadReceiveFromRouter;
std::ofstream one_two_delay_file;
std::ofstream timestamp_record_file;
socket_message_sender s_message;


/*
	common 
*/
void callbackNdtPose(const geometry_msgs::PoseStamped msg);
void callbackTF(const tf2_msgs::TFMessage msg);
void loadOpt(int argc, char* argv[]);
std::string paramOrganize(std::string param);


double speed;
double longitude;
double latitude;
geometry_msgs::PoseStamped nowPose;
geometry_msgs::PoseStamped prevPose;
float generationUnixTime;
bool isSender;
std::string router_addr;
PJ *p_proj;
double roll, yaw, pitch;
std::ofstream delay_output_file;
struct timeval myTime;

std::random_device rnd;
std::mt19937 mt;

