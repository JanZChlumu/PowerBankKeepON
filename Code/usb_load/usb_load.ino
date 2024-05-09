#include <EEPROM.h>

#define PIN_BTN PIN_PA6         // learning button
#define PIN_VOLTAGE PIN_PA1     // voltage measurement
#define PIN_FET PIN_PA2         // FET transistor Gate
#define PIN_LED_GREEN PIN_PA7   // learning Green LED
#define MIN_TRIGGER_TIME_MS 20  // minimal time when FET is OPEN

static unsigned int tick = 0;
static bool learning;
unsigned int addr = 0;
unsigned int sleepTime_ms = 0;

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);

  pinMode(PIN_LED_GREEN, OUTPUT);
  digitalWrite(PIN_LED_GREEN, LOW);

  pinMode(PIN_FET, OUTPUT);
  digitalWrite(PIN_FET, LOW);

  // Learning mode activated
  if (digitalRead(PIN_BTN) == LOW) {
    //button pressed start learning
    learning = true;

    // blink with Green learning LED
    for (char i = 0; i < 3; i++) {
      digitalWrite(PIN_LED_GREEN, HIGH);
      delay(100);
      digitalWrite(PIN_LED_GREEN, LOW);
    }
  } else
  // counting mode activated
  {
    learning = false;
    EEPROM.get(addr, tick);

    if (tick < 2) {
      learning = true;

      // probably error blink with both LEDs
      for (char i = 0; i < 3; i++) {
        digitalWrite(PIN_FET, HIGH);
        digitalWrite(PIN_LED_GREEN, HIGH);
        delay(100);
        digitalWrite(PIN_FET, LOW);
        digitalWrite(PIN_LED_GREEN, LOW);
      }
    } else {
      // expected that powerbank died before string the last value
      tick--;
      sleepTime_ms = tick * 1000;
    }
  }

  if (learning == true) {
    // reset tick stored in eeprom
    tick = 0;
    EEPROM.put(addr, tick);
  }
}

void loop() {

  // put your main code here, to run repeatedly:
  if (learning == true) {
    delay(1000);

    tick++;  // increment ticks
    EEPROM.put(addr, tick);
  }
  // trigger the FET every maximum timeout
  else {
    digitalWrite(PIN_FET, HIGH);
    delay(MIN_TRIGGER_TIME_MS);
    digitalWrite(PIN_FET, LOW);

    delay(sleepTime_ms);
  }
}
