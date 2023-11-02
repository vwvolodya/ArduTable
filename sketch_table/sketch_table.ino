#include "LedIndicator.h"
#include "Storage.h"
#include "DCMotor.h"

#define LED_INTERNAL_PIN 13

#define PWM_MOTOR1_PIN 6
#define PWM_MOTOR2_PIN 5    // right motor
#define DIR_MOTOR1_PIN 7
#define DIR_MOTOR2_PIN 4

#define HALL_1_PIN 2  // for motor 2 
#define HALL_2_PIN 3

#define EXTERNAL_LED_PIN 11
#define MOTOR_CONTROLL_INTERVAL 25

volatile unsigned int errorCode = 0;

volatile long leftHallSensorCounter = 0;
volatile long rightHallSensorCounter = 0;
volatile motion currentTableState = STOP;

volatile Storage leftLegStorage(8);  //address 8
volatile Storage rightLegStorage(12);

LedIndicator led1(LED_INTERNAL_PIN);

DCMotor leftMotor(PWM_MOTOR1_PIN, DIR_MOTOR1_PIN);
DCMotor rightMotor(PWM_MOTOR2_PIN, DIR_MOTOR2_PIN);

unsigned long previousMillis = 0;
unsigned long controlCycleCounter = 0;

void leftHallCallback() {
  if (currentTableState == UP) {
    leftHallSensorCounter += 1;
  } else if (currentTableState == DOWN) {
    leftHallSensorCounter -= 1;
  } else if (currentTableState == STOP) {
    errorCode = 1;
  }
  leftLegStorage.setLong(leftHallSensorCounter);
}
void rightHallCallback() {
  if (currentTableState == UP) {
    rightHallSensorCounter += 1;
  } else if (currentTableState == DOWN) {
    rightHallSensorCounter -= 1;
  } else if (currentTableState == STOP) {
    errorCode = 2;
  }
  rightLegStorage.setLong(rightHallSensorCounter);
}

void setup() {
  led1.init();
  leftMotor.init();
  rightMotor.init();

  pinMode(HALL_1_PIN, INPUT_PULLUP);
  pinMode(HALL_2_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(HALL_1_PIN), leftHallCallback, RISING);
  attachInterrupt(digitalPinToInterrupt(HALL_2_PIN), rightHallCallback, RISING);

  // leftHallSensorCounter = leftLegStorage.getLong();
  // rightHallSensorCounter = rightLegStorage.getLong();

  delay(1000);  // just to set up everything

  previousMillis = millis();  // setting starting point
  currentTableState = UP; //TODO get rid of this shit

}



void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= MOTOR_CONTROLL_INTERVAL) {
    previousMillis = currentMillis;
    controlCycleCounter += 1;
    leftMotor.up();
    rightMotor.up();
  }
}

