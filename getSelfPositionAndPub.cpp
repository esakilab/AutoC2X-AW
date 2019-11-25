#include "getSelfPositionAndPub.h"


using namespace std;

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

autoware_msgs::DetectedObjectArray createObjectArray(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4){    
    std::random_device rnd;
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
	polygon.points = createConvexHull(x1, y1, x2, y2, x3, y3, x4, y4);
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

std::string paramOrganize(std::string param){
	char **prm;
	std::string params;
    params=param+" no_defs";
    istringstream si(params);

    int n_pair=1;         // add one for last token "no_defs"
    for ( unsigned long i=0; i < params.size(); i++ )
        if ( params[i] == '=' ) n_pair++;

    prm = new char *[n_pair];
    for ( int i=0; i < n_pair; i++ ) {
        prm[i] = new char[256];
        si >> prm[i];
    }

    p_proj=pj_init(n_pair, prm);
    

    for ( int i=n_pair-1; i >=0; i-- ) {
        delete [] prm[i];
    }
    delete [] prm;

    if ( !p_proj ) {
        cerr << "Failed to initialize the PROJ library\n";
        exit(1);
    }

    return params;
 
}

projUV ll2xy( string lat, string lon ){
  projUV ll;
  ll.u = dmstor(lon.c_str(),0);
  ll.v = dmstor(lat.c_str(),0);
  return pj_fwd(ll, p_proj);
}

// void callback(const autoware_msgs::DetectedObjectArray msg){
void callback(const geometry_msgs::PoseStamped msg){
    prevPose = nowPose;
    nowPose = msg;

    float sin, cos, r, x1, x2, x3, y1, y2, y3, X, Y;
    // x1 = prevPose.pose.position.x;
    // y1 = prevPose.pose.position.y;
    // x2 = nowPose.pose.position.x;
    // y2 = nowPose.pose.position.y;
    // r = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
    // sin = (y2 - y1) / r;
    // cos = (x2 - x1) / r;

    float lat, lon, o_x, o_y;
    lat = 35.71419722;
    lon = 139.76148888;
    projUV obj_xy = ll2xy(to_string(lat), to_string(lon));


	float org_x, org_y, moved_x, moved_y, rotated_x, rotated_y;
	org_x = obj_xy.u;
	org_y = obj_xy.v;

	moved_x = org_x - nowPose.pose.position.x;
	moved_y = org_y - nowPose.pose.position.y;

	// rotated_x = moved_x * std::cos(yaw) + moved_y * std::sin(yaw);
	// rotated_y = -moved_x * std::sin(yaw) + moved_y * std::cos(yaw);

	rotated_x = moved_x * (std::cos(-yaw) * std::cos(-pitch)) + moved_y * (std::cos(-yaw) * std::sin(-pitch) * std::sin(-roll) - std::sin(-yaw) * std::cos(-roll));
	rotated_y = moved_x * (std::sin(-yaw) * std::cos(-pitch)) + moved_y * (std::sin(-yaw) * std::sin(-pitch) * std::sin(-roll) + std::cos(-yaw) * std::cos(-roll));

	std::cout << "yaw:" << yaw << " pitch:" << pitch << "roll:" << roll << std::endl;
	std::cout << "x:" << rotated_x << " y:" << rotated_y << std::endl;
    // x3 = obj_xy.u;
    // y3 = obj_xy.v;

    // Y = -(-(y3-y2)*cos + (x3-x2)*sin);
    // X = (y3-y2) * sin + (x3-x2) * cos;

    // std::cout << "dist:" << setprecision(4) <<  sqrt(X*X + Y*Y) <<  "m X:" << X << " Y:" << Y << std::endl;posestamped
	// std::cout << "x:" << msg.pose.orientation.x << " y:" << msg.pose.orientation.y << " z:" << msg.pose.orientation.z << " w:" << msg.pose.orientation.w << std::endl;

	X = rotated_x;
	Y = rotated_y;
    autoware_msgs::DetectedObjectArray pubMsg = createObjectArray(X, Y, X+10, Y, X+10, Y+10, X, Y+10);
    // autoware_msgs::DetectedObjectArray pubMsg = createObjectArray(0, 0, 10, 0, 10, 20, 0, 20);

    chatter_pub.publish(pubMsg);

}

void callback2(const tf2_msgs::TFMessage msg){
	if(msg.transforms[0].header.frame_id == "/map" && msg.transforms[0].child_frame_id == "/base_link"){
		tf2::Quaternion rot_q(msg.transforms[0].transform.rotation.x, msg.transforms[0].transform.rotation.y, msg.transforms[0].transform.rotation.z, msg.transforms[0].transform.rotation.w);
		tf2::Matrix3x3(rot_q).getRPY(roll, pitch, yaw);
	}
	// roll *= -1;
	// pitch *= -1;
	// yaw *= -1;
}

int main(int argc, char* argv[]){
    paramOrganize("proj=tmerc lat_0=36 lon_0=139.8333333333333 k=0.9999 x_0=0 y_0=0 ellps=GRS80 units=m");
    ros::init(argc, argv, "sampleCatcher");
    ros::NodeHandle n;

    // ros::Subscriber sub = n.subscribe("detection/lidar_detector/objects_markers", 1024, callback);
    ros::Subscriber sub = n.subscribe("ndt_pose", 1024, callback);
	ros::Subscriber sub2 = n.subscribe("tf", 1024, callback2);

    chatter_pub = n.advertise<autoware_msgs::DetectedObjectArray>("detection/lidar_detector/objects", 10); 

    ros::spin();
    return 0;
}