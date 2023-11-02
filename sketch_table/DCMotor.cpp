#include "Arduino.h"
#include "DCMotor.h"

#define DUTY_CYCLE_INCREMENT 10


int DCMotor::motion2PinSignal(motion m) {
  int dir;
  switch (m) {
    case UP:
      dir = LOW;
      break;
    case DOWN:
      dir = HIGH;
      break;
    default:
      break;
  }
  return dir;
}

DCMotor::DCMotor(byte pwmPin, byte directionPin) {
  this->directionPin = directionPin;
  this->pwmPin = pwmPin;
  this->prevDirection = STOP;
  this->prevState = STOPPED;
  this->prevDutyCycle = 0;
  this->maxLoad = 240;
}

void DCMotor::init() {
  pinMode(this->directionPin, OUTPUT);
  pinMode(this->pwmPin, OUTPUT);
  digitalWrite(this->directionPin, 0);
  analogWrite(this->pwmPin, 0);
}

void DCMotor::move(motion m, int dutyCycle) {
  int dir = this->motion2PinSignal(m);
  digitalWrite(this->directionPin, dir);
  analogWrite(this->pwmPin, dutyCycle);
}

void DCMotor::up() {
  int currentDutyCycle;
  states curState;
  if (this->prevDirection == UP) {
    currentDutyCycle = max(this->prevDutyCycle + DUTY_CYCLE_INCREMENT, this->maxLoad);
    if (currentDutyCycle <= this->maxLoad) {
      curState = STARTING;
    } else {
      curState = MOVING;
    }
  }
  else if (this->prevDirection == DOWN) {
    // this shoud be an error as well.
  }
  else if (this->prevDirection == STOP) {
    currentDutyCycle = max(this->prevDutyCycle + DUTY_CYCLE_INCREMENT, 0);
    curState = STARTING;
  }
  else {
    // Not really clear what to do. Its an exception;
  }
  this->move(UP, currentDutyCycle);
  this->prevDutyCycle = currentDutyCycle;
  this->prevDirection = UP;
  this->prevState = curState;
}

void DCMotor::down() {
}

void DCMotor::stop() {
  int currentDutyCycle;
  states curState;
  motion curDir;
  if (this->prevDirection == UP) {
    currentDutyCycle = max(this->prevDutyCycle - DUTY_CYCLE_INCREMENT, 0);
    curDir = UP;
    if (currentDutyCycle == 0) {
      curState = STOPPED;
    } else {
      curState = STOPPING;
    }
  }
  else if (this->prevDirection == STOP) {
    curDir = STOP;
    curState = STOPPED;
  }
  else if (this->prevDirection == DOWN) {
    curDir = DOWN;
  }
  else {
  }
  if (curDir != STOP) 
  { 
    this->move(curDir, currentDutyCycle); 
  }
  this->prevDutyCycle = currentDutyCycle;
  this->prevDirection = curDir;
  this->prevState = curState;
}
