#include <ros/ros.h>
#include <std_msgs/String.h>
#include <mcservice_msgs/Trajectory.h>
#include <mcservice_msgs/TrajectoryPoint.h>
#include <mcservice_msgs/Pose.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "mcservice");
  ros::NodeHandle nh;
  ros::Publisher trajectory_pub = nh.advertise<mcservice_msgs::Trajectory>("/ego_vehicle/planned_trajectory", 100);
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    mcservice_msgs::Trajectory trajectory;
    trajectory.trajectory.clear();
    for (int i = 0; i < 5; i++) {
      mcservice_msgs::TrajectoryPoint tp;
      tp.time = i;
      tp.pose.latitude = i;
      tp.pose.longitude = i;
      tp.pose.altitude = i;
      trajectory.trajectory.push_back(tp);
    }
    
    // ROS_INFO("publish: %s", trajectory.c_str());
    trajectory_pub.publish(trajectory);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
