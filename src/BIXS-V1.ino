#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <SFE_BMP180.h>
#include <SparkFunLSM9DS1.h>
#include <XBee.h>
#include <Sys_Init.h>
#include <Sys_Op.h>

//Define the Baudrate for the serial communication (Applies to all functions)
#define BAUD_RATE 9600
const unsigned int I2C = 0x70;

void setup()
{
  //Initialize Serial
  SysCom_Init(BAUD_RATE);
  XBee_Init();
  //Initialize the sensor (it is important to get calibration values stored on the device).
  Alt_Init(); //Altimeter initialization
  IMU_Init(); //IMU initialization
}

void loop()
{
  Alt_Update();
  IMU_Update();
}