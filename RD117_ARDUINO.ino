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
#include "max30102.h"

// comment this line to disable motion sensors
#define MOTION

#ifdef MOTION
#include <MPU9250_asukiaaa.h>
#endif

// ADALOGGER pins
const byte chipSelect = 4;
const byte oxiInt = 10; // ADALOGGER pin connected to MAX30102 INT

uint32_t red, ir;
uint8_t uch_dummy,k;

//MPU9250 Vars
#ifdef MOTION
MPU9250 mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ;
#endif

//Output vars
String result;

void setup() {

  pinMode(oxiInt, INPUT);  //pin D10 connects to the interrupt output pin of the MAX30102
  Wire.begin();
  
  Serial.begin(115200);

  maxim_max30102_reset(); //resets the MAX30102
  delay(1000);

  maxim_max30102_read_reg(REG_INTR_STATUS_1,&uch_dummy);  //Reads/clears the interrupt status register
  
  maxim_max30102_init();  //initialize the MAX30102

#ifdef MOTION
  //SETUP MPU9250
  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();

#endif

}

void loop() {
  

  while(digitalRead(oxiInt)==1);
  maxim_max30102_read_fifo(&red, &ir);  //read from MAX30102 FIFO

  result = "{";
  result += "\"red\":" + String(red, DEC);
  result += ",\"ir\":" + String(ir, DEC);

#ifdef MOTION
  //Get Accel Values
  mySensor.accelUpdate();
  aX = mySensor.accelX();
  aY = mySensor.accelY();
  aZ = mySensor.accelZ();
  aSqrt = mySensor.accelSqrt();

  result += ",\"accel\": {";
  result += "\"x\":" + String(aX);
  result += ",\"y\":" + String(aY);
  result += ",\"z\":" + String(aZ);
  result += ",\"sqrt\":" + String(aSqrt);
  result += "}"; 

  mySensor.gyroUpdate();
  gX = mySensor.gyroX();
  gY = mySensor.gyroY();
  gZ = mySensor.gyroZ();

  result += ",\"gyro\": {";
  result += "\"x\":" + String(gX);
  result += ",\"y\":" + String(gY);
  result += ",\"z\":" + String(gZ);
  result += "}"; 

#endif

  result += "}"; 
  Serial.println(result);
  Serial.flush();

}


