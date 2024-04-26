#include <EEPROM.h>

#define PIN_BTN PIN_PA6
#define PIN_VOLTAGE PIN_PA1
#define PIN_STIMUL PA2

static unsigned int tick = 0;
static bool store;
unsigned int addr = 0;

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);

  if(digitalRead(PIN_BTN) == LOW)
  {
    //button pressed start counting
    store = true;
    EEPROM.put(addr, tick);
  }
  else
  {
    store = false;
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  EEPROM.write(addr, tick);
  delay(1000);
}
