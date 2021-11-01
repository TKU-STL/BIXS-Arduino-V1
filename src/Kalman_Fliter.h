#include <Arduino.h>
#include <Kalman.h>

using namespace BLA;

//------------------------------------
/****       KALMAN PARAMETERS    ****/
//------------------------------------

#define Nstate 2
#define Nobs 2

// measurement std (to be characterized from your sensors)
#define n1 0.2
#define n2 0.1

// model std (~1/inertia). Freedom you give to relieve your evolution equation
#define m1 0.01
#define m2 0.02

KALMAN<Nstate,Nobs> K; // your Kalman filter
BLA::Matrix<Nobs> obs; // observation vector

//-----------------------------------
/****     GET SENSOR DATA       ****/
//-----------------------------------

BLA::Matrix<Nobs> get_sensor_data_Alt(){
       // It is your job to fill in this method
       // grab data from your accelerometer, GPS, etc...
       //double imu_x = IMU_AccX();
       return Alt_Output();
}


BLA::Matrix<Nobs> get_sensor_data_AccX(){
       // It is your job to fill in this method
       // grab data from your accelerometer, GPS, etc...
       //double imu_x = IMU_AccX();
       return IMU_AccX();
}

BLA::Matrix<Nobs> get_sensor_data_AccY(){
       // It is your job to fill in this method
       // grab data from your accelerometer, GPS, etc...
       return IMU_AccY();
}

BLA::Matrix<Nobs> get_sensor_data_AccZ(){
       // It is your job to fill in this method
       // grab data from your accelerometer, GPS, etc...
       return IMU_AccZ();
}

BLA::Matrix<Nobs> get_sensor_data_GyroX(){
       // It is your job to fill in this method
       // grab data from your accelerometer, GPS, etc...
       return IMU_GyroX();
}

BLA::Matrix<Nobs> get_sensor_data_GyroY(){
       // It is your job to fill in this method
       // grab data from your accelerometer, GPS, etc...
       return IMU_GyroY();
}

BLA::Matrix<Nobs> get_sensor_data_GyroZ(){
       // It is your job to fill in this method
       // grab data from your accelerometer, GPS, etc...
       return IMU_GyroZ();
}

//-----------------------------------
/****           SETUP           ****/
//-----------------------------------

void Kalman_Init() {
  // example of evolution matrix
  K.F = {1.0, 0.0,
         0.0, 1.0};
  // example of measurement matrix
  K.H = {1.0, 0.0,
         0.0, 1.0};
  // example of measurement covariance matrix
  K.R = {n1*n1,   0.0,
           0.0, n2*n2};
  // example of model covariance matrix
  K.Q = {m1*m1,   0.0,
           0.0, m2*m2};
}

//-----------------------------------
/****            LOOP           ****/
//-----------------------------------

void Kalman_Update_Alt() 
{
  // eventually update your evolution matrix inside the loop
  K.F = {1.0,  0.2,
         0.0,  1.0};
  
  // GRAB MEASUREMENT
  obs = get_sensor_data_Alt();
  
  // APPLY KALMAN FILTER
  K.update(obs);

  // PRINT RESULTS: measures and estimated state
  Serial << obs << ' ' << K.x << '\n';
}

void Kalman_Update_AccX() 
{
  // eventually update your evolution matrix inside the loop
  K.F = {1.0,  0.2,
         0.0,  1.0};
  
  // GRAB MEASUREMENT
  obs = get_sensor_data_AccX();
  
  // APPLY KALMAN FILTER
  K.update(obs);

  // PRINT RESULTS: measures and estimated state
  Serial << obs << ' ' << K.x << '\n';
}

void Kalman_Update_AccY()
{
       // eventually update your evolution matrix inside the loop
       K.F = {1.0,  0.2,
               0.0,  1.0};
       
       // GRAB MEASUREMENT
       obs = get_sensor_data_AccY();
       
       // APPLY KALMAN FILTER
       K.update(obs);
       
       // PRINT RESULTS: measures and estimated state
       Serial << obs << ' ' << K.x << '\n';
}

void Kalman_Update_AccZ()
{
       // eventually update your evolution matrix inside the loop
       K.F = {1.0,  0.2,
               0.0,  1.0};
       
       // GRAB MEASUREMENT
       obs = get_sensor_data_AccZ();
       
       // APPLY KALMAN FILTER
       K.update(obs);
       
       // PRINT RESULTS: measures and estimated state
       Serial << obs << ' ' << K.x << '\n';
}

void Kalman_Update_GyroX()
{
       // eventually update your evolution matrix inside the loop
       K.F = {1.0,  0.2,
               0.0,  1.0};
       
       // GRAB MEASUREMENT
       obs = get_sensor_data_GyroX();
       
       // APPLY KALMAN FILTER
       K.update(obs);
       
       // PRINT RESULTS: measures and estimated state
       Serial << obs << ' ' << K.x << '\n';
}

void Kalman_Update_GyroY()
{
       // eventually update your evolution matrix inside the loop
       K.F = {1.0,  0.2,
               0.0,  1.0};
       
       // GRAB MEASUREMENT
       obs = get_sensor_data_GyroY();
       
       // APPLY KALMAN FILTER
       K.update(obs);
       
       // PRINT RESULTS: measures and estimated state
       Serial << obs << ' ' << K.x << '\n';
}

void Kalman_Update_GyroZ()
{
       // eventually update your evolution matrix inside the loop
       K.F = {1.0,  0.2,
               0.0,  1.0};
       
       // GRAB MEASUREMENT
       obs = get_sensor_data_GyroZ();
       
       // APPLY KALMAN FILTER
       K.update(obs);
       
       // PRINT RESULTS: measures and estimated state
       Serial << obs << ' ' << K.x << '\n';
}