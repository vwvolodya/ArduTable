#include "StateManagement.h"

bool TablePosition::auxilary(int direction, int diff) {
  bool result = false;
  if (direction == UP) {
    result = diff > this->epsilon;
  } else if (direction == DOWN) {
    result = -diff > this->epsilon;
  } else {
    return result;
  }
}


bool TablePosition::isLeftFaster(int direction) {
  int diff = this->leftHallSensorCounter - this->rightHallSensorCounter;
  bool result = this->auxilary(direction, diff);
  return result;
}

bool TablePosition::isRightFaster(int direction) {
  int diff = this->rightHallSensorCounter - this->leftHallSensorCounter;
  bool result = this->auxilary(direction, diff);
  return result;
}