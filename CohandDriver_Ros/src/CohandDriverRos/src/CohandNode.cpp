#include "CohandNode.h"
#include <vector>
#include <string>

CohandDriver RosApp;
ros::Publisher pub;
int Fingers[4][3];


void CohandCallBackHandler(const std_msgs::Int32MultiArray &_msg)
{
	if(_msg.data.size() == 4)
	{
		//ROS_INFO("the data:%d, %d, %d, %d.", _msg.data[0], _msg.data[1], _msg.data[2], _msg.data[3]);
		switch(_msg.data[0])
		{
			case 0:
				RosApp.ExcuteCmd();
				break;
			case 1:
				RosApp.Finger1ToTarget(_msg.data[1], _msg.data[2], _msg.data[3]);
				break;
			case 2:
				RosApp.Finger1ToTarget(_msg.data[1], _msg.data[2], _msg.data[3]);
				break;
			case 3:
				RosApp.Finger1ToTarget(_msg.data[1], _msg.data[2], _msg.data[3]);
				break;
			case 4:
				RosApp.PoseToTarget(_msg.data[1], _msg.data[2], _msg.data[3]);
				break;
			default:
				ROS_INFO("the cmd error:%d.", _msg.data[0]);
				break;
		}
	}
	else
	{
		ROS_INFO("the data format error.");
	}

}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "CohandDriver");
	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("CohandTopic", 5, CohandCallBackHandler);
    pub = n.advertise<std_msgs::Int32MultiArray>("CohandState", 10);

	std::string AutoConnect, PortName;
	ros::param::get("/AutoConnect", AutoConnect);
	ros::param::get("/SerialName", PortName);
	int state = 0;
	if(AutoConnect == "true") state = RosApp.OpenPort(115200);
	else state = RosApp.OpenPort(PortName, 115200);

	if(state == 1)
	{
		ros::spinOnce();
		ros::Rate loop_rate(10);
		while (ros::ok())
		{
			std::vector<uint32_t> info_1(10);
			std_msgs::Int32MultiArray info;
			RosApp.GetFinger1State(info_1);
			ROS_INFO_STREAM("info "<<info_1[0]<<" "<<info_1[1]<<" "<<info_1[2]);
			ROS_INFO_STREAM(" test ");
            2311
			info.data.push_back(1);
			info.data.push_back(info_1[0]);
			info.data.push_back(info_1[1]);
			info.data.push_back(info_1[2]);
			pub.publish(info);
			loop_rate.sleep();
		}
	}
	else
	{
		ROS_INFO("Open hand failed, please check the usb serial port.");
	}




	return 0;
}
