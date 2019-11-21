#include "getSelfPositionAndPub.h"


using namespace std;



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

projUV ll2xy( string lat, string lon )
{
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
    x1 = prevPose.pose.position.x;
    y1 = prevPose.pose.position.y;
    x2 = nowPose.pose.position.x;
    y2 = nowPose.pose.position.y;
    r = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
    sin = (y2 - y1) / r;
    cos = (x2 - x1) / r;

    float lat, lon, o_x, o_y;
    lat = 35.71399009527691959;
    lon = 139.76269524060614913;
    projUV obj_xy = ll2xy(to_string(lat), to_string(lon));

    x3 = obj_xy.u;
    y3 = obj_xy.v;
    
    X = -(y3-y2)*cos + (x3-x2)*sin;
    Y = (y3-y2) * sin + (x3-x2) * cos;

    std::cout << "dist:" << setprecision(4) <<  sqrt(X*X + Y*Y) <<  "m X:" << X << " Y:" << Y << std::endl;


}

int main(int argc, char* argv[]){
    paramOrganize("proj=tmerc lat_0=36 lon_0=139.8333333333333 k=0.9999 x_0=0 y_0=0 ellps=GRS80 units=m");
    ros::init(argc, argv, "sampleCatcher");
    ros::NodeHandle n;

    // ros::Subscriber sub = n.subscribe("detection/lidar_detector/objects_markers", 1024, callback);
    ros::Subscriber sub = n.subscribe("ndt_pose", 1024, callback);

    ros::spin();
    return 0;
}