#ifndef DC_MOTOR_H
#define DC_MOTOR_H

enum motion {
  UP,
  DOWN,
  STOP
};

enum states {
  STARTING,
  MOVING,
  STOPPING,
  STOPPED
};

class DCMotor {
private:

  byte pwmPin;
  byte directionPin;
  
  motion prevDirection;
  states prevState;
  int maxLoad;
  int prevDutyCycle;

  int motion2PinSignal(motion m);
  void move(motion m, int dutyCycle);

public:
  DCMotor(byte pwmPin, byte directionPin);
  void init();
  void up();
  void down();
  void stop();
};

#endif