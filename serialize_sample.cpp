#include <iostream>
#include <sstream>
#include <string>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

struct Pokemon {
    std::string name;
    int hp = 0;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(hp, name);
    }
};

struct socket_message{
	long timestamp;
	std::vector<int> speed;
	std::vector<int> latitude;
	std::vector<int> longitude;
	std::vector<int> time;
	std::vector<int> stationid;

	template<class Archive>
		void serialize( Archive& ar){
            ar(timestamp, speed, latitude, longitude, time, stationid);
		}
};



int main()
{
    socket_message msg;
    msg.timestamp = 10000;
    msg.longitude.push_back(1);
    msg.latitude.push_back(1);
    msg.time.push_back(1);
    msg.stationid.push_back(1);
    msg.speed.push_back(1);

    std::stringstream ss;
    {
        cereal::BinaryOutputArchive o_archive(ss);
        o_archive(msg);
    }

    std::cout << ss.str() << std::endl;

    socket_message recv_msg;
    cereal::BinaryInputArchive i_archive(ss);
    i_archive(recv_msg);

    std::cout << recv_msg.timestamp << std::endl;
    std::cout << recv_msg.longitude[0] << std::endl;


    // Pokemon pokemon;
    // pokemon.name = "PIKACHU";
    // pokemon.hp = 100;

    // std::stringstream ss;
    // {
    //     cereal::BinaryOutputArchive o_archive(ss);
    //     o_archive(pokemon);
    // }
    // std::cout << ss.str() << std::endl;

    // Pokemon pokemon_i;
    // cereal::BinaryInputArchive i_archive(ss);
    // i_archive(pokemon_i);

    // std::cout << pokemon_i.hp << std::endl;
    // std::cout << pokemon_i.name << std::endl;


#ifdef _MSC_VER
    system("pause");
#endif
    return 0;
}
