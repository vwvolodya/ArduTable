#include "DCMotor.h"
#include "StateManagement.h"
#include "PushButton.h"
#include "LedIndicator.h"
#include <LiquidCrystal_I2C.h>
#include "IMU.h"

#define DEBUG 0

#define PWM_MOTOR1_PIN 6
// right motor
#define PWM_MOTOR2_PIN 5
#define DIR_MOTOR1_PIN 7
#define DIR_MOTOR2_PIN 4

#define UP_BUTTON_PIN 9
#define DOWN_BUTTON_PIN 10

#define MODE_BUTTON_PIN 11

#define LED_PIN 13

#define HALL_1_PIN 2
#define HALL_2_PIN 3

// making interval longer for debugging
#define CONTROLL_INTERVAL 20
#define MOTOR_MAX_LOAD_DUTY_CYCLE 240
#define STARTUP_DELAY 2000

// for debugging we want to erase values stored in EEPROM
#define RESET_HALL_COUNTERS true

volatile int errorCode = 0;
volatile TablePosition tablePosition = TablePosition();

LedIndicator led = LedIndicator(LED_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

IMU imu = IMU();

void displayPrint(LiquidCrystal_I2C display, int lValue, int rValue, float lDuty = 0, float rDuty = 0) {
  const int display_size = 16;

  char firstRowBuffer[display_size];
  firstRowBuffer[0] = '\0';

  char secondRowBuffer[display_size];
  secondRowBuffer[0] = '\0';

  snprintf(firstRowBuffer, sizeof(firstRowBuffer), "L %i %.2f", lValue, lDuty);
  snprintf(secondRowBuffer, sizeof(secondRowBuffer), "R %i %.2f", rValue, rDuty);

  // display.clear();
  display.setCursor(0, 0);
  display.print(firstRowBuffer);

  display.setCursor(0, 1);
  display.print(secondRowBuffer);
}

DCMotor leftMotor = DCMotor(PWM_MOTOR1_PIN, DIR_MOTOR1_PIN, MOTOR_MAX_LOAD_DUTY_CYCLE);
DCMotor rightMotor = DCMotor(PWM_MOTOR2_PIN, DIR_MOTOR2_PIN, MOTOR_MAX_LOAD_DUTY_CYCLE - 7);    // making right motor a tiny bit slower. actual max duty is 238. 

PushButton upButton = PushButton(UP_BUTTON_PIN);
PushButton downButton = PushButton(DOWN_BUTTON_PIN);

PushButton modeButton = PushButton(MODE_BUTTON_PIN);

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

void rightHallCallback() {
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

  modeButton.init(25);  // debounce delay
  led.init();

  // Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.noCursor();

  imu.init(false);

  pinMode(HALL_1_PIN, INPUT_PULLUP);
  pinMode(HALL_2_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(HALL_1_PIN), leftHallCallback, RISING);
  attachInterrupt(digitalPinToInterrupt(HALL_2_PIN), rightHallCallback, RISING);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= CONTROLL_INTERVAL && currentMillis > previousMillis) {
    previousMillis = currentMillis;
    modeButton.update();
    upButton.update();
    downButton.update();
    
    if (tablePosition.mode == AUTO) {
      if (modeButton.isPressed()) {
        tablePosition.mode = MANUAL;  // entering manual mode here;
        led.on();
      } else {
        if (upButton.isPressed()) {
          leftMotor.up();
          rightMotor.up();
        } else if (downButton.isPressed()) {
          leftMotor.down();
          rightMotor.down();
        } else {
          leftMotor.stop();
          rightMotor.stop();
        }
      }

    } else if (tablePosition.mode == MANUAL) {
      if (modeButton.isPressed()) {
        tablePosition.mode = AUTO;  // second press of 2 buttons exits manual mode; UP/Down button become Left/Right buttons for moving UP only!
        led.off();
      } else {
        if (upButton.isPressed()) {
          // up button for left motor
          leftMotor.up();
          // manually "leveling" right and left
          tablePosition.leftHallSensorCounter = tablePosition.rightHallSensorCounter;
        } else if (downButton.isPressed()) {
          // down button for right motor;
          rightMotor.up();
          tablePosition.rightHallSensorCounter = tablePosition.leftHallSensorCounter;
        } else {
          leftMotor.stop();
          rightMotor.stop();
        }
      }
    }

    Measurements angles = imu.getData();
    displayPrint(lcd, tablePosition.leftHallSensorCounter, tablePosition.rightHallSensorCounter, angles.x, angles.y);
  }
}
