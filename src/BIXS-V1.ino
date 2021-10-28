#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <SparkFunLSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Sys_Init.h>
#include <Sys_Op.h>
#include <Kalman_Fliter.h>

//Define the Baudrate for the serial communication (Applies to all functions)
#define BAUD_RATE 9600
const unsigned int I2C = 0x70;

void setup()
{
  //Initialize Serial
  SysCom_Init(BAUD_RATE);
  //XBee_Init();
  //Initialize the sensor (it is important to get calibration values stored on the device).
  Alt_Init(); //Altimeter initialization
  IMU_Init(); //IMU initialization
  //Kalman filter initialization
  Kalman_Init(); 
}

void loop()
{
  //char command = Serial.read();
  //XBee_Com(command);
  Alt_Update();
  IMU_Update();
  Serial.println("----------------Kalman Fliter Output----------------");
  Kalman_Update_Alt();
  Kalman_Update_AccX();
  Kalman_Update_AccY();
  Kalman_Update_AccZ();
  Kalman_Update_GyroX();
  Kalman_Update_GyroY();
  Kalman_Update_GyroZ();
}