#ifndef MY_LED_H
#define MY_LED_H

class LedIndicator {
private:
  byte pin;
  
public:
  LedIndicator(byte pin);
  void init();
  void on(unsigned int duration);
  void off(unsigned int duration);
  void blinkN(unsigned int n, unsigned int interval);
};
#endif