
/*

DebouncedInput - simple flash

Demonstration of the use and behaviour of the DebouncedInput
class.

This demo simply reads the INPUT_PIN and flashes the onboard LED (pin 13)
for 250ms whenever it senses a change from high to low.

State changes are counted. You can "double click" your button and see the 
LED flash twice.

Hardware setup:
You will need a switch or button on INPUT_PIN (pin 4), the other side
of the switch grounded.

You will need an LED on pin 13, but most arduino boards have an onboard LED
on the pin already.

Copyright (c) Paul Murray, 2015. Released into the public domain
under the unlicense http://unlicense.org .
*/


#include <DebounceInput.h>

#define INPUT_PIN 4
#define LED_PIN 13

DebouncedInput pin(INPUT_PIN);

int flashes = 0;
unsigned long mostRecentFlash;

void setup() {
  /*
   Note that we do not set up the input pin. The debouncedInput
   class does this for us, assuming INPUT_PULLUP (the ususal case)
   */
  
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // this is all you have to do to sense if the 
  // debounced input has fallen low, the usual 
  // case for a pushbutton
  
  if(pin.readFalling()) {
    flashes += 2; // on and off
  }
  
  // this part of the sketch flases the LED for 250ms and decrements
  // the count. If the count is an odd number after the decrement, then
  // the LED is turned on, otherwise it is turned off. 
  
  if(flashes && millis()-mostRecentFlash >= 250) {
    flashes --;
    digitalWrite(LED_PIN, flashes % 2);
    mostRecentFlash = millis();
  }
}
