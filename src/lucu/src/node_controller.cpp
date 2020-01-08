#include <ros/ros.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <termios.h>
#include <string.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/State.h>

void keyCommandIndicator(const std_msgs::String::ConstPtr& msg);
void keyboardCommands();
void do_takeoff();
void do_forward();
void do_landing();
void do_backward();
void do_left();
void do_right();

char g_key_command = 'o';
const double KEYBOARD_CALL_DURATION = 0.5;

ros::Time last_keyboard_command;

ros::ServiceClient set_mode_client;
ros::ServiceClient arming_client;

ros::Publisher position_publisher;

geometry_msgs::PoseStamped position;

mavros_msgs::CommandBool arm_cmd;
mavros_msgs::SetMode offb_set_mode;
mavros_msgs::State current_state;

int main(int argc, char **argv)
{
	ros::init(argc,argv, "circle");
	ros::NodeHandle nh;
	
	position_publisher = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);
	ros::Subscriber key_command_subscriber = nh.subscribe<std_msgs::String>("key_commands", 1000, keyCommandIndicator);
	set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
	arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
	
	ros::Rate rate(100);
	while(ros::ok())
	{
		ros::spinOnce();
			
		if (ros::Time::now() - last_keyboard_command > ros::Duration(KEYBOARD_CALL_DURATION)) {
			last_keyboard_command = ros::Time::now();
			keyboardCommands();	
		}
	}
}

void keyCommandIndicator(const std_msgs::String::ConstPtr& msg){
	g_key_command = *msg->data.c_str();
	ROS_INFO("%c", g_key_command); 
}

void keyboardCommands(){
	switch(g_key_command){
		case 'p':
			do_takeoff();
			break;
	}
}

void do_takeoff()
{
	position.pose.position.x = 0;
	position.pose.position.y = 0;
	position.pose.position.z = 3;
	while(current_state.mode != "OFFBOARD")
	{
			arm_cmd.request.value = true;
			arming_client.call(arm_cmd);
			offb_set_mode.request.custom_mode = "OFFBOARD";
			set_mode_client.call(offb_set_mode);	
			position_publisher.publish(position);
			ros::spinOnce();
	}	
}
