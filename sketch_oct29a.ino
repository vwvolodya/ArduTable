#define LED_1_PIN 10

class LedIndicator {
private:
  byte pin;
protected:
  void init() {
    pinMode(this->pin, OUTPUT);
    this->off(0);
  }
public:
  LedIndicator(byte pin) {
    this->pin = pin;
    this->init();
  }
  void on(unsigned int duration) {
    digitalWrite(this->pin, HIGH);
    delay(duration);
  }
  void off(unsigned int duration) {
    digitalWrite(this->pin, LOW);
    delay(duration);
  }
  void blinkN(unsigned int n, unsigned int interval) {
    // blink N times with interval between them
    for (unsigned int k = 0; k < n; k += 1) {
      this->on(interval);
      this->off(interval);
    }
  }
};

class PushButton {
private:
  byte pin;
  byte state;
  byte lastReading;

  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;
protected:
  void init() {
    pinMode(this->pin, INPUT);
    this->lastReading = LOW;
    this->update();
  }
public:
  PushButton(byte pin) {
    this->pin = pin;
    this->init();
  }

  bool isPressed() {
    return (this->getState() == HIGH);
  }

  byte getState() {
    this->update();
    return this->state;
  }
  void update() {
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
};

LedIndicator led1(LED_1_PIN);

void setup() {
}

void loop() {
  led1.on(500);
  led1.off(200);
}
