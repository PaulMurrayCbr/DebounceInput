
/*

DebouncedInput - nfo to serial

Demonstration of the use and behaviour of the DebouncedInput
class.

This demo monitors the INPUT_PIN and sends information to serial from
the methods available on DebouncedInput.

This sketch may fail to detect fast double-clicks due to the time it
takes to send the diagnostic mesages to the serial at 9600.

Hardware setup:
You will need a switch or button on INPUT_PIN (pin 4), the other side
of the switch grounded.

You will to monitor the serial output (the IDE works fine).

*/


#include <DebounceInput.h>

#define INPUT_PIN 4

DebouncedInput pin(INPUT_PIN);

unsigned long second;

void setup() {
  /*
   Note that we do not set up the input pin. The debouncedInput
   class does this for us, assuming INPUT_PULLUP (the ususal case)
   */

  Serial.begin(9600);
}

void loop() {
  // this line performns the sampleing of the pin. We need to do this once
  // to make the detection of rising and falling states work correctly
  
  pin.read();
  
  // we will print off the state every second
  if(millis() - second >= 1000) {
    Serial.print("pin.high(): ");
    Serial.print(pin.high());
    Serial.print(", pin.low(): ");
    Serial.print(pin.low());
    if(pin.low()) {
      Serial.println(", Button is DOWN");
    }
    else {
      Serial.println(", Button is UP");
    }
    second = millis();
  }
  
  if(pin.changing()) {
      Serial.print("Button changing");
  } 
  if(pin.rising()) {
      Serial.println(", button being RELEASED");
  } 
  if(pin.falling()) {
      Serial.println(", button being PRESSED");
  } 
}
