#include <Arduino.h>
#include "LedIndicator.h"

void LedIndicator::init() {
  pinMode(this->pin, OUTPUT);
  this->off();
}

LedIndicator::LedIndicator(byte pin) {
  this->pin = pin;
}

void LedIndicator::on() {
  digitalWrite(this->pin, HIGH);
}

void LedIndicator::off() {
  digitalWrite(this->pin, LOW);
}

void LedIndicator::blinkN(unsigned int n) {
  // blink N times with interval between them
  for (unsigned int k = 0; k < n; k += 1) {
    this->on();
    this->off();
  }
}