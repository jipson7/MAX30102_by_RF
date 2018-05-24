/********************************************************
*
* Project: MAXREFDES117#
* Filename: RD117_ARDUINO.ino
* Description: This module contains the Main application for the MAXREFDES117 example program.
*
* Revision History:
*\n 1-18-2016 Rev 01.00 GL Initial release.
*\n 12-22-2017 Rev 02.00 Significantlly modified by Robert Fraczkiewicz
*
* --------------------------------------------------------------------
*
* This code follows the following naming conventions:
*
* char              ch_pmod_value
* char (array)      s_pmod_s_string[16]
* float             f_pmod_value
* int32_t           n_pmod_value
* int32_t (array)   an_pmod_value[16]
* int16_t           w_pmod_value
* int16_t (array)   aw_pmod_value[16]
* uint16_t          uw_pmod_value
* uint16_t (array)  auw_pmod_value[16]
* uint8_t           uch_pmod_value
* uint8_t (array)   auch_pmod_buffer[16]
* uint32_t          un_pmod_value
* int32_t *         pn_pmod_value
*
* ------------------------------------------------------------------------- */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "algorithm_by_RF.h"
#include "max30102.h"
#include <MPU9250_asukiaaa.h>

// ADALOGGER pins
const byte chipSelect = 4;
const byte oxiInt = 10; // ADALOGGER pin connected to MAX30102 INT

uint32_t elapsedTime,timeStart;

uint32_t aun_ir_buffer[BUFFER_SIZE]; //infrared LED sensor data
uint32_t aun_red_buffer[BUFFER_SIZE];  //red LED sensor data
float old_n_spo2;  // Previous SPO2 value
uint8_t uch_dummy,k;

//MPU9250 Vars
MPU9250 mySensor;

float aX, aY, aZ, aSqrt, gX, gY, gZ;

//Output vars
String led_output, accel_output, gyro_output;

void setup() {

  pinMode(oxiInt, INPUT);  //pin D10 connects to the interrupt output pin of the MAX30102
  Wire.begin();
  
  Serial.begin(115200);

  maxim_max30102_reset(); //resets the MAX30102
  delay(1000);

  maxim_max30102_read_reg(REG_INTR_STATUS_1,&uch_dummy);  //Reads/clears the interrupt status register

  char my_status[20];
  
  old_n_spo2=0.0;
  maxim_max30102_init();  //initialize the MAX30102

  //SETUP MPU9250
  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
  //mySensor.beginMag();

}

//Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 4 seconds
void loop() {
  float n_spo2,ratio,correl;  //SPO2 value
  int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
  int32_t n_heart_rate; //heart rate value
  int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
  int8_t i;
  char hr_str[10];
  
  //buffer length of 100 stores 4 seconds of samples running at 25sps
  //read 100 samples, and determine the signal range
  timeStart = millis();
  Serial.println("start");
  for(i=0;i<BUFFER_SIZE;i++)
  {
    //Get LED Values
    while(digitalRead(oxiInt)==1);  //wait until the interrupt pin asserts
    maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));  //read from MAX30102 FIFO
    
    elapsedTime=millis()-timeStart;
    Serial.println("offset " + String(elapsedTime, DEC));
    
    led_output = "led ";
    led_output += " ";
    led_output += String(aun_red_buffer[i], DEC);
    led_output += " ";
    led_output += String(aun_ir_buffer[i], DEC);    
    Serial.println(led_output);

    //Get Accel Values
    elapsedTime=millis()-timeStart;
    accel_output = "accel ";
    mySensor.accelUpdate();
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
    accel_output += String(aX);
    accel_output += " ";
    accel_output += String(aY);
    accel_output += " ";
    accel_output += String(aZ);
    accel_output += " ";
    accel_output += String(aSqrt);
    Serial.println(accel_output);

    elapsedTime=millis()-timeStart;
    gyro_output = "gyro ";
    mySensor.gyroUpdate();
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    gyro_output += String(gX);
    gyro_output += " ";
    gyro_output += String(gY);
    gyro_output += " ";
    gyro_output += String(gZ);
    Serial.println(gyro_output);
  }

  //calculate heart rate and SpO2 after 100 samples (4 seconds of samples) using Robert's method
  rf_heart_rate_and_oxygen_saturation(aun_ir_buffer, BUFFER_SIZE, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid, &ratio, &correl); 

  Serial.print("oxygen ");
  Serial.print(n_spo2);
  Serial.print(" ");
  Serial.println(ch_spo2_valid, DEC);
  
  Serial.print("hr ");
  Serial.print(n_heart_rate, DEC);
  Serial.print(" ");
  Serial.println(ch_hr_valid, DEC);

  Serial.print("ratio ");
  Serial.println(ratio);
  Serial.print("correl ");
  Serial.println(correl);

  if(ch_hr_valid && ch_spo2_valid) { 
    old_n_spo2=n_spo2;
  }
}

