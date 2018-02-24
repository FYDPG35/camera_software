#include <ros/ros.h>
#include <iostream>
#include <camera_pkg/inc_header.h>

int main(int argc, char **argv)
{
	ros::init(argc,argv,"main_control");
    ros::NodeHandle n;

    //Subscribe to the desired topics and assign callbacks
    

    //Set the loop rate
    ros::Rate loop_rate(20);    //20Hz update rate

    while (ros::ok())
    {
    	loop_rate.sleep(); //Maintain the loop rate
    	ros::spinOnce();   //Check for new messages

	 	ROS_INFO("Hello");
    }

    return 0;
}