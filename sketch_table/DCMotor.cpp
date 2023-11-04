#include "Arduino.h"
#include "DCMotor.h"
#include "StateManagement.h"


int DCMotor::motion2PinSignal(motorDirectionEnum m) {
  int dir;
  switch (m) {
    case UP:
      dir = LOW;
      break;
    case DOWN:
      dir = HIGH;
      break;
    case STOP:
      dir = LOW;
      break;
    default:
      break;
  }
  return dir;
}

DCMotor::DCMotor(byte pwmPin, byte directionPin, int maxLoad) {
  this->directionPin = directionPin;
  this->pwmPin = pwmPin;
  this->maxLoad = maxLoad;
  this->ms = MotorState();
  this->errorCode = 0;
}

void DCMotor::init() {
  pinMode(this->directionPin, OUTPUT);
  pinMode(this->pwmPin, OUTPUT);
  // set to zeros control ports;
  digitalWrite(this->directionPin, 0);
  analogWrite(this->pwmPin, 0);
}

void DCMotor::move(motorDirectionEnum m, int dutyCycle) {
  int dir = this->motion2PinSignal(m);
  digitalWrite(this->directionPin, dir);
  analogWrite(this->pwmPin, dutyCycle);
}

motorStateEnum DCMotor::determineState(motorDirectionEnum intendedDirection) {
  motorStateEnum result;
  if (intendedDirection == UP) {
    if (this->ms.currentMotorDirection == STOP) {
      // it should be starting
      result = STARTING;
    } else if (this->ms.currentMotorDirection == UP) {
      // its either STARTING OR MOVING depending on dutyCycle
      if (this->ms.currentMotorDutyCycle < this->maxLoad) {
        result = STARTING;
      } else {
        result = MOVING;
      }
    } else if (this->ms.currentMotorDirection == DOWN) {
      // its STOPPING or STOPPED depending on dutyCycle
      if (this->ms.currentMotorDutyCycle > 0) {
        result = STOPPING;
      } else {
        result = STOPPED;
      }
    } else {
      this->errorCode = 120;
    }
  } else if (intendedDirection == DOWN) {
    // TODO this if is almost identical to the one above. UP and DOWN is the only difference
    if (this->ms.currentMotorDirection == STOP) {
      // it should be starting
      result = STARTING;
    } else if (this->ms.currentMotorDirection == DOWN) {
      // its either STARTING OR MOVING depending on dutyCycle
      if (this->ms.currentMotorDutyCycle < this->maxLoad) {
        result = STARTING;
      } else {
        result = MOVING;
      }
    } else if (this->ms.currentMotorDirection == UP) {
      // its STOPPING or STOPPED depending on dutyCycle
      if (this->ms.currentMotorDutyCycle > 0) {
        result = STOPPING;
      } else {
        result = STOPPED;
      }
    } else {
      this->errorCode = 121;
    }
  } else if (intendedDirection == STOP) {
    if (this->ms.currentMotorDirection == STOP) {
      // it should be stopped
      result = STOPPED;
    } else if (this->ms.currentMotorDirection == DOWN) {
      // its either STOPPING OR STOPPED depending on dutyCycle
      if (this->ms.currentMotorDutyCycle > 0) {
        result = STOPPING;
      } else {
        result = STOPPED;
      }
    } else if (this->ms.currentMotorDirection == UP) {
      // its STOPPING or STOPPED depending on dutyCycle
      if (this->ms.currentMotorDutyCycle > 0) {
        result = STOPPING;
      } else {
        result = STOPPED;
      }
    } else {
      this->errorCode = 122;
    }
  } else {
    this->errorCode = 110;
  }
  return result;
}

