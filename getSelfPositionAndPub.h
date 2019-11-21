#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "autoware_msgs/DetectedObjectArray.h"
#include "visualization_msgs/MarkerArray.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "projects.h"


PJ *p_proj;
geometry_msgs::PoseStamped nowPose;
geometry_msgs::PoseStamped prevPose;