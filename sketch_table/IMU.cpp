#include "Arduino.h"
#include "IMU.h"

void IMU::init(bool estimateError) {
  Wire.begin();                           // Initialize comunication
  Wire.beginTransmission(this->address);  // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                       // Talk to the register 6B
  Wire.write(0x00);                       // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);

  if (estimateError) {
    this->estimateError();
  }

  // this->configAccelerometer();
  // this->configGyro();
}

IMU::IMU() {
  this->address = 0x68;
  this->currentTime = millis();
}

void IMU::configAccelerometer() {
  Wire.beginTransmission(this->address);
  Wire.write(0x1C);  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
}

void IMU::configGyro() {
  Wire.beginTransmission(this->address);
  Wire.write(0x1B);  // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);  // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
}

Measurements IMU::getAccelerometerMeasurements() {
  Wire.beginTransmission(this->address);
  Wire.write(0x3B);  // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(this->address, 6, true);  // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  float AccX = (Wire.read() << 8 | Wire.read()) / 16384.0;  // X-axis value
  float AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;  // Y-axis value
  float AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;  // Z-axis value

  Measurements result = { AccX, AccY, AccZ };
  return result;
}

Measurements IMU::getGyroMeasurements() {
  this->currentTime = millis();
  Wire.beginTransmission(this->address);
  Wire.write(0x43);  // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(this->address, 6, true);          // Read 4 registers total, each axis value is stored in 2 registers
  float GyroX = (Wire.read() << 8 | Wire.read()) / 131.0;  // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  float GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  float GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  Measurements result = { GyroX, GyroY, GyroZ };
  return result;
}

Measurements IMU::getAngles() {
  Measurements angles = { 0, 0, 0 };

  Measurements acc = this->getAccelerometerMeasurements();
  float x = (atan(acc.y / sqrt(pow(acc.x, 2) + pow(acc.z, 2))) * 180 / PI) + this->estimatedAccError.x;       // AccErrorX
  float y = (atan(-1 * acc.x / sqrt(pow(acc.y, 2) + pow(acc.z, 2))) * 180 / PI) + this->estimatedAccError.y;  // AccErrorY

  Measurements gyro = this->getGyroMeasurements();

  // Correct the outputs with the calculated error values
  gyro.x += this->estimatedGyroError.x;
  gyro.y += this->estimatedGyroError.y;
  gyro.z += this->estimatedGyroError.z;

  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  //TODO sort this shit out. time intervals are unclear
  // gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  // gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  // angles.z =  yaw + GyroZ * elapsedTime;
  // Complementary filter - combine acceleromter and gyro angle values
  angles.x = 0.96 * gyro.x + 0.04 * x;
  angles.y = 0.96 * gyro.y + 0.04 * y;

  return angles;
}

void IMU::estimateError() {
  //estimating Accelerometer
  float AccErrorX = 0;
  float AccErrorY = 0;
  for (short i = 0; i < 200; i++) {
    Measurements acc = this->getAccelerometerMeasurements();
    AccErrorX = AccErrorX + ((atan((acc.y) / sqrt(pow((acc.x), 2) + pow((acc.z), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (acc.x) / sqrt(pow((acc.y), 2) + pow((acc.z), 2))) * 180 / PI));
  }
  this->estimatedAccError.x = AccErrorX / 200;
  this->estimatedAccError.y = AccErrorY / 200;

  // estimating Gyro
  float GyroErrorX = 0;
  float GyroErrorY = 0;
  float GyroErrorZ = 0;

  for (short i = 0; i < 200; i++) {
    Measurements gyro = this->getGyroMeasurements();
    GyroErrorX = GyroErrorX + (gyro.x / 131.0);
    GyroErrorY = GyroErrorY + (gyro.y / 131.0);
    GyroErrorZ = GyroErrorZ + (gyro.z / 131.0);
  }
  this->estimatedGyroError.x = GyroErrorX / 200;
  this->estimatedGyroError.y = GyroErrorY / 200;
  this->estimatedGyroError.z = GyroErrorZ / 200;
}

Measurements IMU::getAltAccelerometerMeasurements(){
  Wire.beginTransmission(this->address);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(this->address, 14, true);
  int AcX = Wire.read() << 8 | Wire.read();
  int AcY = Wire.read() << 8 | Wire.read();
  int AcZ = Wire.read() << 8 | Wire.read();
  Measurements result = {AcX, AcY, AcZ};
  return result;
}

Measurements IMU::getData() {
  int minVal = 265;
  int maxVal = 402;
  Measurements acc = this->getAltAccelerometerMeasurements();
  int xAng = map((int)acc.x, minVal, maxVal, -90, 90);
  int yAng = map((int)acc.y, minVal, maxVal, -90, 90);
  int zAng = map((int)acc.z, minVal, maxVal, -90, 90);

  float x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  float y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  float z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
  Measurements result = {x, y, z};
  return result;
}