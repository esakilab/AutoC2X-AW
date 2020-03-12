# AutoC2X-AW

![movie](https://user-images.githubusercontent.com/23014935/76400182-39570c00-63c3-11ea-81cb-a6b84179406d.gif)

AutoC2X is cooperative awareness driving software, extension for Autoware and OpenC2X. AutoC2X-AW is extension for Autoware.

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
