#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <nav_msgs/Odometry.h>
#include <math.h>
#include <std_msgs/Float32.h>

//#include <tf/Matrix3x3.h>
double x;
double y;
double human_x;
double human_y;
double dist;
void odom_callback(const geometry_msgs::TransformStampedPtr& msg)
    {
        x = msg->transform.translation.x;
        y = msg->transform.translation.y;

    } 

void human_callback(const geometry_msgs::TransformStampedPtr& msg)
    {
	human_x = msg->transform.translation.x;
        human_y = msg->transform.translation.y;
    }


  int main(int argc, char** argv) {
     ros::init(argc, argv, "state_publisher");
     ros::NodeHandle n;
     ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
     ros::Publisher dist_pub = n.advertise<std_msgs::Float32>("distance", 1);
     tf::TransformListener listener;
     tf::TransformBroadcaster broadcaster1;
     tf::TransformBroadcaster broadcaster2;
     tf::TransformBroadcaster broadcaster3;
     ros::Subscriber sub = n.subscribe("vicon/Sphero3_3/Sphero3_3", 1, odom_callback); 
     ros::Subscriber sub_human = n.subscribe("vicon/Helmet_1/Helmet_1", 1, human_callback);
     //ros::Rate loop_rate(10);
     // robot state
     double pos = 0, angle = 0;
 
     // message declarations
     geometry_msgs::TransformStamped odom_trans;

     geometry_msgs::TransformStamped map;
     geometry_msgs::TransformStamped odom_world_trans;
     tf::StampedTransform transform;
     // add base_link

     sensor_msgs::JointState joint_state;
     
     odom_world_trans.header.frame_id = "world";
     odom_world_trans.child_frame_id = "odom";
     odom_trans.header.frame_id = "odom";
     odom_trans.child_frame_id = "dummy";
     map.header.frame_id = "world";
     map.child_frame_id = "map";
     ros::AsyncSpinner spinner(1);
     //spinner.start();
     while (ros::ok()) {
         joint_state.header.stamp = ros::Time::now();
         joint_state.name.resize(1);
         joint_state.position.resize(1);
         joint_state.name[0] ="dummy";
         joint_state.position[0] = pos;
         // update transform
	spinner.start();
    	ros::spinOnce();
    	ros::Rate rate(1.0);
    	spinner.stop();
         map.header.stamp = ros::Time::now();
         map.transform.translation.x = 0;
         map.transform.translation.y = 0;
         map.transform.translation.z = 0;
         map.transform.rotation = tf::createQuaternionMsgFromYaw(0);
         broadcaster1.sendTransform(map);
         odom_trans.header.stamp = ros::Time::now();
	 //ROS_INFO("odom_trans: %f", odom_trans.header.stamp.toSec());
         odom_trans.transform.translation.x = 0;
         odom_trans.transform.translation.y = 0;
         odom_trans.transform.translation.z = 0;
         odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(0);

         broadcaster3.sendTransform(odom_trans);
         ros::spinOnce();
	 //ros::Time now = ros::Time::now() - ros::Duration(0.1);
	 ros::Time now = ros:: Time::now();

         // robot to world
	 odom_world_trans.header.stamp = now;
	 odom_world_trans.transform.translation.x = x;
	 odom_world_trans.transform.translation.y = y;
	 odom_world_trans.transform.translation.z = 0.0;
	 tf::Matrix3x3 m(transform.getRotation());
	 double roll, pitch, yaw;
  	 m.getRPY(roll, pitch, yaw);
	 odom_world_trans.transform.rotation = tf::createQuaternionMsgFromYaw(yaw);

         broadcaster2.sendTransform(odom_world_trans);
	 
	 dist = sqrt((human_x - x)*(human_x - x) + (human_y - y) * (human_y - y));
         std_msgs::Float32 d;
	 d.data = dist;
	 dist_pub.publish(d);
         ros::spinOnce();
         joint_pub.publish(joint_state);
         ros::spinOnce();

     }
     //spinner.stop();
     return 0;
 }
