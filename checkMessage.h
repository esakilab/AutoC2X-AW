#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <chrono>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <boost/thread.hpp>
#include <sys/time.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

struct message {
	int speed;
	float latitude;
	float longitude;
	int time;
};

struct request{
	float timestamp;

private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize( Archive& ar, unsigned int ver){
			ar & timestamp;
		}
};


struct socket_message{
	long timestamp;
	std::vector<int> speed;
	std::vector<int> latitude;
	std::vector<int> longitude;
	std::vector<int> time;

private:
	friend class boost::serialization::access;
	template<class Archive>
		void serialize( Archive& ar, unsigned int ver){
			ar & timestamp;
			ar & speed;
			ar & latitude;
			ar & longitude;
			ar & time;
		}
};

void sendRequestToRouter();



double roll, yaw, pitch;

float lat, lon;

float speed, longitude, latitude, generationUnixTime;

boost::thread* mThreadReceive;

socket_message s_message;
socket_message r_message;

int sock_fd;
int flag;