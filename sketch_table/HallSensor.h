#ifndef HALL_SENSOR_H
#define HALL_SENSOR_H
class HallSensor {
private:
  byte pin;

public:
  HallSensor(byte pin);
  void init();
  bool isActivated();
  unsigned int getCycles();
};
#endif