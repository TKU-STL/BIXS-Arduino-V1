/* SFE_BMP180 altitude example sketch
This sketch shows how to use the Bosch BMP180 pressure sensor
as an altimiter.
https://www.sparkfun.com/products/11824
Like most pressure sensors, the BMP180 measures absolute pressure.
Since absolute pressure varies with altitude, you can use the pressure
to determine your altitude.
Because pressure also varies with weather, you must first take a pressure
reading at a known baseline altitude. Then you can measure variations
from that pressure
Hardware connections:
- (GND) to GND
+ (VDD) to 3.3V
(WARNING: do not connect + to 5V or the sensor will be damaged!)
You will also need to connect the I2C pins (SCL and SDA) to your
Arduino. The pins are different on different Arduinos:
Any Arduino pins labeled:  SDA  SCL
Uno, Redboard, Pro:        A4   A5
Mega2560, Due:             20   21
Leonardo:                   2    3
Leave the IO (VDDIO) pin unconnected. This pin is for connecting
the BMP180 to systems with lower logic levels such as 1.8V
Have fun! -Your friends at SparkFun.
The SFE_BMP180 library uses floating-point equations developed by the
Weather Station Data Logger project: http://wmrx00.sourceforge.net/
Our example code uses the "beerware" license. You can do anything
you like with this code. No really, anything. If you find it useful,
buy me a beer someday.
V10 Mike Grusin, SparkFun Electronics 10/24/2013
*/

// Your sketch must #include this library, and the Wire library.
// (Wire is a standard library included with Arduino.):

#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <SFE_BMP180.h>
#include <SparkFunLSM9DS1.h>
#include <Sys_Init.h>
#include <SysOp.h>
// You will need to create an SFE_BMP180 object, here called "pressure":
/*
#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG 0x6B
#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250 // 250 ms between prints
//static unsigned long lastPrint = 0;
#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.
*/

#define BAUD_RATE 115200

void setup()
{
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