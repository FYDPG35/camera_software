cmake_minimum_required(VERSION 2.8.3)
project(camera_pkg)


# check c++11 / c++0x
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SDL2_DIR /usr/local/lib/cmake/SDL2/)

MESSAGE( STATUS "SDL2_DIR is: " ${SDL2_DIR} )

# set(SDL2_INCLUDE_DIRS /usr/local/include/SDL2)
# set(SDL2_LIBRARIES /usr/local/lib/libSDL2.so)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  gazebo_msgs
  geometry_msgs
  nav_msgs
  roscpp
  rospy
  sensor_msgs
  std_msgs
  tf
  visualization_msgs
  SDL2 REQUIRED
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES camera_pkg ${SDL2_LIBRARY}
#  CATKIN_DEPENDS geometry_msgs roscpp rospy sensor_msgs std_msgs tf
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
)


## Declare a cpp executable
add_executable(camera_node src/camera.cpp src/main.c src/feature_test.c)


## Specify libraries to link a library or executable target against
 target_link_libraries(camera_node
   ${catkin_LIBRARIES}
   ${SDL2_LIBRARIES}
 )


