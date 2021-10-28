#include <Wire.h>
#include <XBee.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SparkFunLSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

LSM9DS1 IMU;
Adafruit_BMP085_Unified Altimeter = Adafruit_BMP085_Unified(10085);
//XBee XBee_A = XBee();
//SoftwareSerial nss(8,9);

void SysCom_Init(long int BAUDRATE)
{
    Serial.begin(BAUDRATE);
    Serial.println("System_COM_Init");
    if (Serial.available())
    {
        Serial.println("System Serial Communication Successful");
    }
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  Altimeter.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void Alt_Init()
{
    Serial.println("Pressure Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!Altimeter.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void IMU_Init()
{
    // Initialize the accelerometer and gyroscope:
    if (IMU.begin() == false) // with no arguments, this uses default addresses (AG:0x6B, M:0x1E) and i2c port (Wire).
    {
        Serial.println("Failed to communicate with LSM9DS1.");
        Serial.println("Double-check wiring.");
        Serial.println("Default settings in this sketch will work for an out of the box LSM9DS1 Breakout, but may need to be modified if the board jumpers are.");
        while (1)
            ;
    }
    else
    {
        Serial.println("LSM9DS1 init success");
    }
}
/*
void XBee_Init()
{
    Serial.println("XBee_Init");
    XBee_A.setSerial(Serial);
    if (Serial.available())
    {
        Serial.println("XBee Serial Communication Successful");
    }
}*/