motorDirectionEnum DCMotor::determineDirection(motorDirectionEnum intendedDirection, motorStateEnum calculatedState) {
  motorDirectionEnum result;
  if (intendedDirection == UP) {
    switch (calculatedState) {
      case STOPPED:
        result = UP;
        break;
      case STARTING:
        result = UP;
        break;
      case STOPPING:
        result = DOWN;
        break;
      case MOVING:
        result = UP;
        break;
      default:
        this->errorCode = 121;
        break;
    };
  } else if (intendedDirection == DOWN) {
    switch (calculatedState) {
      case STOPPED:
        result = DOWN;
        break;
      case STARTING:
        result = DOWN;
        break;
      case STOPPING:
        result = UP;
        break;
      case MOVING:
        result = DOWN;
        break;
      default:
        this->errorCode = 122;
        break;
    };
  } else if (intendedDirection == STOP) {
    switch (calculatedState) {
      case STOPPED:
        result = STOP;
        break;
      case STARTING:
        if (this->ms.currentMotorDirection == UP) {
          result = UP;
        } else if (this->ms.currentMotorDirection == DOWN) {
          result = DOWN;
        } else if (this->ms.currentMotorDirection == STOP) {
          //this state looks unreasonable. Setting
          this->errorCode = 125;
        } else {
          this->errorCode = 124;
        }
        break;
      case STOPPING:
        if (this->ms.currentMotorDirection == UP) {
          result = UP;
        } else if (this->ms.currentMotorDirection == DOWN) {
          result = DOWN;
        } else if (this->ms.currentMotorDirection == STOP) {
          //this state looks unreasonable. Setting
          this->errorCode = 126;
        } else {
          this->errorCode = 124;
        }
        break;
      case MOVING:
        if (this->ms.currentMotorDirection == UP) {
          result = UP;
        } else if (this->ms.currentMotorDirection == DOWN) {
          result = DOWN;
        } else if (this->ms.currentMotorDirection == STOP) {
          //this state looks unreasonable. Setting
          this->errorCode = 127;
        } else {
          this->errorCode = 124;
        }
        break;
      default:
        this->errorCode = 123;
        break;
    };
  } else {
    this->errorCode = 111;
  }
  return result;
}

int DCMotor::accelerationFunction(unsigned short step){
  // linear function with 25 steps y = 10x
  // int dutyCycleValue = (int)10 * step;

  //logarithmic function with 25 steps y = 74 * ln(x+1)
  // int dutyCycleValue = (int) 74 * log(step + 1);

  //logarithmic function with 25 steps y = 93 ln(0.5x + 1) or y = 101 ln(0.4x +1)
  int dutyCycleValue = (int) 101 * log(0.4 * step + 1);

  dutyCycleValue = min(dutyCycleValue, this->maxLoad);
  return dutyCycleValue;
}

int DCMotor::calculateDutyCycle(motorStateEnum state) {
  unsigned short dutyCycle;
  if (state == STOPPED) {
    dutyCycle = 0;
  } else if (state == STARTING) {
    dutyCycle = min(this->ms.currentMotorDutyCycle + this->dutyCycleIncrement, this->rampUpCycles - 1);  // -1 because count from 0
  } else if (state == MOVING) {
    dutyCycle = this->rampUpCycles;
  } else if (state == STOPPING) {
    dutyCycle = max(this->ms.currentMotorDutyCycle - this->dutyCycleIncrement, 0);
  } else {
    this->errorCode = 144;
  }
  
  return dutyCycle;
}

void DCMotor::updateState(motorDirectionEnum curMotDir, motorStateEnum curMotSt, int curMotDutyCycle) {
  this->ms.currentMotorDutyCycle = curMotDutyCycle;
  this->ms.currentMotorDirection = curMotDir;
  this->ms.currentMotorState = curMotSt;
}

int DCMotor::getDutyCycle() {
  return this->ms.currentMotorDutyCycle;
}

motorDirectionEnum DCMotor::getDirection() {
  return this->ms.currentMotorDirection;
}

motorStateEnum DCMotor::getState() {
  return this->ms.currentMotorState;
}

int DCMotor::getErrorCode() {
  return this->errorCode;
}

void DCMotor::intelligentMove(motorDirectionEnum intendedDirection) {
  //perception
  motorStateEnum calculatedState = this->determineState(intendedDirection);
  //planning
  motorDirectionEnum calculatedDirection = this->determineDirection(intendedDirection, calculatedState);
  int calculatedDutyCycle = this->calculateDutyCycle(calculatedState);
  int dutyCycleValue = this->accelerationFunction(calculatedDutyCycle);
  //controll
  this->move(calculatedDirection, dutyCycleValue);
  this->updateState(calculatedDirection, calculatedState, calculatedDutyCycle);
}

void DCMotor::up() {
  this->intelligentMove(UP);
}

void DCMotor::down() {
  this->intelligentMove(DOWN);
}

void DCMotor::stop() {
  this->intelligentMove(STOP);
}
