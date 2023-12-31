#ifndef MY_BUTTON_H
#define MY_BUTTON_H

class PushButton {
private:
  byte pin;
  byte state;
  byte lastReading;
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay;
  
public:
void init(unsigned long debounceDelay=50);
  PushButton(byte pin);
  bool isPressed();
  byte getState();
  void update();
};
#endif