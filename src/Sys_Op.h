#include <Arduino.h>
#include <SFE_BMP180.h>
#include <SparkFunLSM9DS1.h>

#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG 0x6B
#define PRINT_CALCULATED
#define PRINT_SPEED 250 // 250 ms between prints
static unsigned long lastPrint = 0;
#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.
#define ALTITUDE 57

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
  /*
    double alt;
    double baseline;
    baseline = getPressure();
    double sea_level_pressure;
    sea_level_pressure = Altimeter.sealevel(baseline, ALTITUDE);

    Serial.print("baseline pressure: ");
    Serial.print(baseline);
    Serial.println(" mb");
    // Get a new pressure reading
    // Show the relative altitude difference between
    // the new reading and the baseline reading:
    alt = Altimeter.altitude(pressure, sea_level_pressure);
    
    Serial.print("relative altitude: ");
    if (alt >= 0.0){
      Serial.print(" "); // add a space for positive numbers
    }
    Serial.print(alt);
    Serial.println(" meters, ");
*/

char status;
  double T,P,p0,a;

  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  
  Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE*3.28084,0);
  Serial.println(" feet");
  
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

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
    // Function returns 1 if successful, 0 if failure.

    status = Altimeter.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      
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
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = Altimeter.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = Altimeter.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = Altimeter.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a, 4);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  //delay(5000);  // Pause for 5 seconds.
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
  Mission_Clock_Time = Mission_Clock_Time/1000;
  Serial.print("Mission Clock Time (in second): ");
  Serial.println(Mission_Clock_Time);
}

void XBee_Com(char command)
{
  if(command == 'l'){
    Serial.println("Launch Sequence Started");
    for(int i = 0; i < 10; i++){
      Serial.print("Time-minus: ");
      Serial.println(i);
      delay(1000);
    }
  }else if(command == 's'){
    Serial.println("Shutdown Sequence Started");
  }else if(command == 'r'){
    Serial.println("Reset Sequence Started");
  }else if(command == 'a'){
    Serial.println("Aborted");
  }else{
    Serial.println("Invalid Command");
  }
}

double Alt_Output()
{
  double alt;
  // Get a new pressure reading
  // Show the relative altitude difference between
  // the new reading and the baseline reading:
  alt = Altimeter.altitude(pressure, baseline);
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
