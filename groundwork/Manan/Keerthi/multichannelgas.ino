/*
    This is a demo to test gas library
    This code is running on Xadow-mainboard, and the I2C slave is Xadow-gas
    There is a ATmega168PA on Xadow-gas, it get sensors output and feed back to master.
    the data is raw ADC value, algorithm should be realized on master.

    please feel free to write email to me if there is any question

    Jacky Zhang, Embedded Software Engineer
    qi.zhang@seeed.cc
    17,mar,2015
*/

#include <Wire.h>
#include "MutichannelGasSensor.h"
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Float64MultiArray.h>
ros::NodeHandle nh;

std_msgs::Float64MultiArray mgs;

ros::Publisher sens("/mgs", &mgs);

void setup()
{
    //Serial.begin(115200);  // start serial for output
    //Serial.println("power on!");
    gas.begin(0x04);//the default I2C address of the slave is 0x04
    gas.powerOn();
    nh.initNode();
    while (!nh.connected()) {
    nh.spinOnce();  // Keep checking for connection
    delay(100);     // Short delay to prevent busy-waiting
}
    nh.advertise(sens);
    mgs.layout.dim=NULL;
    mgs.layout.data_offset=0;
    //Serial.print("Firmware Version = ");
    //Serial.println(gas.getVersion());
}

void loop()
{
    float val1,val2,val3,val4,val5,val6,val7,val8;

    val1 = gas.measure_NH3();
    //Serial.print("The concentration of NH3 is ");
    //if(val1>=0) Serial.print(c);
    //else Serial.print("invalid");
    //Serial.println(" ppm");

    val2 = gas.measure_CO();
    //Serial.print("The concentration of CO is ");
    //if(val2>=0) Serial.print(c);
    //else Serial.print("invalid");
    //Serial.println(" ppm");

    val3 = gas.measure_NO2();
    //Serial.print("The concentration of NO2 is ");
    //if(c>=0) Serial.print(c);
    //else Serial.print("invalid");
    //Serial.println(" ppm");

    val4 = gas.measure_C3H8();
    //Serial.print("The concentration of C3H8 is ");
    //if(c>=0) Serial.print(c);
    //else Serial.print("invalid");
    //Serial.println(" ppm");

    val5 = gas.measure_C4H10();
    //Serial.print("The concentration of C4H10 is ");
    //if(c>=0) Serial.print(c);
    //else Serial.print("invalid");
    //Serial.println(" ppm");

    val6 = gas.measure_CH4();
    //Serial.print("The concentration of CH4 is ");
    //if(c>=0) Serial.print(c);
    //else Serial.print("invalid");
    //Serial.println(" ppm");

    val7 = gas.measure_H2();
    //Serial.print("The concentration of H2 is ");
    //if(c>=0) Serial.print(c);
    //else Serial.print("invalid");
    //Serial.println(" ppm");

    val8 = gas.measure_C2H5OH();
    //Serial.print("The concentration of C2H5OH is ");
    //if(c>=0) Serial.print(c);
    //else Serial.print("invalid");
    //Serial.println(" ppm");
    float array[]={val1,val2,val3,val4,val5,val6,val7,val8};
    mgs.data=array;
    mgs.data_length=8;
    sens.publish(&mgs);
    nh.spinOnce();

    delay(10);
    //Serial.println("...");
}
