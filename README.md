# DebounceInput
This is a library for the Arduino IDE for debouncing digital inputs (buttons, switches).

Two classes are provided, DebounceFilter and DebouncedInput.

For more information, see `DebounceInput.h`.

**DebounceFilter**

An implementation of cactusjack's [Elegant debouncing solution with software Schmitt trigger emulation](http://forum.arduino.cc/index.php?topic=125297), with a few tweaks.

**DebouncedInput**

A class which is constructed on a digital input pin, and which uses a DebounceFilter to sense the state of the pin. Methods are provided to query the state of the pin and to sense rise and fall events.

## Installation

To download. click the DOWNLOAD ZIP button, rename the uncompressed folder DebounceInput. 

Place the DebounceInput library folder your `arduinosketchfolder/libraries/` folder. 

You may need to create the libraries subfolder if its your first library. Restart the IDE.

### Method 2 - use git

Navigate to your `arduinosketchfolder/libraries/` folder, and clone the repository there.

	git clone https://github.com/PaulMurrayCbr/DebounceInput.git

You may need to create the libraries subfolder if its your first library. Restart the IDE.
