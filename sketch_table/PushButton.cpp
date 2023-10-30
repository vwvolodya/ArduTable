#include <Arduino.h>
#include "PushButton.h"


void PushButton::init() {
  pinMode(this->pin, INPUT);
  this->lastReading = LOW;
  this->update();
}

PushButton::PushButton(byte pin) {
  this->pin = pin;
  this->init();
}

bool PushButton::isPressed() {
  return (this->getState() == HIGH);
}

byte PushButton::getState() {
  this->update();
  return this->state;
}

void PushButton::update() {
  byte currentReading = digitalRead(this->pin);
  if (this->lastReading != currentReading) {
    this->lastDebounceTime = millis();
  }
  if (millis() - this->lastDebounceTime > this->debounceDelay) {
    // Update the 'state' attribute only if debounce is checked
    this->state = currentReading;
  }
  this->lastReading = currentReading;
}
