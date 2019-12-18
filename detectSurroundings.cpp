#include "detectSurroundings.h"
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <math.h>

using namespace std;

void init(ros::NodeHandle n) {
	paramOrganize("proj=tmerc lat_0=36 lon_0=139.8333333333333 k=0.9999 x_0=0 y_0=0 ellps=GRS80 units=m");

	char cur_dir[1024];
	getcwd(cur_dir, 1024);

	time_t t = time(nullptr);
	const tm* lt = localtime(&t);
	std::stringstream s;
	s<<"20";
	s<<lt->tm_year-100; //100を引くことで20xxのxxの部分になる
	s<<"-";
	s<<lt->tm_mon+1; //月を0からカウントしているため
	s<<"-";
	s<<lt->tm_mday; //そのまま
	s<<"_";
	s<<lt->tm_hour;
	s<<":";
	s<<lt->tm_min;
	s<<":";
	s<<lt->tm_sec;
	std::string timestamp = s.str();

	std::string filename = std::string(cur_dir) + "/../output/delay/" + timestamp + ".csv";
	std::cout << "filename:" << filename << std::endl;
	delay_output_file.open(filename, std::ios::out);

	filename = std::string(cur_dir) + "/../output/1_2_delay/" + timestamp + ".csv";
	std::cout << "filename:" << filename << std::endl;
	one_two_delay_file.open(filename, std::ios::out);

	//通信モードの時は使う
	struct sockaddr_in addr;
	if( (sockfd = socket( AF_INET, SOCK_STREAM, 0) ) < 0 ) perror( "socket" ); 
	addr.sin_family = AF_INET;
	addr.sin_port = htons( 23457 );
	addr.sin_addr.s_addr = inet_addr( "192.168.1.1" );
	// addr.sin_addr.s_addr = inet_addr( "10.0.0.1" );
	connect( sockfd, (struct sockaddr *)&addr, sizeof( struct sockaddr_in ) );

}


