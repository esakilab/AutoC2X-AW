#include "ros/ros.h"
#include "autoware_msgs/DetectedObjectArray.h"
#include "visualization_msgs/MarkerArray.h"
#include "tf2_msgs/TFMessage.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

void callback(const autoware_msgs::DetectedObjectArray msg){
// void callback(const visualization_msgs::MarkerArray msg){
// void callback(const tf2_msgs::TFMessage msg){
    // std::cout << ros::Time::now() << std::endl;
    // std::cout << ros::Time::now() << " " << ros::WallTime::now() <<   " listen:" << msg.objects.size() << std::endl;
    // for(int i = 0; i < msg.objects.size(); i++){
    //     std::cout << msg.objects[i].header.stamp.sec << "." << msg.objects[i].header.stamp.nsec << std::endl;
    //     break;
    // }
    
    // for(int i = 0; i < msg.transforms.size(); i++){
    //     if(msg.transforms[i].header.frame_id == "/map" && msg.transforms[i].child_frame_id == "/base_link"){
    //         // std::cout << msg.transforms[i].header.frame_id << " -> " << msg.transforms[i].child_frame_id << std::endl;
    //         // std::cout << "transform:" <<  msg.transforms[i].transform.translation.x << " " << msg.transforms[i].transform.translation.y << msg.transforms[i].transform.translation.z << std::endl;
    //         // std::cout << "rotation:" << msg.transforms[i].transform.rotation.x << " " <<  msg.transforms[i].transform.rotation.y << " " <<  msg.transforms[i].transform.rotation.z << " " <<  msg.transforms[i].transform.rotation.w << std::endl;

    //         tf2::Quaternion rot_q(msg.transforms[i].transform.rotation.x, msg.transforms[i].transform.rotation.y, msg.transforms[i].transform.rotation.z, msg.transforms[i].transform.rotation.w);
    //         double roll, pitch, yaw;
    //         tf2::Matrix3x3(rot_q).getRPY(roll, pitch, yaw);
    //         // std::cout << "roll:" << roll << " yaw:" << yaw << " pitch:" << pitch << std::endl;
    //         std::cout << yaw << std::endl;
    //     }
    // }
}



void callback3(const geometry_msgs::PoseStamped msg){
    std::cout << "ndt received:" << msg.header.stamp.nsec    << std::endl;
}

void callback2(const tf2_msgs::TFMessage msg){
    // std::cout << "hey" << std::endl;
    std::cout << "tf received:" << msg.transforms[0].header.stamp.nsec    << std::endl;
}


int main(int argc, char* argv[]){

    ros::init(argc, argv, "sampleCatcher");
    ros::NodeHandle n;

    // ros::Subscriber sub = n.subscribe("detection/lidar_detector/objects", 1024, callback);
    // ros::Subscriber sub = n.subscribe("detection/lidar_detector/objects_markers", 1024, callback);
    // ros::Subscriber sub = n.subscribe("detection/objects", 1024, callback);
    // ros::Subscriber sub = n.subscribe("prediction/motion_predictor/path_markers", 1024, callback);
    ros::Subscriber sub3 = n.subscribe("ndt_pose", 1024, callback3);
	ros::Subscriber sub2 = n.subscribe("tf", 1024, callback2);

    ros::spin();
    return 0;
}