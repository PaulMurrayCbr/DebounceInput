/*

Filter Demo 1 - on and off

Demonstration of the use and behaviour of the DebounceFilter
class.

Generally you would not use DebounceFilter directly, instead using
DebouncedInput to "wrap" a digital input pin. However, you can
do so if you wish.

This demo simply feeds 'high' into a debounce filter until it senses it,
and then feeds 'low' into the filter until it changes back.

This sketch uses 'forceSample' rather than 'addSample' to bypass the
sample rate limit.

Hardware setup:
This sketch runs stand-alone, outputting some text to the serial monitor.

Copyright (c) Paul Murray, 2015. Released into the public domain
under the unlicense http://unlicense.org .

*/

#include <DebounceInput.h>

DebounceFilter filter;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // initial delay to allow you to pop up the serial monitor

  Serial.print("\n\nDemonstration starting in:");
  for (int i = 3; i > 0; i--) {
    Serial.print(' ');
    Serial.print(i);
    delay(1000);
  }

  Serial.println("\n\nDebounceFilter demonstration 1 - simple on and off");

  filter.reset(false);
  int ct = 0;
  while (!filter.state()) {
    filter.forceSample(true);
    ct++;
  }
  Serial.print("filter goes high after ");
  Serial.print(ct);
  Serial.println(" samples");

  ct = 0;
  while (filter.state()) {
    filter.forceSample(false);
    ct++;
  }
  Serial.print("filter goes low after ");
  Serial.print(ct);
  Serial.println(" subsequent samples");

  Serial.println("\nDemonstration complete.");

}

void loop() {
  // put your main code here, to run repeatedly:

}