void sendToRouter(){
	 // データ送信
	
    gettimeofday(&myTime, NULL);
	s_message.timestamp = myTime.tv_sec * 1000000 + myTime.tv_usec;
    auto it1 = s_message.speed.begin();
    auto it2 = s_message.time.begin();
    auto it3 = s_message.longitude.begin();
    auto it4 = s_message.latitude.begin();
    auto it5 = s_message.stationid.begin();

    s_message.speed.insert(it1,speed * 100);
	s_message.time.insert(it2, ((generationUnixTimeSec*1000 + (int)generationUnixTimeNSec/1000000 - 1072850400000)) % 65536);
	s_message.longitude.insert(it3, longitude * 10000000);
	s_message.latitude.insert(it4, latitude * 10000000);
	s_message.stationid.insert(it5, 0);

	std::cout << "delay: " <<  s_message.timestamp << std::endl;


	std::stringstream ss;
	{
		boost::archive::text_oarchive archive(ss);
		archive << s_message;
	}

	std::cout << ss.str().c_str() << std::endl;

	ss.seekg(0, ios::end);
	if( send( sockfd, ss.str().c_str(), ss.tellp(), 0 ) < 0 ) {
			perror( "send" );
	} else {
	}
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

void calcEgovehicleState(){
		double prevTime = prevPose.header.stamp.sec + prevPose.header.stamp.nsec/1000000000.0;
		double nowTime = nowPose.header.stamp.sec + nowPose.header.stamp.nsec/1000000000.0;
		double timedelta = nowTime - prevTime;

		double d_x = pow(prevPose.pose.position.x - nowPose.pose.position.x, 2);
		double d_y = pow(prevPose.pose.position.y - nowPose.pose.position.y, 2);

		// double degToRad = M_PI / 180;
		projUV xy;
		xy.u = nowPose.pose.position.x;
		xy.v = nowPose.pose.position.y;
		projUV result = pj_inv(xy, p_proj);
		result.u /= DEG_TO_RAD;
		result.v /= DEG_TO_RAD;
		std::cout << std::setprecision(20) << result.v << "," << result.u << std::endl;

		longitude = result.u;
		latitude = result.v;

		speed = sqrt(d_x + d_y) / timedelta;
}

void timeCalc(){
	geometry_msgs::PoseStamped newestPose = nowPose;
	ros::Time  a2, a3;
	ros::WallTime a1, a4;
	a1 = ros::WallTime::now();
	a2 = ros::Time::now();
	long diffTimeSec = ((long)a1.sec - (long)a2.sec);
	long diffTimeNSec = ((long)a1.nsec - (long)a2.nsec);
	
	generationUnixTimeSec = (long)newestPose.header.stamp.sec + diffTimeSec;
	generationUnixTimeNSec = (long)newestPose.header.stamp.nsec + diffTimeNSec;
	if(generationUnixTimeNSec < 0){
		generationUnixTimeSec -= 1;
		generationUnixTimeNSec = 1000000000 + generationUnixTimeNSec;
	}
	if(generationUnixTimeNSec >= 1000000000){
		generationUnixTimeSec += 1;
		generationUnixTimeNSec -= 1000000000;
	}
	long delaySec = a1.sec - generationUnixTimeSec;
	long delayNSec = a1.nsec - generationUnixTimeNSec;
	if(delayNSec < 0){
		delaySec -= 1;
		delayNSec = 1000000000 + delayNSec;
	}
	delay_output_file <<  std::setprecision(20) <<  ros::WallTime::now() << "," << delayNSec / 1000000000.0 << std::endl;
}

void callback(const geometry_msgs::PoseStamped msg){
    std::cout << "hello" << std::endl;
	prevPose = nowPose;
	nowPose = msg;
	timeCalc();
	calcEgovehicleState();
	sendToRouter();
}

void callback_objects(const autoware_msgs::DetectedObjectArray msg){
	s_message.longitude.clear();
	s_message.latitude.clear();
	s_message.speed.clear();
	s_message.time.clear();
    s_message.stationid.clear();

    std::uniform_int_distribution<> rand(1, 99999);

	for(unsigned int i = 0; i < msg.objects.size(); i++){
		float sum_x = 0.0;
		float sum_y = 0.0;
		float sum_z = 0.0;
		for(unsigned int j = 0; j < msg.objects[i].convex_hull.polygon.points.size(); j++){
			sum_x += msg.objects[i].convex_hull.polygon.points[j].x;
			sum_y += msg.objects[i].convex_hull.polygon.points[j].y;
			sum_z += msg.objects[i].convex_hull.polygon.points[j].z;
		}
		sum_x /= (float)msg.objects[i].convex_hull.polygon.points.size();
		sum_y /= (float)msg.objects[i].convex_hull.polygon.points.size();
		sum_z /= (float)msg.objects[i].convex_hull.polygon.points.size();

		projUV xy;
		xy.u = sum_x;
		xy.v = sum_y;
		projUV result = pj_inv(xy, p_proj);
		result.u /= DEG_TO_RAD;
		result.v /= DEG_TO_RAD;
		// std::cout << std::setprecision(20) << result.v << "," << result.u << std::endl;

		s_message.longitude.push_back(result.u * 10000000);
		s_message.latitude.push_back(result.v * 10000000);
		s_message.speed.push_back(0);
		// s_message.time.push_back(((generationUnixTimeSec*1000 + (int)generationUnixTimeNSec/1000000 - 1072850400000)) % 65536);
		s_message.time.push_back(0);
		s_message.stationid.push_back(rand(mt));

        
		// std::cout << std::setprecision(20) <<  "lat:" << result.v * 10000000 << " lon:" << result.u * 10000000 << " time:" << (((long)generationUnixTimeSec*1000 + (long)generationUnixTimeNSec/1000000 - 1072850400000)) % 65536 << std::endl;
	}
    std::cout << "detected objects:" << msg.objects.size() << std::endl;
	// sendToRouter(); ここで送る必要はあるか？
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
    addr.sin_port = htons( 23458 );
    addr.sin_addr.s_addr = INADDR_ANY;
    if( bind( sockfd, (struct sockaddr *)&addr, sizeof( addr ) ) < 0 ) perror( "bind" );
    if( listen( sockfd, SOMAXCONN ) < 0 ) perror( "listen" );
    if( ( client_sockfd = accept( sockfd, (struct sockaddr *)&from_addr, &len ) ) < 0 ) perror( "accept" );
 
    // 受信
    int rsize;
    while( 1 ) {
		std::stringstream ss(std::ios::binary | std::ios::out | std::ios::in);
		memset( buf, 0, sizeof( buf ) );
        rsize = recv( client_sockfd, buf, sizeof( buf ), 0 );
		ss << buf;

		boost::archive::text_iarchive archive(ss);
		// cereal::BinaryInputArchive archive(ss);
		archive >> s_message;

        gettimeofday(&myTime, NULL);
		one_two_delay_file << myTime.tv_sec * 1000000 + myTime.tv_usec << "," << s_message.timestamp << std::endl;
        if ( rsize == 0 ) {
            break;
        } else if ( rsize == -1 ) {
            perror( "recv" );
        }
    }
    close( client_sockfd );
    close( sockfd );

}


int main(int argc,  char* argv[]) {
    mt = std::mt19937(rnd());
	sample.timestamp = 11400;
    mThreadReceiveFromRouter = new boost::thread(boost::ref(receiveFromRouter));

	ros::init(argc, argv, "listener");
	ros::NodeHandle n,n2;

	ros::Subscriber sub1 = n.subscribe("ndt_pose", 1024, callback);
	ros::Subscriber sub2 = n.subscribe("detection/lidar_detector/objects", 1024, callback_objects);
	init(n);

	std::cout << "hairuyo" << std::endl;
	ros::spin();
	std::cout << "detayo" << std::endl;
	return 0;
}



// void sampleCallback(autoware_msgs::DetectedObjectArray msg){
// 	std::cout << "listen:" << msg.header.stamp.sec << std::endl; 
// }