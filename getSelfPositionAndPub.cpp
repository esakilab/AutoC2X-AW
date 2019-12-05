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
	int N = 10;
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
	int N = 10;
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			for(int z = 0; z < N; z++){
				geometry_msgs::Point32 p;
				p.x = x*0.5 + 10;
				p.y = y*0.5;
				p.z = z*0.5;
				result.push_back(p);
			}
		}
	}
	return result;
}

autoware_msgs::DetectedObjectArray createObjectArray(std::vector<float> X, std::vector<float> Y){

    std::random_device rnd;
	autoware_msgs::DetectedObjectArray msg;
	std::vector<autoware_msgs::DetectedObject> objects;

	
	msg.header.frame_id = "velodyne";
	ros::Time nowTime = ros::Time::now();
	for(int i=0; i<X.size(); i++){
		 
		// std::chrono::system_clock::time_point t1, t2, t3, t4, t5, t6, t7, t8;

		float x1, y1, x2, y2, x3, y3, x4, y4;
		x1 = X[i];
		y1 = Y[i];
		x2 = X[i] + 10;
		y2 = Y[i];
		x3 = X[i]+10;
		y3 = Y[i]+10;
		x4 = X[i];
		y4 = Y[i]+10;


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



		// t1 = std::chrono::system_clock::now();
		std::vector<geometry_msgs::Point32> points = box_line;
		std::vector<sensor_msgs::ChannelFloat32> channels;
		// t2 = std::chrono::system_clock::now();
		channels.push_back( channel );
		// t3 = std::chrono::system_clock::now();
		sensor_msgs::PointCloud input_msg;
		sensor_msgs::PointCloud2 output_msg;
		// t4 = std::chrono::system_clock::now();
		input_msg.header.frame_id = "velodyne";
		input_msg.points = points;
		input_msg.channels = channels;
		sensor_msgs::convertPointCloudToPointCloud2(input_msg, output_msg);
		object.pointcloud = output_msg;
		// t5 = std::chrono::system_clock::now();



		geometry_msgs::PolygonStamped convex_hull_msg;
		convex_hull_msg.header.frame_id = "velodyne";
		geometry_msgs::Polygon polygon;
		polygon.points = createConvexHull(x1, y1, x2, y2, x3, y3, x4, y4);
		convex_hull_msg.polygon = polygon;
		object.convex_hull = convex_hull_msg;


		
		object.header.stamp = nowTime;
		object.pointcloud.header.stamp = nowTime;
		object.convex_hull.header.stamp = nowTime;
		objects.push_back(object);

		// t6 = std::chrono::system_clock::now();

		// break;

		// double e1, e2, e3, e4, e5, e6;
		// e1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
		// e2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3-t2).count();
		// e3 = std::chrono::duration_cast<std::chrono::milliseconds>(t4-t3).count();
		// e4 = std::chrono::duration_cast<std::chrono::milliseconds>(t5-t4).count();
		// e5 = std::chrono::duration_cast<std::chrono::milliseconds>(t6-t5).count();
		// std::cout << "e1:" << e1 << " e2:" << e2 << " e3:" << e3 << " e4:" << e4 << " e5:" << e5 << std::endl;
	}
	
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

void callback3(const geometry_msgs::PoseStamped msg){
    prevPose = nowPose;
    nowPose = msg;

	std::vector<float> X, Y;

	for(int i=0; i<s_message.latitude.size(); i++){
		projUV obj_xy = ll2xy(to_string(s_message.latitude[i]/10000000.0), to_string(s_message.longitude[i]/10000000.0));

		float org_x, org_y, moved_x, moved_y, rotated_x, rotated_y;
		org_x = obj_xy.u;
		org_y = obj_xy.v;

		moved_x = org_x - nowPose.pose.position.x;
		moved_y = org_y - nowPose.pose.position.y;

		std::cout << "i:" << i << " x_diff:" << moved_x << " y_diff:" << moved_y << std::endl;

		rotated_x = moved_x * (std::cos(-yaw) * std::cos(-pitch)) + moved_y * (std::cos(-yaw) * std::sin(-pitch) * std::sin(-roll) - std::sin(-yaw) * std::cos(-roll));
		rotated_y = moved_x * (std::sin(-yaw) * std::cos(-pitch)) + moved_y * (std::sin(-yaw) * std::sin(-pitch) * std::sin(-roll) + std::cos(-yaw) * std::cos(-roll));

		X.push_back(rotated_x);
		Y.push_back(rotated_y);
		// break;
	}

	autoware_msgs::DetectedObjectArray pubMsg = createObjectArray(X, Y);
	chatter_pub.publish(pubMsg);
}


void callback(const autoware_msgs::DetectedObjectArray msg){
	std::cout << s_message.latitude.size() << std::endl;
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

void receiveFromRouter(){
	std::cout << "*****receive setup" << std::endl;
	int sockfd;
    int client_sockfd;
    struct sockaddr_in addr;
    socklen_t len = sizeof( struct sockaddr_in );
    struct sockaddr_in from_addr;
    char buf[4096];
 
    memset( buf, 0, sizeof( buf ) );
    if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
        perror( "socket" );
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons( 23457 );
    addr.sin_addr.s_addr = INADDR_ANY;
    if( bind( sockfd, (struct sockaddr *)&addr, sizeof( addr ) ) < 0 ) perror( "bind" );
    if( listen( sockfd, SOMAXCONN ) < 0 ) perror( "listen" );
    if( ( client_sockfd = accept( sockfd, (struct sockaddr *)&from_addr, &len ) ) < 0 ) perror( "accept" );
 
    // 受信
    int rsize;
    while( 1 ) {
		std::stringstream ss;
		memset( buf, 0, sizeof( buf ) );
        rsize = recv( client_sockfd, buf, sizeof( buf ), 0 );
		ss << buf;

		boost::archive::text_iarchive archive(ss);
		archive >> s_message;

		// for(int i = 0; i < s_message.latitude.size(); i++){
		// 	std::cout << "lat:" << s_message.latitude[i] << " lon:" << s_message.longitude[i] << " speed:" << s_message.speed[i] << " time:" << s_message.time[i] << std::endl;
		// }

		
        if ( rsize == 0 ) {
            break;
        } else if ( rsize == -1 ) {
            perror( "recv" );
        }
		
    }
 
    close( client_sockfd );
    close( sockfd );
}

int main(int argc, char* argv[]){
    paramOrganize("proj=tmerc lat_0=36 lon_0=139.8333333333333 k=0.9999 x_0=0 y_0=0 ellps=GRS80 units=m");
    ros::init(argc, argv, "sampleCatcher");
    ros::NodeHandle n;

    // ros::Subscriber sub = n.subscribe("detection/lidar_detector/objects", 1024, callback);
	ros::Subscriber sub2 = n.subscribe("tf", 1024, callback2);
    ros::Subscriber sub3 = n.subscribe("ndt_pose", 1024, callback3);

    // lat = 35.71419722;
    // lon = 139.76148888;
	// lon = 120;
    chatter_pub = n.advertise<autoware_msgs::DetectedObjectArray>("detection/lidar_detector/objects", 10);

	box_line = createLine();
	channel = createChannel("rgb");

	// s_message.latitude.push_back(35.714464 * 10000000);
	// s_message.longitude.push_back(139.760606 * 10000000);
	// s_message.speed.push_back(100);
	// s_message.time.push_back(100);

	// s_message.latitude.push_back(35.71419722 * 10000000);
	// s_message.longitude.push_back(139.76148888 * 10000000);
	// s_message.speed.push_back(100);
	// s_message.time.push_back(100);

	// s_message.latitude.push_back(35.714497 * 10000000);
	// s_message.longitude.push_back(139.763014 * 10000000);
	// s_message.speed.push_back(100);
	// s_message.time.push_back(100);

	// s_message.latitude.push_back(35.713997 * 10000000);
	// s_message.longitude.push_back(139.760153 * 10000000);
	// s_message.speed.push_back(100);
	// s_message.time.push_back(100);

	// s_message.latitude.push_back(35.712992 * 10000000);
	// s_message.longitude.push_back(139.759819 * 10000000);
	// s_message.speed.push_back(100);
	// s_message.time.push_back(100);

	mThreadReceive = new boost::thread(boost::ref(receiveFromRouter)); 

    ros::spin();
    return 0;
}