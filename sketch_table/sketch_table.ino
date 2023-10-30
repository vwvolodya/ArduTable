#include "LedIndicator.h"
#include "Storage.h"

#define LED_1_PIN 13

int I = 0;
LedIndicator led1(LED_1_PIN);
Storage storage(1); //address 1

void setup() {
  led1.init();
}

void loop() {
  int number = storage.get();
  led1.blinkN(2, 100 + (I*10));
  I+=1;
  storage.set(I);
}
