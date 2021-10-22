#include <Arduino.h>
#include <SFE_BMP180.h>
#include <SparkFunLSM9DS1.h>

void System_COM_Init(long int BAUDRATE){
    Serial.begin(BAUDRATE);
    Serial.println("System_COM_Init");
    if (Serial.available())
    {
        Serial.println("System Serial Communication Successful");
    }
}

void Alt_Init(){
    SFE_BMP180 Altimeter;
    if (Altimeter.begin())
    {
        Serial.println("BMP180 init success");
    }else{
        // Oops, something went wrong, this is usually a connection problem,
        // see the comments at the top of this sketch for the proper connections.

        Serial.println("BMP180 init fail (disconnected?)\n\n");
        while (1)
            ; // Pause forever.
    }
}

void IMU_Init(){
    // Initialize the accelerometer and gyroscope:
    LSM9DS1 imu;
    if (imu.begin() == false) // with no arguments, this uses default addresses (AG:0x6B, M:0x1E) and i2c port (Wire).
    {
        Serial.println("Failed to communicate with LSM9DS1.");
        Serial.println("Double-check wiring.");
        Serial.println("Default settings in this sketch will work for an out of the box LSM9DS1 Breakout, but may need to be modified if the board jumpers are.");
        while (1);
    }else{
        Serial.println("LSM9DS1 init success");
    }
}