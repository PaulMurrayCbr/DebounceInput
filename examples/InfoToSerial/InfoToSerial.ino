
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

Copyright (c) Paul Murray, 2015. Released into the public domain
under the unlicense http://unlicense.org .
*/


#include <DebounceInput.h>

#define INPUT_PIN 4

DebouncedInput pin;

unsigned long second;

void setup() {
  Serial.begin(9600);
  while(!Serial) {
    ; // wait for the serial to be ready
  }
  
  
  pinMode(INPUT_PIN, INPUT_PULLUP);
  pin.attach(INPUT_PIN);
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
