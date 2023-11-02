#include "Arduino.h"
#include "HallSensor.h"

HallSensor::HallSensor(byte pin) {
  this->pin = pin;
}

void HallSensor::init() {
  pinMode(this->pin, INPUT);
}

bool HallSensor::isActivated() {
  int value = digitalRead(this->pin);
  return (value == HIGH);
}

unsigned int HallSensor::getCycles() {
  unsigned int count = 0;
  float start = millis();
  bool currentState = false;    // not activated at the start
  // while true {
  //   if (this->isActivated) {
      
  //   }
  //   else{

  //   }
  // }
}
