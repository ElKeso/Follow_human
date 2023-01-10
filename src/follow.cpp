#include "ros/ros.h"
#include "math.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_msgs/TFMessage.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int16.h"

//x=profundidad
//y=horizontal

  geometry_msgs::Twist mover;
  geometry_msgs::TransformStamped transformStamped;
  float d_l; float d_a; float x; float y;
  float PI = 3.14159265;
  int f=0; int i=1;
  float d_l_const=1.3; float d_a_const=0;
  float t_l=0.1; float t_a=0.;


void init(const tf2_msgs::TFMessage & msg){
}

float distancia_lineal(float x, float y){
  d_l=sqrt(pow(x,2)+pow(y,2));
  return d_l; 
}

float distancia_angular(float x, float y){
  d_a=atan(y/x);
  return d_a;
}
int complete=0;
void chatterCallback(const std_msgs::Int16  &msg){
  if(msg.data==1){
    f=1;
  }
  if(msg.data==2){
    if(complete==0){
      f=2;
    }
    else{
      f=0;
    }
  }
  if(msg.data==3){
    f=0;
  }
}

int main(int argc, char **argv){
  ros::init(argc, argv, "follow_human");
  ros::NodeHandle nh;
  ros::Subscriber sub_broad = nh.subscribe("/tf", 1000, init);
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("RosAria/cmd_vel",1000);
  ros::Subscriber sub = nh.subscribe("/signal", 1000, chatterCallback);
  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);
  ros::Rate loop_rate(1000);//rate del while
  ros::Rate robot(10);// publicacion para moverse
  while (ros::ok()){
      ros::spinOnce();
     try{
       transformStamped = tfBuffer.lookupTransform("openni_depth_frame", "torso_1",
                                ros::Time(0));
     }
     catch (tf2::TransformException &ex) {
       ROS_WARN("%s",ex.what());
       ros::Duration(1.0).sleep();
       continue;
     }
     x=transformStamped.transform.translation.x;
     y=transformStamped.transform.translation.y;
     mover.angular.z=0;
     mover.linear.x=0;

    switch(f){
      case 0://stand by

      break;

      case 1://seguimiento
        if(abs(y)>0.05){
          if(abs(y)<0.10){
          mover.angular.z=0;
          mover.linear.x=0;
          pub.publish(mover);
          robot.sleep();
          }
        else{
         if(y>0){
            mover.angular.z=0.25;
            mover.linear.x=0;
         }
         if(y<0){
            mover.angular.z=-0.25;
            mover.linear.x=0;
         } 
            pub.publish(mover);
            robot.sleep();
        } 
        } 
      ROS_INFO("La Profundidas es %f, y el Horizonte es %f", x, y); 
      break;

      case 2://retorno

      break; 

      case 3://finish

      break;
    }
    loop_rate.sleep();
  }
  return 0;
}