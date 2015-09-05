# DebounceInput
This is a library for the Arduino IDE for debouncing digital inputs (buttons, switches).

The debounce algorithm is based on cactusjack's [Elegant debouncing solution with software Schmitt trigger emulation](http://forum.arduino.cc/index.php?topic=125297).

## Installation

To download. click the DOWNLOAD ZIP button, rename the uncompressed folder DebounceInput. 

Place the DebounceInput library folder your `arduinosketchfolder/libraries/` folder (create the folder if necessary).

Alternatively, you can use git. Navigate to your `arduinosketchfolder/libraries/` folder (create the folder if necessary), and clone the repository there.

	git clone https://github.com/PaulMurrayCbr/DebounceInput.git

After restarting the IDE, navigate to `Sketch > Include Library` in the IDE menu. "Debounce Input" should appear at the bottom, under "contributed ibraries".

Example sketches using the library appear under `File > Examples > Debounce Input`

## Using

To use DebouncedInput, create an instance on an input pin, eg

    DebouncedInput button(4);

Then in your loop, make a call to `read()` to read the pin. This should be done frequently. The object will sample the input no more than once every 4ms, so provided you are calling read() more frequently than that, it should debounce correctly.

    void loop() {
        button.read();
        // rest of your sketch goes here
    }
    
You can look at the debounced state of the pin like so:

	if(button.high()) { }

Or any of these methods:

- `high()`
- `low()`
- `changing()`
- `rising()`
- `falling()`

For convenience, these method combine a read() with a check for a change of state.

- `readChanging()`
- `readRising()`
- `readFalling()`

the `read()` method itself also returns the state of the pin, same as `high()`.


The changing, rising, and falling methods return true if the debounced pin state changed as a result of the most resent read. So do not do this:

    // Bad! Don't do this!
    
    if(button.readRising()) {
    }
    else if(button.readFalling()) {
    }

If you need to detect either case, then use something like this:

    button.read();
    
    if(button.rising()) {
    }
    else if(button.falling()) {
    }

For more detailed information, see the `src/DebounceInput.h` file.

