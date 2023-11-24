#include "Arduino.h"
#include "PID.h"

PID::PID(){
  this->currentTimestamp = millis();
}

void PID::setParams(float ki, float kp, float kd){
  this->kd = kd;
  this->kp = kp;
  this->ki = ki;
}

void PID::update(int lhall, int rhall, int leftInput, int rightInput){
  unsigned long currentTs = millis();
  int dt = currentTs - this->currentTimestamp;
  this->currentTimestamp = currentTs;

  int lDeltaSteps = abs(lhall - this->leftConter);
  int rDeltaSteps = abs(rhall - this->rightCounter);

  float output = this->kp * lDeltaSteps;


  this->leftConter = lhall;
  this->rightCounter = rhall;
}