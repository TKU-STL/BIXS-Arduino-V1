#include <Arduino.h>

void SysCom_Init(long int BAUDRATE)
{
  Serial.begin(BAUDRATE);
  Serial.println("System_COM_Init");
  if (Serial.available())
  {
    Serial.println("System Serial Communication Successful");
  }
}