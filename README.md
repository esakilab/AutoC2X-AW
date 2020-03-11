# AutoC2X-AW

![movie](https://user-images.githubusercontent.com/23014935/76397781-09a60500-63bf-11ea-8ac6-b07c4bded540.gif)

AutoC2X is cooperative awareness driving software, extension for Autoware and OpenC2X. AutoC2X-AW is extension for Autoware.

## description

[Autoware](https://gitlab.com/autowarefoundation/autoware.ai) is open-source autonomous driving software. [OpenC2X](https://www.ccs-labs.org/software/openc2x/) is open-source cooperative ITS software, able to communicate with other vehicles following ITS-G5. AutoC2X is an extension for Autoware and OpenC2X. Using this software, you can get other vehicle information run by Autoware. AutoC2X is fork project from [OpenC2X-standalone](https://github.com/florianklingler/OpenC2X-standalone).


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
