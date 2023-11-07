#ifndef DC_MOTOR_H
#define DC_MOTOR_H
#include "StateManagement.h"


class DCMotor {
private:
  byte pwmPin;
  byte directionPin;
  int maxLoad;
  MotorState ms;
  int errorCode;
  unsigned short rampUpCycles = 25;         // nymber of cycles for soft start / soft shutdown
  unsigned short dutyCycleIncrement = 1;    // counting cycles instead of actual value. The value itself is a function of this number
  
  int motion2PinSignal(motorDirectionEnum m);
  void move(motorDirectionEnum m, int dutyCycle);
  motorStateEnum determineState(motorDirectionEnum intendedDirection);
  motorDirectionEnum determineDirection(motorDirectionEnum intendedDirection, motorStateEnum calculatedState);
  int calculateDutyCycle(motorStateEnum state);
  void updateState(motorDirectionEnum curMotDir, motorStateEnum curMotSt, int curMotDutyCycle);
  void intelligentMove(motorDirectionEnum intendedDirection);

  int accelerationFunction(unsigned short step);

public:
  DCMotor(byte pwmPin, byte directionPin, int maxLoad);

  motorDirectionEnum getDirection();
  motorStateEnum getState();
  int getDutyCycle();

  int getErrorCode();
  void init();

  void up();
  void down();
  void stop();
};

#endif