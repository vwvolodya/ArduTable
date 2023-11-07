#ifndef MY_LED_H
#define MY_LED_H

class LedIndicator {
private:
  byte pin;
  
public:
  LedIndicator(byte pin);
  void init();
  void on();
  void off();
  void blinkN(unsigned int n);
};
#endif