#include <Arduino.h>
#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 Altimeter;

double baseline; // baseline pressure

double getPressure()
{
  char status;
  double T, P, p0, a;

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
          return (P);
        }
        else
          Serial.println("error retrieving pressure measurement\n");
      }
      else
        Serial.println("error starting pressure measurement\n");
    }
    else
      Serial.println("error retrieving temperature measurement\n");
  }
  else
    Serial.println("error starting temperature measurement\n");
}

void Alt_Init()
{
  Serial.println("Altimeter Initiated");

  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (Altimeter.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while (1)
      ; // Pause forever.
  }

  // Get the baseline pressure:

  baseline = getPressure();

  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb");
}

void Alt_Update()
{
  double alt, P;

  // Get a new pressure reading:

  P = getPressure();

  // Show the relative altitude difference between
  // the new reading and the baseline reading:

  alt = Altimeter.altitude(P, baseline);

  Serial.print("relative altitude: ");
  if (alt >= 0.0)
    Serial.print(" "); // add a space for positive numbers
  Serial.print(alt, 1);
  Serial.print(" meters, ");
  if (alt >= 0.0)
    Serial.print(" "); // add a space for positive numbers
  Serial.print(alt * 3.28084, 0);
  Serial.println(" feet");

  delay(500);
}

double Alt_Output()
{
  double alt_output = Altimeter.altitude(getPressure(), baseline);
  // Get a new pressure reading
  // Show the relative altitude difference between
  // the new reading and the baseline reading:
  //alt = Altimeter.altitude(pressure, baseline);
  return alt_output;
}