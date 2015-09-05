/*

Filter Demo 2 - noisy data

Demonstration of the use and behaviour of the DebounceFilter
class.

Generally you would not use DebounceFilter directly, instead using
DebouncedInput to "wrap" a digital input pin. However, you can
do so if you wish.

This demo feeds three cycles of 20 high and 20 low into a
debounce filter. However, a random 1 in 4 samples are
reversed.

Output shows the "real" base sample, the noisy sample, and
the behaviour of the filter. The filter correctly detects
state changes in the base sample, with a lag of about 10
samples (usually).

This sketch uses 'forceSample' rather than 'addSample' to bypass the
sample rate limit.

Hardware setup:
This sketch runs stand-alone, outputting some text to the serial monitor.

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

  Serial.println("\n\nDebounceFilter demonstration 2 - noisy signal");

  filter.reset(false);

  for (int i = 0; i < 20 * 2 * 3; i++)
  {
    boolean baseSignal = (i % 40) < 20;
    boolean signal = baseSignal;
    if (random(4) == 0) signal = !baseSignal;
    filter.forceSample(signal);

    if (i < 10) Serial.print(' ');
    if (i < 100) Serial.print(' ');
    Serial.print(i);
    Serial.print(": base signal ");
    Serial.print(baseSignal);
    Serial.print(", noisy signal ");
    Serial.print(signal);
    Serial.print(", state  ");
    Serial.print(filter.state());
    Serial.print(", change ");
    Serial.print(filter.stateChanged());
    if (baseSignal != filter.state()) {
      Serial.print(" LAG...");
    }
    if (filter.stateChanged()) {
      Serial.print(", state change detected after ");
      Serial.print(i - ((i / 20) * 20));
      Serial.print(" samples");
    }
    Serial.println();


  }
  Serial.println("\nDemonstration complete.");

}

void loop() {
  // put your main code here, to run repeatedly:

}
