//MPU6050 accelerometer test 3
//PID controller on the direction of the car to face the target direction

//Wiring:
//Arduino - MPU6050
//A5 - SCL/SDA
//A4 - SDA/SCL
//5V - VCC
//GND - GND

#include "MPU6050.h"

//Controls - Tuning required
const double proportionalFactor = 200;
const double integratedFactor = 50;
const double derivativeFactor = 700;
const double refreshRateHertz = 60;
const double targetAngleDegrees = 90;
const double gyroScaleFactor = 1.0 / 131.0; //Magic number from who knows where

//Main vars
MPU6050 accelgyro;
int16_t rawGyroZ; //Z angle is yaw
uint32_t lastTimeMicroseconds, curTimeMicroseconds, deltaTimeSeconds;
double angleDegrees, deltaAngleDegrees, error, outputPID, proportionalValue, integratedValue, derivativeValue;

void setup() {
  //Initialize serial
  Serial.begin(9600);

  //Initialize MPU6050
  Wire.begin();
  accelgyro.initialize();
  Serial.print(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  
  //Initialize timer
  lastTimeMicroseconds = micros();
  curTimeMicroseconds = micros();
}

void loop() {
  //Timer//
  curTimeMicroseconds = micros();
  deltaTimeSeconds = (double)(curTimeMicroseconds - lastTimeMicroseconds) / 1000000.0; // Calculate delta time
  lastTimeMicroseconds = curTimeMicroseconds; // Update timer

  //Data processing//
  rawGyroZ = accelgyro.getRotationZ(); //Read value from MPU6050
  deltaAngleDegrees = (double)rawGyroZ  * gyroScaleFactor * deltaTimeSeconds; //Convert to degrees/sec
  angleDegrees += deltaAngleDegrees;

  //PID control//
  error = targetAngleDegrees - angleDegrees; //Offset from target
  derivativeValue = -deltaAngleDegrees * derivativeFactor; //Slow down to prevent overshoot
  proportionalValue = error * proportionalFactor; //Pull towards target
  integratedValue += error * integratedFactor; //Improve accuracy on target
  outputPID = derivativeValue + proportionalValue + integratedValue;

  //Output and loop delay//
  Serial.println(angleDegrees); //Log angle to serial
  Serial.println(outputPID); //Log angle to serial
  delay((int)(1000.0 / refreshRateHertz));
}
