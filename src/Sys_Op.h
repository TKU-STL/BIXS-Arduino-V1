#include <Arduino.h>
#include <SparkFunLSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG 0x6B
#define PRINT_CALCULATED
#define PRINT_SPEED 250 // 250 ms between prints
static unsigned long lastPrint = 0;
#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.
#define ALTITUDE 57

/* Get a new sensor event */
sensors_event_t event;
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

void printGyro()
{
  // Now we can use the gx, gy, and gz variables as we please.
  // Either print them as raw ADC values, or calculated in DPS.
  Serial.print("G: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcGyro helper function to convert a raw ADC value to
  // DPS. Give the function the value that you want to convert.
  Serial.print(IMU.calcGyro(IMU.gx), 2);
  Serial.print(", ");
  Serial.print(IMU.calcGyro(IMU.gy), 2);
  Serial.print(", ");
  Serial.print(IMU.calcGyro(IMU.gz), 2);
  Serial.println(" deg/s");
#elif defined PRINT_RAW
  Serial.print(imu.gx);
  Serial.print(", ");
  Serial.print(imu.gy);
  Serial.print(", ");
  Serial.println(imu.gz);
#endif
}

void printAccel()
{
  // Now we can use the ax, ay, and az variables as we please.
  // Either print them as raw ADC values, or calculated in g's.
  Serial.print("A: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcAccel helper function to convert a raw ADC value to
  // g's. Give the function the value that you want to convert.
  Serial.print(IMU.calcAccel(IMU.ax), 2);
  Serial.print(", ");
  Serial.print(IMU.calcAccel(IMU.ay), 2);
  Serial.print(", ");
  Serial.print(IMU.calcAccel(IMU.az), 2);
  Serial.println(" g");
#elif defined PRINT_RAW
  Serial.print(imu.ax);
  Serial.print(", ");
  Serial.print(imu.ay);
  Serial.print(", ");
  Serial.println(imu.az);
#endif
}

void printMag()
{
  // Now we can use the mx, my, and mz variables as we please.
  // Either print them as raw ADC values, or calculated in Gauss.
  Serial.print("M: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcMag helper function to convert a raw ADC value to
  // Gauss. Give the function the value that you want to convert.
  Serial.print(IMU.calcMag(IMU.mx), 2);
  Serial.print(", ");
  Serial.print(IMU.calcMag(IMU.my), 2);
  Serial.print(", ");
  Serial.print(IMU.calcMag(IMU.mz), 2);
  Serial.println(" gauss");
#elif defined PRINT_RAW
  Serial.print(IMU.mx);
  Serial.print(", ");
  Serial.print(IMU.my);
  Serial.print(", ");
  Serial.println(IMU.mz);
#endif
}

// Calculate pitch, roll, and heading.
// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));

  float heading;
  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);

  heading -= DECLINATION * PI / 180;

  if (heading > PI)
    heading -= (2 * PI);
  else if (heading < -PI)
    heading += (2 * PI);

  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll *= 180.0 / PI;

  Serial.print("Pitch, Roll: ");
  Serial.print(pitch, 2);
  Serial.print(", ");
  Serial.println(roll, 2);
  Serial.print("Heading: ");
  Serial.println(heading, 2);
}

void Alt_Update()
{
  Altimeter.getEvent(&event);
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");

    /* Calculating altitude with reasonable accuracy requires pressure    *
     * sea level pressure for your position at the moment the data is     *
     * converted, as well as the ambient temperature in degress           *
     * celcius.  If you don't have these values, a 'generic' value of     *
     * 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA   *
     * in sensors.h), but this isn't ideal and will give variable         *
     * results from one day to the next.                                  *
     *                                                                    *
     * You can usually find the current SLP value by looking at weather   *
     * websites or from environmental information centers near any major  *
     * airport.                                                           *
     *                                                                    *
     * For example, for Paris, France you can check the current mean      *
     * pressure and sea level at: http://bit.ly/16Au8ol                   */

    /* First we get the current temperature from the BMP085 */
    float temperature;
    Altimeter.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    //float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print("Altitude:    ");
    Serial.print(Altimeter.pressureToAltitude(seaLevelPressure,
                                              event.pressure));
    Serial.println(" m");
    Serial.println("");
  }
  else
  {
    Serial.println("Sensor error");
  }
  delay(1000);
}

void IMU_Update()
{
  if (IMU.gyroAvailable())
  {
    // To read from the gyroscope,  first call the
    // readGyro() function. When it exits, it'll update the
    // gx, gy, and gz variables with the most current data.
    IMU.readGyro();
  }
  if (IMU.accelAvailable())
  {
    // To read from the accelerometer, first call the
    // readAccel() function. When it exits, it'll update the
    // ax, ay, and az variables with the most current data.
    IMU.readAccel();
  }
  if (IMU.magAvailable())
  {
    // To read from the magnetometer, first call the
    // readMag() function. When it exits, it'll update the
    // mx, my, and mz variables with the most current data.
    IMU.readMag();
  }

  if ((lastPrint + PRINT_SPEED) < millis())
  {
    printGyro();  // Print "G: gx, gy, gz"
    printAccel(); // Print "A: ax, ay, az"
    printMag();   // Print "M: mx, my, mz"
    // Print the heading and orientation for fun!
    // Call print attitude. The LSM9DS1's mag x and y
    // axes are opposite to the accelerometer, so my, mx are
    // substituted for each other.
    printAttitude(IMU.ax, IMU.ay, IMU.az,
                  -IMU.my, -IMU.mx, IMU.mz);
    Serial.println();

    lastPrint = millis(); // Update lastPrint time
  }
  Serial.println("--------------------------------------------------------");
  delay(100);
}

void Mission_Clock_Update()
{
  double Mission_Clock_Time = millis();
  Mission_Clock_Time = Mission_Clock_Time / 1000;
  Serial.print("Mission Clock Time (in second): ");
  Serial.println(Mission_Clock_Time);
}

void XBee_Com(char command)
{
  if (command == 'l')
  {
    Serial.println("Launch Sequence Started");
    for (int i = 0; i < 10; i++)
    {
      Serial.print("Time-minus: ");
      Serial.println(i);
      delay(1000);
    }
  }
  else if (command == 's')
  {
    Serial.println("Shutdown Sequence Started");
  }
  else if (command == 'r')
  {
    Serial.println("Reset Sequence Started");
  }
  else if (command == 'a')
  {
    Serial.println("Aborted");
  }
  else
  {
    Serial.println("Invalid Command");
  }
}

double Alt_Output()
{
  double alt;
  alt = Altimeter.pressureToAltitude(seaLevelPressure, event.pressure);
  // Get a new pressure reading
  // Show the relative altitude difference between
  // the new reading and the baseline reading:
  //alt = Altimeter.altitude(pressure, baseline);
  return alt;
}

double IMU_AccX()
{
  return IMU.ax;
}

double IMU_AccY()
{
  return IMU.ay;
}

double IMU_AccZ()
{
  return IMU.az;
}

double IMU_GyroX()
{
  return IMU.gx;
}

double IMU_GyroY()
{
  return IMU.gy;
}

double IMU_GyroZ()
{
  return IMU.gz;
}
