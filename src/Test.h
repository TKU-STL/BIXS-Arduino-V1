#include<Arduino.h>

void TestDataStream()
{
    Serial.println("TestDataStream");
    Serial.println("-----IMU----");
    Serial.println("X: 1.0"); 
    Serial.println("Y: 1.0");
    Serial.println("Z: 1.0"); 
    Serial.println();
    Serial.println("-----Gyroscope----");
    Serial.println("Alpha: 1.0"); 
    Serial.println("Beta: 1.0"); 
    Serial.println("Tetha: 1.0"); 
    Serial.println();
    Serial.println("-----Altimeter----");
    Serial.println("Pressure: 1000hpa");
    Serial.println("Altitude: 1.0"); 
    Serial.println();
}