# AutoC2X-AW

![movie](https://user-images.githubusercontent.com/23014935/76400182-39570c00-63c3-11ea-81cb-a6b84179406d.gif)

AutoC2X is cooperative awareness driving software, extension for Autoware and OpenC2X. AutoC2X-AW is extension for Autoware.

## description

[Autoware](https://gitlab.com/autowarefoundation/autoware.ai) is open-source autonomous driving software. [OpenC2X](https://www.ccs-labs.org/software/openc2x/) is open-source cooperative ITS software, able to communicate with other vehicles following ITS-G5. AutoC2X is an extension for Autoware and OpenC2X. Using this software, you can get other vehicle information run by Autoware. 


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

### how to run
```
cd build/
./autoware_pub [-r|-s] <router's ipv4 address>
```

You may have to be superuser.
