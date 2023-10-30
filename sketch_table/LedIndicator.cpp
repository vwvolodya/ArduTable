#include <Arduino.h>
#include "LedIndicator.h"

void LedIndicator::init() {
  pinMode(this->pin, OUTPUT);
  this->off(0);
}

LedIndicator::LedIndicator(byte pin) {
  this->pin = pin;
}

void LedIndicator::on(unsigned int duration) {
  digitalWrite(this->pin, HIGH);
  delay(duration);
}

void LedIndicator::off(unsigned int duration) {
  digitalWrite(this->pin, LOW);
  delay(duration);
}

void LedIndicator::blinkN(unsigned int n, unsigned int interval) {
  // blink N times with interval between them
  for (unsigned int k = 0; k < n; k += 1) {
    this->on(interval);
    this->off(interval);
  }
}