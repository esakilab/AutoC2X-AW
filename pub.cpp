#include "ros/ros.h"
#include "autoware_msgs/DetectedObjectArray.h"
#include "sensor_msgs/PointCloud2.h"
#include "sensor_msgs/PointCloud.h"
#include "sensor_msgs/point_cloud_conversion.h"
#include <random>

std::vector<geometry_msgs::Point32> createConvexHull(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4){
    std::random_device rnd;

	std::vector<geometry_msgs::Point32> result;

	geometry_msgs::Point32 a1, a2, a3, a4;

	a1.x = x1;
	a1.y = y1;
	a1.z = 0;
	
	a2.x = x2;
	a2.y = y2;
	a2.z = 0;

	a3.x = x3;
	a3.y = y3;
	a3.z = 0;

	a4.x = x4;
	a4.y = y4;
	a4.z = 0;

	result.push_back(a1);
	result.push_back(a2);
	result.push_back(a3);
	result.push_back(a4);
	result.push_back(a1);

	return result;
}

sensor_msgs::ChannelFloat32 createChannel(std::string name){
	sensor_msgs::ChannelFloat32 result;
	int N = 50;
	result.name = name;

	std::vector<float> values;
	for(int i=0; i<N*N*N ; i++){
		values.push_back(0.9);
	}

	result.values = values;
	return result;
}

std::vector<geometry_msgs::Point32> createLine(){
	std::vector<geometry_msgs::Point32> result;
	int N = 50;
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			for(int z = 0; z < N; z++){
				geometry_msgs::Point32 p;
				p.x = x*0.1 + 10;
				p.y = y*0.1;
				p.z = z*0.1;
				result.push_back(p);
			}
		}
	}
	return result;
}

autoware_msgs::DetectedObjectArray createObjectArray(){    
    std::random_device rnd;
	std::cout << "hello" << std::endl;
	autoware_msgs::DetectedObjectArray msg;
	std::vector<autoware_msgs::DetectedObject> objects;

	msg.header.frame_id = "velodyne";
	autoware_msgs::DetectedObject object;
	object.header.frame_id = "velodyne";
	object.label = "unknown";
	object.valid = 1;
	object.score = 1;
	object.space_frame = "velodyne";
	object.pose.position.x = 10;
	object.pose.position.y = 10;
	object.pose.orientation.w = 1;
	object.dimensions.x = 16.3;
	object.dimensions.y = 4.06;
	object.dimensions.z = 2.34;



	std::vector<geometry_msgs::Point32> points = createLine();
	std::vector<sensor_msgs::ChannelFloat32> channels;
	channels.push_back( createChannel("rgb") );
	sensor_msgs::PointCloud input_msg;
	sensor_msgs::PointCloud2 output_msg;
	input_msg.header.frame_id = "velodyne";
	input_msg.points = points;
	input_msg.channels = channels;
	sensor_msgs::convertPointCloudToPointCloud2(input_msg, output_msg);
	object.pointcloud = output_msg;

	geometry_msgs::PolygonStamped convex_hull_msg;
	convex_hull_msg.header.frame_id = "velodyne";
	geometry_msgs::Polygon polygon;
	polygon.points = createConvexHull((int)(rnd() % 10) -5, (int)(rnd() % 10) -5, (int)(rnd() % 10) -5, (int)(rnd() % 10) -5, (int)(rnd() % 10) -5, (int)(rnd() % 10) -5, (int)(rnd() % 10) -5, (int)(rnd() % 10) -5 );
	convex_hull_msg.polygon = polygon;
	object.convex_hull = convex_hull_msg;
    
    ros::Time nowTime = ros::Time::now();
    object.header.stamp = nowTime;
    object.pointcloud.header.stamp = nowTime;
    object.convex_hull.header.stamp = nowTime;
	objects.push_back(object);
	msg.objects = objects;
    msg.header.stamp = nowTime;

    return msg;
}

sensor_msgs::PointCloud2 createPointsPublisher(){
	// ros::NodeHandle n;
	// ros::Publisher pub = n.advertise<sensor_msgs::PointCloud2>("points_cluster", 1000);


	std::vector<geometry_msgs::Point32> points = createLine();
	std::vector<sensor_msgs::ChannelFloat32> channels;
	channels.push_back( createChannel("rgb") );

	
	sensor_msgs::PointCloud input_msg;
	sensor_msgs::PointCloud2 output_msg;

	// input_msg.header.stamp = nowTime;
	input_msg.header.frame_id = "velodyne";
	input_msg.points = points;
	input_msg.channels = channels;

	sensor_msgs::convertPointCloudToPointCloud2(input_msg, output_msg);
	ros::Time nowTime = ros::Time::now();
    output_msg.header.stamp = nowTime;
    return output_msg;
	
}

int main(int argc, char* argv[]){

    ros::init(argc, argv, "sampleTalker");
    ros::NodeHandle n;

    ros::Publisher chatter_pub = n.advertise<autoware_msgs::DetectedObjectArray>("detection/lidar_detector/objects", 10); 
    // ros::Publisher chatter_pub = n.advertise<sensor_msgs::PointCloud2>("/points_cluster", 10); 
    ros::Rate loop_rate(100);
    int count = 0;
    while(ros::ok()){
        
        autoware_msgs::DetectedObjectArray msg = createObjectArray();
        // sensor_msgs::PointCloud2 msg = createPointsPublisher();

        chatter_pub.publish(msg);
        loop_rate.sleep();
        ++count;
    }
    

    ros::spin();
    return 0;
}