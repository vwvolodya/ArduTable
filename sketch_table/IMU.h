#ifndef IMU_H
#define IMU_H
#include <Wire.h>

struct Measurements{
  float x;
  float y;
  float z;
};


class IMU {
private:
  short address;
  Measurements acc;
  Measurements gyro;

  Measurements estimatedAccError = {0, 0, 0};
  Measurements estimatedGyroError = {0, 0, 0};

  unsigned long currentTime;

  void configAccelerometer();
  void configGyro();

  Measurements getAccelerometerMeasurements();
  Measurements getGyroMeasurements();

  Measurements getAltAccelerometerMeasurements();

  void estimateError();


public:
  void init(bool estimateError);
  IMU();
  Measurements getAngles();
  Measurements getData();
};
#endif