#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Int8.h>

geometry_msgs::Point rover;
std_msgs::Int8 cameras;
std_msgs::Int8 fans;

class Teleop{
  private:
    ros::NodeHandle nh;
    ros::Publisher vel_pub;
    //ros::Publisher color_pub;
    ros::Publisher cam_pub;
    ros::Subscriber sub;
    ros::Publisher fan_pub;
  public:
    Teleop(){
      fans.data=0;
      this->vel_pub = this->nh.advertise<geometry_msgs::Point>("/rover", 20);
      this->cam_pub = this->nh.advertise<std_msgs::Int8>("/cameras", 20);
      this->fan_pub = this->nh.advertise<std_msgs::Int8>("/fan_control", 20);
      //this->color_pub = this->nh.advertise<std_msgs::Int8>("/led", 20);
      this->sub = this->nh.subscribe("/joy0", 20, &Teleop::joyCallback, this);
    }

    void teleop(float linear, float rotational, float speed){
      
      rover.x=(linear*((speed+1)/2)+rotational*((speed+1)/2))*100;
      rover.z=(linear*((speed+1)/2)-rotational*((speed+1)/2))*100;

      this->vel_pub.publish(rover);
    }

    void joyCallback(const sensor_msgs::Joy& msg){
      // ros::Rate loop_rate(20); 

      if(msg.axes[1]>0.1 || msg.axes[1] < -0.1 || msg.axes[0] > 0.1 || msg.axes[0] < -0.1){
        this->teleop(msg.axes[1],msg.axes[0],msg.axes[2]);
      }
      else{
        rover.x = 0;
        rover.z = 0;
        this->vel_pub.publish(rover);
      }
      
      if(msg.axes[9]>0.9) //Gimbal 1 Up
      {
      	cameras.data=0;
      	this->cam_pub.publish(cameras);
      }
      if(msg.axes[9]<-0.9)  //Gimbal 1 Down
      {
      	cameras.data=1;
      	this->cam_pub.publish(cameras);
      }
      if(msg.axes[8]>0.9)  //Gimbal 1 Left
      {
      	cameras.data=2;
      	this->cam_pub.publish(cameras);
      }
      if(msg.axes[8]<-0.9)  //Gimbal 1 Right
      {
      	cameras.data=3;
      	this->cam_pub.publish(cameras);
      }

      if(msg.axes[7]>0.9) //Gimbal 2 Left
      {
      	cameras.data=4;
      	this->cam_pub.publish(cameras);
      }
      if(msg.axes[7]<-0.9)  //Gimbal 2 Right
      {
      	cameras.data=5;
      	this->cam_pub.publish(cameras);
      }
      if(msg.buttons[8])  //Gimbal 2 Up
      {
      	cameras.data=6;
      	this->cam_pub.publish(cameras);
      }
      if(msg.buttons[9])  //Gimbal 2 Down
      {
      	cameras.data=7;
      	this->cam_pub.publish(cameras);
      }
      
      if(msg.axes[5]>0.9) //Gimbal 3 Left
      {
      	cameras.data=8;
      	this->cam_pub.publish(cameras);
      }
      if(msg.axes[5]<-0.9)  //Gimbal 3 Right
      {
      	cameras.data=9;
      	this->cam_pub.publish(cameras);
      }

      if(msg.buttons[0])
      {
        if(fans.data==0)
        {
          fans.data=1;
        }
        else
          fans.data=0;
        this->fan_pub.publish(fans);
      }
  
      
      // led.data = 0;
      // this->color_pub.publish(led);
      // loop_rate.sleep();
    }
};

int main(int argc, char **argv){
  ros::init(argc, argv, "teleop", ros::init_options::AnonymousName);
  Teleop teleop = Teleop();
  ros::spin();
  return 0;
}
