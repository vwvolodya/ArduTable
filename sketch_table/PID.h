#ifndef MY_PID_H
#define MY_PID_H

class PID {
private:
  float ki;
  float kp;
  float kd;

  int leftConter = 0;
  int rightCounter = 0;

  int leftControlSignal = 0;
  int rightControlSignal = 0;

  int lDelta = 0;
  int rDelta = 0;

  float leftMotorSpeed = 0;
  float rightMotorSpeed = 0;

  float leftMotorAcceleration = 0;
  float rightMotorAcceleration = 0;

  unsigned long currentTimestamp = 0;

public:
  PID();
  void setParams(float ki, float kp, float kd);

  void update(int lhall, int rhall, int leftInput, int rightInput);

};
#endif