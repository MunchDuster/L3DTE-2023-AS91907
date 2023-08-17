//Deliverable 2 MPU6050 code test
//Simplified using some peer review for the most basic and reliable system

#include <MPU6050.h>

//Constants
const double minimumReading = -32768; //Lowest reading from sernsor
const double maximumReading = 32767; //Highest reading from sensor
const double gyroRangeDegreesPerSecond = 250; //The highest reading from sensor in degrees per second
const int deltaTimeMilliseconds = 10; //Milliseconds between iterations
const double deltaTimeSeconds = (double)deltaTimeMilliseconds / 1000.0;

//Variables
MPU6050 accelgyro;
int16_t ax, ay, az, gx, gy, gz;
double deltaAngleDegreesPerSecond, angleDegrees;

//Functions
void setup() {
  Serial.begin(9600); //Initialize serial
  accelgyro.initialize(); //Initialize MPU6050
  Serial.println(accelgyro.testConnection() ? "MPU Connection Success" : "MPU Connection FAIL");
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); //Read MPU6050 values
  deltaAngleDegreesPerSecond = map((double)gz, minimumReading, maximumReading, -gyroRangeDegreesPerSecond, gyroRangeDegreesPerSecond); //Scale value from output to degrees per second
  angleDegrees += deltaAngleDegreesPerSecond * deltaTimeSeconds; //Add delta to accumulate
  Serial.println(angleDegrees); //Log to serial
  delay(deltaTimeMilliseconds); //Wait for next iteration
}
