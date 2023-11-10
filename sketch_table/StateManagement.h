#ifndef STATE_MANAGEMENT_H
#define STATE_MANAGEMENT_H
#include "Storage.h"

enum motorDirectionEnum {
  UP,
  DOWN,
  STOP
};

enum motorStateEnum {
  STARTING,
  MOVING,
  STOPPING,
  STOPPED
};

enum tableControlEnum {
  MANUAL, 
  AUTO
};

struct MotorState{
  motorDirectionEnum currentMotorDirection = STOP;
  motorStateEnum currentMotorState = STOPPED;
  int currentMotorDutyCycle = 0;
};

struct TablePosition {
  //TODO read those values from eeprom
  long leftHallSensorCounter = 1000;    // starting with virtual setpoint of 10000 to prevent negative numbers;
  long rightHallSensorCounter = 1000;
  unsigned int epsilon = 18;  // threshold of difference between left and right motors. ~18 clicks in 1 cycle (17.65 by estimates)
  tableControlEnum mode = AUTO;

  bool isLeftFaster(int direction);
  bool isRightFaster(int direction);

  private:
    Storage leftLegStorage{8};  //address 8
    Storage rightLegStorage{12};

    bool auxilary(int direction, int diff);
  
  // gyro measurements can go here as well
};

#endif