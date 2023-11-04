#include "DCMotor.h"
#include "StateManagement.h"
#include "PushButton.h"

#define LED_INTERNAL_PIN 13

#define PWM_MOTOR1_PIN 6
// right motor
#define PWM_MOTOR2_PIN 5
#define DIR_MOTOR1_PIN 7    
#define DIR_MOTOR2_PIN 4
#define UP_BUTTON_PIN 3
#define DOWN_BUTTON_PIN 2

// making interval longer for debugging
#define CONTROLL_INTERVAL 20
#define MOTOR_MAX_LOAD_DUTY_CYCLE 240
#define STARTUP_DELAY 5000

volatile TablePosition tablePosition = TablePosition();


DCMotor leftMotor(PWM_MOTOR1_PIN, DIR_MOTOR1_PIN, MOTOR_MAX_LOAD_DUTY_CYCLE);
DCMotor rightMotor(PWM_MOTOR2_PIN, DIR_MOTOR2_PIN, MOTOR_MAX_LOAD_DUTY_CYCLE);

PushButton upButton(UP_BUTTON_PIN);

unsigned long previousMillis;


void setup() {
  Serial.begin(9600);
  leftMotor.init();
  rightMotor.init();
  upButton.init();
  previousMillis = STARTUP_DELAY;  // setting starting point at 4 seconds from boot
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= CONTROLL_INTERVAL) {
    previousMillis = currentMillis;
    upButton.update();
    if (upButton.isPressed()){
      Serial.println("UP");
      leftMotor.up();
      rightMotor.up();
    }
    else{
      Serial.println("STOP");
      leftMotor.stop();
      rightMotor.stop();
    }

    Serial.println(leftMotor.getDutyCycle(), DEC);
    Serial.println(leftMotor.getDirection());
    Serial.println(leftMotor.getState(), DEC);
    Serial.println(leftMotor.getErrorCode(), DEC);
  }
}

