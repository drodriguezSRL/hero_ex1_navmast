#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/Joy.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

class joycmd_listen
{
public:
	joycmd_listen();
	~joycmd_listen();
	void loop();

private:
	void joyCallback(const sensor_msgs::Joy& joy);
	// Node Handle declaration
	ros::NodeHandle nh;
	// ROS subscriber and publisher declarations
	ros::Subscriber joy_sub;
	//publish command message to joints/servos of arm
	ros::Publisher joint1_pub = nh.advertise<std_msgs::Float64>("/joint1_controller/command", 100);
	ros::Publisher joint2_pub = nh.advertise<std_msgs::Float64>("/joint2_controller/command", 100);
    ros::Publisher joint3_pub = nh.advertise<std_msgs::Float64>("/joint3_controller/command", 100);

	int triangle_button = 0;
    int circle_button = 0;
    int x_mark_button = 0;
    int square_button = 0;
    int L1_button = 0;
    int R1_button = 0;
	int L2_button = 0;
	int R2_button = 0;
	int share_button = 0;
	int options_button = 0;
	int L3_button = 0;
	int R3_button = 0;
	int playstation_button;
	int touch_button;

	float L_stick_horizontal = 0.0;
 	float L_stick_vertical = 0.0;
	float R_stick_horizontal = 0.0;
	float L2;
	float R2;
	float R_stick_vertical = 0.0;
	int Axis_button_LR;
	int Axis_button_UPDOWN;

	std_msgs::Float64 position1;
	std_msgs::Float64 position2;
	std_msgs::Float64 position3;
};

joycmd_listen::joycmd_listen()
{
	joy_sub = nh.subscribe("/joy", 100, &joycmd_listen::joyCallback, this); 
}

joycmd_listen::~joycmd_listen()
{
	;
}

void joycmd_listen::joyCallback(const sensor_msgs::Joy& joy)
{	
	square_button = joy.buttons[0];
	x_mark_button = joy.buttons[1];
	circle_button = joy.buttons[2];
	triangle_button = joy.buttons[3];
	L1_button = joy.buttons[4];
	R1_button = joy.buttons[5];
	L2_button = joy.buttons[6];
	R2_button = joy.buttons[7];
	share_button = joy.buttons[8];
	options_button = joy.buttons[9];
	L3_button = joy.buttons[10];
	R3_button = joy.buttons[11];
	playstation_button = joy.buttons[12];
	touch_button = joy.buttons[13];

	L_stick_horizontal = joy.axes[0];
	L_stick_vertical = joy.axes[1];
	R_stick_horizontal = joy.axes[2];
	L2 = joy.axes[3];
	R2 = joy.axes[4];
	R_stick_vertical = joy.axes[5];
	Axis_button_LR = joy.axes[6];
	Axis_button_UPDOWN = joy.axes[7];
}

void joycmd_listen::loop()
{
	ros::Rate rate(100.0);
	
	while(ros::ok())
	{
		ros::spinOnce();
		
		if(touch_button == 1){
			ROS_INFO("touch_button is pushed. initial state.");
			position1.data = 1.5;
			joint1_pub.publish(position1);
			position2.data = 0.0;
			joint2_pub.publish(position2);
			position3.data = 0.0;
			joint3_pub.publish(position3);
		}

        else if(square_button == 1){
        	ROS_INFO("square_button is pushed. LIDAR Tilt.");
        	if(L_stick_vertical == 1.0 && position1.data < 1.5){
        		position1.data += 0.01;
				joint1_pub.publish(position1);
			}
			else if(L_stick_vertical == -1.0 && position1.data > 0.0){
				position1.data -= 0.01;
				joint1_pub.publish(position1);
			}
        }

        else if(circle_button == 1){
        	ROS_INFO("circle_button is pushed. Camera Pan and Tilt.");
        	if(L_stick_horizontal > 0.5 && position2.data < 1.5){
        		position2.data += 0.01;
				joint2_pub.publish(position2);
			}
			else if(L_stick_horizontal < -0.5 && position2.data > -1.5){
				position2.data -= 0.01;
				joint2_pub.publish(position2);
			}
			if(L_stick_vertical > 0.5 && position3.data < 1.5){
        		position3.data += 0.01;
				joint3_pub.publish(position3);
			}
			else if(L_stick_vertical < -0.5 && position3.data > -1.0){
				position3.data -= 0.01;
				joint3_pub.publish(position3);
			}
        }
		
	rate.sleep();
	}
}


int main(int argc, char** argv)
{
	ros::init(argc, argv, "navmast_joy_ctrl");

  	ROS_INFO("start joy_ctl !!");
	joycmd_listen joy;
	joy.loop();
}
