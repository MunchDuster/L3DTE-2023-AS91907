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
const double refreshRateHertz = 100;
const double targetAngleDegrees = 90;
const int calibrationReadings = 200;
const int calibrationTimeMilliseconds = 1000;
const int accelerometerTestConnectionFailDelayMilliseconds = 500;

//Constants (Not for control)
const double gyroScaleFactor = 1.0 / 131.0; //Magic number from who knows where
double deltaTimeSeconds = 1.0 / refreshRateHertz;
int deltaTimeMilliseconds = (int)(deltaTimeSeconds * 1000.0);
//Main vars
MPU6050 accelgyro;
int16_t rawGyroZ; //Z angle is yaw
double deltaAngleDegrees, deltaAngleDegreesOffset;
double angleDegrees, error, outputPID, proportionalValue, integratedValue, derivativeValue;

void setup() {
  //Initialize serial
  Serial.begin(9600);

  //Initialize MPU6050
  Wire.begin();
  accelgyro.initialize();
  while(!accelgyro.testConnection()) {
    Serial.println("MPU6050 connection failed");
    delay(accelerometerTestConnectionFailDelayMilliseconds);
  }
  Serial.println("MPU6050 connection successful");

  //Calibrate MPU6050
  for(int i = 0;i < calibrationReadings; i++) {
    deltaAngleDegreesOffset += GetDeltaAngle(false); 
    delay(deltaTimeMilliseconds);
  }
  deltaAngleDegreesOffset /= (double) calibrationReadings;
}

void loop() {
  //Update angle
  GetDeltaAngle(false);
  angleDegrees += deltaAngleDegrees;

  //PID control
  error = targetAngleDegrees - angleDegrees; //Offset from target
  derivativeValue = -deltaAngleDegrees * derivativeFactor; //Slow down to prevent overshoot
  proportionalValue = error * proportionalFactor; //Pull towards target
  integratedValue += error * integratedFactor; //Improve accuracy on target
  outputPID = derivativeValue + proportionalValue + integratedValue;

  //Output and delay
  Serial.println(angleDegrees);
  //Serial.println(outputPID);
  delay(deltaTimeMilliseconds);
}
double GetDeltaAngle(bool useOffset) {
  rawGyroZ = accelgyro.getRotationZ(); //Read value from MPU6050
  deltaAngleDegrees = (double)rawGyroZ  * gyroScaleFactor * deltaTimeSeconds; //Convert to degrees/sec
  if(useOffset) deltaAngleDegrees -= deltaAngleDegreesOffset;
}
