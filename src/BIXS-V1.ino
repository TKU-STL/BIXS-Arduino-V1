#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <SparkFunLSM9DS1.h>
#include <SFE_BMP180.h>
#include <Sys_Init.h>
#include <Altimeter.h>
#include <IMU.h>
//#include <Kalman_Fliter.h>
#include <System_Utility.h>
#include <Test.h>

//Define the Baudrate for the serial communication (Applies to all functions)
#define BAUD_RATE 9600
const int DELAY = 2000;
const unsigned int I2C = 0x70;

void setup()
{
  //Initialize Serial
  SysCom_Init(BAUD_RATE);
  //Initialize the sensor (it is important to get calibration values stored on the device).
  Alt_Init(); //Altimeter initialization
  IMU_Init(); //IMU initialization
  //Kalman filter initialization
  //Kalman_Init();
}

void loop()
{
  if ((millis() / 1000) > 10)
  {
    System_Clock();
    Alt_Update();
    IMU_Update();
    delay(DELAY);
  }
  else
  {
    Serial.print("T-minus: ");
    Serial.println((10-(millis() / 1000)));
    delay(1000);
  } /*
  Serial.println("----------------Kalman Fliter Output----------------");
  Kalman_Update_Alt();
  Kalman_Update_AccX();
  Kalman_Update_AccY();
  Kalman_Update_AccZ();
  Kalman_Update_GyroX();
  Kalman_Update_GyroY();
  Kalman_Update_GyroZ();
*/
}