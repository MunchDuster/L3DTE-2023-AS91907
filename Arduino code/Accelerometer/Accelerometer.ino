#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Kalman.h"

MPU6050 accelgyro;
Kalman kalmanX;

int16_t ax, ay, az;
int16_t gx, gy, gz;

double accX, accY, accZ;
double gyroX;
double gyroXangle, kalAngleX;

uint32_t timer; // Adding timer variable

void setup() {
    Wire.begin();
    Serial.begin(9600);
    accelgyro.initialize();

    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
   
    // Initialize kalman filter with the initial angle
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    accX = atan((double)ay / sqrt((double)ax * ax + (double)az * az)) * RAD_TO_DEG;
    kalmanX.setAngle(accX);

    timer = micros(); // Initialize timer at the end of setup
}

void loop() {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
   
    // Calculate pitch and roll from the raw data
    accX = atan((double)ay / sqrt((double)ax * ax + (double)az * az)) * RAD_TO_DEG;
    gyroX = (double)gx / 131.0;

    // Apply the complementary filter to figure out the change in angle - choice of alpha is
    // estimated now, 1/8 cyc^-1 where a conservative because 1/20 cyc^-1 in accelerometer
    // 1/256 s^-1 in gyro and 0.01 alpha = 0.04 approx = 1/8
    // This isn't a true complementary filter due to the code, which has different time values
    // To make it a true complementary filter, dt should be an exact time difference
    double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
    timer = micros(); // Update the timer for the next loop

    gyroXangle = gyroX * dt; // Calculate gyro angle without any filter
    kalAngleX = kalmanX.getAngle(accX, gyroX, dt); // Calculate the angle using a Kalman filter

    Serial.print("Angle X: "); Serial.println(kalAngleX);

    delay(100);
}

