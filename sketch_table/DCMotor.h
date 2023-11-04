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
  
  int motion2PinSignal(motorDirectionEnum m);
  void move(motorDirectionEnum m, int dutyCycle);
  motorStateEnum determineState(motorDirectionEnum intendedDirection);
  motorDirectionEnum determineDirection(motorDirectionEnum intendedDirection, motorStateEnum calculatedState);
  int calculateDutyCycle(motorStateEnum state);
  void updateState(motorDirectionEnum curMotDir, motorStateEnum curMotSt, int curMotDutyCycle);
  void intelligentMove(motorDirectionEnum intendedDirection);

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