 #include <ros/ros.h>
 #include <tf2_ros/transform_listener.h>
 #include "tf2_msgs/TFMessage.h"
 #include <geometry_msgs/TransformStamped.h>
 #include <geometry_msgs/Twist.h>
 


float x; float y;
     geometry_msgs::TransformStamped transformStamped;

 int main(int argc, char** argv){
   ros::init(argc, argv, "prueba_listener");
   ros::NodeHandle node;
   
   tf2_ros::Buffer tfBuffer;
   tf2_ros::TransformListener tfListener(tfBuffer);
   ros::Rate rate(10.0);
   while (node.ok()){
    ros::spinOnce;
     try{
       transformStamped = tfBuffer.lookupTransform("openni_depth_frame", "torso_5",
                                ros::Time(0));
     }
     catch (tf2::TransformException &ex) {
       ROS_WARN("%s",ex.what());
       ros::Duration(1.0).sleep();
       continue;
     }
     x=transformStamped.transform.translation.x;
     y=transformStamped.transform.translation.y;
     ROS_INFO("La Profundidas es %f, y el Horizonte es %f", x, y);
     rate.sleep();
   }
   return 0;
};