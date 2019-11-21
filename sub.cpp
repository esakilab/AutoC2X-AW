#include "ros/ros.h"
#include "autoware_msgs/DetectedObjectArray.h"
#include "visualization_msgs/MarkerArray.h"

// void callback(const autoware_msgs::DetectedObjectArray msg){
void callback(const visualization_msgs::MarkerArray msg){
    std::cout << ros::Time::now() << std::endl;
    // std::cout << ros::Time::now() << " " << ros::WallTime::now() <<   " listen:" << msg.objects.size() << std::endl;
    // for(int i = 0; i < msg.objects.size(); i++){
    //     std::cout << msg.objects[i].header.stamp.sec << "." << msg.objects[i].header.stamp.nsec << std::endl;
    //     break;
    // }
}

int main(int argc, char* argv[]){

    ros::init(argc, argv, "sampleCatcher");
    ros::NodeHandle n;

    // ros::Subscriber sub = n.subscribe("detection/lidar_detector/objects", 1024, callback);
    ros::Subscriber sub = n.subscribe("detection/lidar_detector/objects_markers", 1024, callback);
    // ros::Subscriber sub = n.subscribe("detection/objects", 1024, callback);
    // ros::Subscriber sub = n.subscribe("prediction/motion_predictor/path_markers", 1024, callback);

    ros::spin();
    return 0;
}