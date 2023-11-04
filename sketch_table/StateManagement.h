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

struct MotorState{
  motorDirectionEnum currentMotorDirection = STOP;
  motorStateEnum currentMotorState = STOPPED;
  int currentMotorDutyCycle = 0;
};

struct TablePosition {
  //TODO read those values from eeprom
  long leftHallSensorCounter = 0;
  long rightHallSensorCounter = 0;

  private:
    Storage leftLegStorage{8};  //address 8
    Storage rightLegStorage{12};
  
  // gyro measurements can go here as well
};

#endif