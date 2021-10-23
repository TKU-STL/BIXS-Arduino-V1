#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <SFE_BMP180.h>
#include <SparkFunLSM9DS1.h>
#include <Sys_Init.h>
#include <Sys_Op.h>

//Define the Baudrate for the serial communication (Applies to all functions)
#define BAUD_RATE 115200

void setup()
{
  //Initialize Serial
  SysCom_Init(BAUD_RATE);
  //Initialize the sensor (it is important to get calibration values stored on the device).
  Alt_Init();
  IMU_Init();
}

void loop()
{
  Alt_Update();
  IMU_Update();
}