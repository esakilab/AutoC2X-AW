#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "autoware_msgs/DetectedObjectArray.h"
#include "sensor_msgs/PointCloud2.h"
#include "sensor_msgs/PointCloud.h"
#include "sensor_msgs/point_cloud_conversion.h"
#include "visualization_msgs/MarkerArray.h"
#include "tf2_msgs/TFMessage.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "tf2/LinearMath/Matrix3x3.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "projects.h"


PJ *p_proj;
geometry_msgs::PoseStamped nowPose;
geometry_msgs::PoseStamped prevPose;

ros::Publisher chatter_pub;

std::vector<geometry_msgs::PoseStamped> poses;

tf2::Quaternion rotated_position;
tf2::Quaternion original_position;

double roll, yaw, pitch;