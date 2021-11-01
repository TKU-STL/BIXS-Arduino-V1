#include <Arduino.h>

void System_Clock(void){
    Serial.print("System_Clock: ");
    Serial.println(millis());
}