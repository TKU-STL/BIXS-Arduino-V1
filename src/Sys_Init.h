#include <Wire.h>
#include <XBee.h>
#include <Arduino.h>
#include <SFE_BMP180.h>
#include <SoftwareSerial.h>
#include <SparkFunLSM9DS1.h>

LSM9DS1 IMU;
SFE_BMP180 Altimeter;
XBee XBee_A = XBee();
SoftwareSerial nss(8,9);

double pressure, baseline;

double getPressure()
{
    char status;
    double T, P;

    // You must first get a temperature measurement to perform a pressure reading.

    // Start a temperature measurement:
    // If request is successful, the number of ms to wait is returned.
    // If request is unsuccessful, 0 is returned.

    status = Altimeter.startTemperature();
    if (status != 0)
    {
        // Wait for the measurement to complete:

        delay(status);

        // Retrieve the completed temperature measurement:
        // Note that the measurement is stored in the variable T.
        // Use '&T' to provide the address of T to the function.
        // Function returns 1 if successful, 0 if failure.

        status = Altimeter.getTemperature(T);
        if (status != 0)
        {
            // Start a pressure measurement:
            // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
            // If request is successful, the number of ms to wait is returned.
            // If request is unsuccessful, 0 is returned.
            status = Altimeter.startPressure(3);
            if (status != 0)
            {
                // Wait for the measurement to complete:
                delay(status);

                // Retrieve the completed pressure measurement:
                // Note that the measurement is stored in the variable P.
                // Use '&P' to provide the address of P.
                // Note also that the function requires the previous temperature measurement (T).
                // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
                // Function returns 1 if successful, 0 if failure.

                status = Altimeter.getPressure(P, T);
                if (status != 0)
                {
                    return P;
                }
                else
                {
                    Serial.println("error retrieving pressure measurement\n");
                }
            }
            else
            {
                Serial.println("error starting pressure measurement\n");
                Serial.println("error starting temperature measurement\n");
                Serial.println("error retrieving temperature measurement\n");
            }
        }
    }
    return P;
}

void SysCom_Init(long int BAUDRATE)
{
    Serial.begin(BAUDRATE);
    Serial.println("System_COM_Init");
    if (Serial.available())
    {
        Serial.println("System Serial Communication Successful");
    }
}

void Alt_Init()
{
    if (Altimeter.begin())
    {
        Serial.println("BMP180 init success");
    }
    else
    {
        // Oops, something went wrong, this is usually a connection problem,
        // see the comments at the top of this sketch for the proper connections.
        Serial.println("BMP180 init fail (disconnected?)\n\n");
        while (1)
            ; // Pause forever.
    }

    double baseline;
    baseline = getPressure();

    Serial.print("baseline pressure: ");
    Serial.print(baseline);
    Serial.println(" mb");
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

void XBee_Init()
{
    Serial.println("XBee_Init");
    XBee_A.setSerial(Serial);
    if (Serial.available())
    {
        Serial.println("XBee Serial Communication Successful");
    }
}