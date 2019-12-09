# how to set up

## dependency
- autoware (ROS)
- libboost
- libproj
    - apt-get install libproj-dev


## edit CMakeLists.txt
change "include_directories" to your environment

## build
```
mkdir build
cd build
cmake ..
make all
```