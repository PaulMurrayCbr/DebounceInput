# DebounceInput
This is a library for the Arduino IDE for debouncing digital inputs (buttons, switches).

Two classes are provided, DebounceFilter and DebouncedInput.

For more information, see `src/DebounceInput.h`.

*** TODO *** Make a better doc file, put it in extras.

**DebounceFilter**

An implementation of cactusjack's [Elegant debouncing solution with software Schmitt trigger emulation](http://forum.arduino.cc/index.php?topic=125297), with a few tweaks.

**DebouncedInput**

A class which is constructed on a digital input pin, and which uses a DebounceFilter to sense the state of the pin. Methods are provided to query the state of the pin and to sense rise and fall events.

## Installation

To download. click the DOWNLOAD ZIP button, rename the uncompressed folder DebounceInput. 

Place the DebounceInput library folder your `arduinosketchfolder/libraries/` folder (create the folder if necessary).

### Method 2 - use git

Navigate to your `arduinosketchfolder/libraries/` folder (create the folder if necessary), and clone the repository there.

	git clone https://github.com/PaulMurrayCbr/DebounceInput.git

## Using

After restarting the IDE, navigate to `Sketch > Include Library` in the menu. "Debounce Input" should appear at the bottom, under "contributed ibraries".

Example sketches using the library appear under `File > Examples > Debounce Input`

