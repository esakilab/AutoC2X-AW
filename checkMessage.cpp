#include "checkMessage.h"

using namespace std;

void sendBackToRouter(){
    if(flag != 100){
        struct sockaddr_in addr;
        if( (sock_fd = socket( AF_INET, SOCK_STREAM, 0) ) < 0 ) perror("socket");
        addr.sin_family = AF_INET;
        addr.sin_port = htons(23458);
        addr.sin_addr.s_addr = inet_addr("192.168.1.1");
        connect(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
        flag = 100;
    }

    std::stringstream ss;
	boost::archive::text_oarchive archive(ss);
	archive << s_message;

	ss.seekg(0, ios::end);
	if( send( sock_fd, ss.str().c_str(), ss.tellp(), 0 ) < 0 ) {
			perror( "send" );
	} else {
	}
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

		std::cout << "receive from router" << std::endl;

		struct timeval myTime;    // time_t構造体を定義．1970年1月1日からの秒数を格納するもの
		gettimeofday(&myTime, NULL);
		long timestamp = myTime.tv_sec * 1000000 + myTime.tv_usec;

		for(int i=0; i<s_message.latitude.size(); i++){
			std::cout << "timestamp:" << s_message.timestamp << ",latitude:" << s_message.latitude[i] << ",longitude:" << s_message.longitude[i] << std::endl;
		}

        sendBackToRouter();

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

	mThreadReceive = new boost::thread(boost::ref(receiveFromRouter)); 

	while(1){
		sleep(1);
	}

    return 0;
}
