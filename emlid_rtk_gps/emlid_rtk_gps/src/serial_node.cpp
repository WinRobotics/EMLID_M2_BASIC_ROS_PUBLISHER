#include "ros/ros.h"
#include "serial/serial.h"
#include "std_msgs/String.h"
#include <std_msgs/Float64.h>
#include <sensor_msgs/NavSatFix.h>

#include "string.h"
#include <sstream>
#include <iostream>
#include <iomanip>

//ROS Native serial lib
serial::Serial ros_ser;
sensor_msgs::NavSatFix current_gps;

//GPS Publisher
void Gps_reader(ros::Publisher &Gps_reader_pub )
{

    std::stringstream ss;
    std::string _data;
    std::string buffer;
    float _lat;
    float _lon;
    std::string date;
    date ="2021";
    
    if(ros_ser.available())
        {

            ros_ser.readline(_data,175,"\n");
            ss<<_data;
            ss>>buffer;
            ss>>buffer;
            ss>>_lat;
            ss>>_lon;
            current_gps.latitude =_lat;
            current_gps.longitude = _lon;
            Gps_reader_pub.publish(current_gps);

            std::cout.precision(10);
            std::cout << "Number " <<std::fixed << _lat <<" "<<_lon<< std::endl;

            

        }

}


int main (int argc, char** argv)
{
    //ROS init
    ros::init(argc, argv, "serial_node");
    ros::NodeHandle nh;
    

    //Publisher Declaration
    ros::Publisher Gps_reader_pub = nh.advertise<sensor_msgs::NavSatFix>("Gps_msg", 1000);

    //Serial port parameters 
    try 
     {
        //Serial port parameters 
        //std::cout << "[ INFO] [" << ros::Time::now() << "]: Starting" << std::endl;
         ros_ser.setPort("/dev/ttyACM0");
         ros_ser.setBaudrate(9600);
         serial::Timeout to = serial::Timeout::simpleTimeout(1000);
         ros_ser.setTimeout(to);
         ros_ser.open();

         
     }
     catch (serial::IOException& e)
     {
         ROS_ERROR_STREAM("Unable to open port ");
         return -1;
     }
     if(ros_ser.isOpen())
     {
         ROS_INFO_STREAM("Serial Port opened");
     }
     else
     {
         return -1;
     }
     ros::Rate loop_rate(10);
     while(ros::ok())
     {

        //std::cout << "[ INFO] [" << ros::Time::now() << "]: Running" << std::endl;
        //Reading from RTK GPS
        Gps_reader(Gps_reader_pub );

        loop_rate.sleep();
        ros::spinOnce();
     }


}
