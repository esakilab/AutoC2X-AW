# AutoC2X-AW

![movie](https://user-images.githubusercontent.com/23014935/76400182-39570c00-63c3-11ea-81cb-a6b84179406d.gif)

AutoC2X is cooperative awareness driving software, extension for Autoware and OpenC2X. AutoC2X-AW is extension for Autoware. If you find this code useful in your research, please consider citing :

    @inproceedings{Tsukada2020,
        title = {AutoC2X: Open-source software to realize V2X cooperative perception among autonomous vehicles},
        author = {Manabu Tsukada and Takaharu Oi and Akihide Ito and Mai Hirata and Hiroshi Esaki},
        year = {2020},
        date = {2020-10-04},
        booktitle = {The 2020 IEEE 92nd Vehicular Technology Conference (VTC2020-Fall)},
        address = {Victoria, B.C., Canada}
    }
    
    @article{Tsukada2020b,
        title = {Networked Roadside Perception Units for Autonomous Driving},
        author = {Manabu Tsukada and Takaharu Oi and Masahiro Kitazawa and Hiroshi Esaki},
        url = {https://www.mdpi.com/1424-8220/20/18/5320},
        doi = {10.3390/s20185320},
        issn = {1424-8220},
        year = {2020},
        date = {2020-09-17},
        journal = {MDPI Sensors},
        volume = {20},
        number = {18}
    }



## Description

[Autoware](https://gitlab.com/autowarefoundation/autoware.ai) is open-source autonomous driving software. [OpenC2X](https://www.ccs-labs.org/software/openc2x/) is open-source cooperative ITS software, able to communicate with other vehicles following ITS-G5. AutoC2X is an extension for Autoware and OpenC2X. Using this software, you can get other vehicle information run by Autoware. 

## AutoC2X-OC
AutoC2X-AW collaborate with [AutoC2X-OC](https://github.com/esakilab/AutoC2X-OC) at router. So you should install and run AutoC2X-OC in router.

## Requirement
- autoware (ROS)
- libboost
- libproj
    - apt-get install libproj-dev

## Usage

### edit CMakeLists.txt
change "include_directories" to your environment

### build
```
mkdir build
cd build
cmake ..
make all
```

### hardware Setup
In AutoC2X, each vehicle is supposed to equip one laptop and one router. Autoware and AutoC2X-AW is running in laptop. AutoC2X-OC is running in router.

![hardware](https://user-images.githubusercontent.com/23014935/76481753-b59a2f80-6455-11ea-9134-4b5376bf75c4.png)

### network setup
You should allocate IPv4 address to laptops and routers. Below image is example.

![network](https://user-images.githubusercontent.com/23014935/76482009-50930980-6456-11ea-9155-3abf0788592b.png)

### how to run

You should start AutoC2X below order.

1. Start AutoC2X-AW at receiver laptop <- # ./autoware_pub -r <router's ipv4 address>
2. Start AutoC2X-OC at receiver router
3. Start AutoC2X-OC at sender router
4. Start AutoC2X-AW at sender laptop <- # ./autoware_pub -s <router's ipv4 address>

```
cd build/
./autoware_pub [-r|-s] <router's ipv4 address>
```

You may have to be superuser.
