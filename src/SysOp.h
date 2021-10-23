#include <SFE_BMP180.h>
#include <SparkFunLSM9DS1.h>

void Alt_Update()
{
    double alt;
    // Get a new pressure reading
    // Show the relative altitude difference between
    // the new reading and the baseline reading:
    alt = Altimeter.altitude(pressure, baseline);
    double Time = millis();

    Serial.print("Time espiled: ");
    Serial.println(Time);

    Serial.print("relative altitude: ");
    if (alt >= 0.0)
        Serial.print(" "); // add a space for positive numbers
    Serial.print(alt, 1);
    Serial.print(" meters, ");
    if (alt >= 0.0)
        Serial.print(" "); // add a space for positive numbers
}