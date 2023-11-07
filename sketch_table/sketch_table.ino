#include "DCMotor.h"
#include "StateManagement.h"
#include "PushButton.h"
#include <LiquidCrystal_I2C.h>

#define PWM_MOTOR1_PIN 6
// right motor
#define PWM_MOTOR2_PIN 5
#define DIR_MOTOR1_PIN 7    
#define DIR_MOTOR2_PIN 4

#define UP_BUTTON_PIN 9
#define DOWN_BUTTON_PIN 10

#define HALL_1_PIN 2
#define HALL_2_PIN 3

// making interval longer for debugging
#define CONTROLL_INTERVAL 20
#define MOTOR_MAX_LOAD_DUTY_CYCLE 240
#define STARTUP_DELAY 2000

volatile int errorCode = 0;
volatile TablePosition tablePosition = TablePosition();

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void displayPrint(LiquidCrystal_I2C display, int firstValue, int secondValue){
  const int display_size = 16;

  char firstRowBuffer[display_size];
  firstRowBuffer[0] = '\0';
  
  char secondRowBuffer[display_size];
  secondRowBuffer[0] = '\0';

  snprintf(firstRowBuffer, sizeof(firstRowBuffer), "Value: %i", firstValue);
  snprintf(secondRowBuffer, sizeof(secondRowBuffer), "Value: %i", secondValue);

  display.clear();
  display.setCursor(0,0);
  display.print(firstRowBuffer);

  display.setCursor(0,1);
  display.print(secondRowBuffer);
}

DCMotor leftMotor = DCMotor(PWM_MOTOR1_PIN, DIR_MOTOR1_PIN, MOTOR_MAX_LOAD_DUTY_CYCLE);
DCMotor rightMotor = DCMotor(PWM_MOTOR2_PIN, DIR_MOTOR2_PIN, MOTOR_MAX_LOAD_DUTY_CYCLE);

PushButton upButton = PushButton(UP_BUTTON_PIN);
PushButton downButton = PushButton(DOWN_BUTTON_PIN);

unsigned long previousMillis;


void leftHallCallback() {
  if (leftMotor.getDirection() == UP) {
    tablePosition.leftHallSensorCounter += 1;
  } else if (leftMotor.getDirection() == DOWN) {
    tablePosition.leftHallSensorCounter -= 1;
  } else if (leftMotor.getDirection() == STOP) {
    errorCode = 21;
  }
}

void rightHallCallback(){
  if (rightMotor.getDirection() == UP) {
    tablePosition.rightHallSensorCounter += 1;
  } else if (rightMotor.getDirection() == DOWN) {
    tablePosition.rightHallSensorCounter -= 1;
  } else if (rightMotor.getDirection() == STOP) {
    errorCode = 31;
  }
}

void setup() {
  // init motors ASAP
  rightMotor.init();
  leftMotor.init();
  
  upButton.init();
  downButton.init();
  previousMillis = STARTUP_DELAY;  // setting starting point at 2 seconds from boot

  // Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(HALL_1_PIN, INPUT_PULLUP);
  pinMode(HALL_2_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(HALL_1_PIN), leftHallCallback, RISING);
  attachInterrupt(digitalPinToInterrupt(HALL_2_PIN), rightHallCallback, RISING);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= CONTROLL_INTERVAL && currentMillis > previousMillis) {
    previousMillis = currentMillis;
    upButton.update();
    downButton.update();
    if (upButton.isPressed()){
      leftMotor.up();
      rightMotor.up();
    }
    else if (downButton.isPressed()){
      leftMotor.down();
      rightMotor.down();
    }
    else{
      leftMotor.stop();
      rightMotor.stop();
    }

    // Serial.println(leftMotor.getDutyCycle(), DEC);
    displayPrint(lcd, tablePosition.leftHallSensorCounter, tablePosition.rightHallSensorCounter);
  }
}

