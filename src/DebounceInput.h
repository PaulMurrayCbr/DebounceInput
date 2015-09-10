/*
 * DebounceInput - a library for debouncing digital inputs.
 *
 * Copyright (c) Paul Murray, 2015. Released into the public domain
 * under the unlicense http://unlicense.org .
 */

#ifndef DebounceInput_h
#define DebounceInput_h

#include <Arduino.h>

//! Debounce algorithm implementation.
/*!
    An implementation of cactusjack's
    [Elegant debouncing solution with software Schmitt trigger emulation](http://forum.arduino.cc/index.php?topic=125297),
    with a few tweaks. 
 
    The tweaks to cactusjack's algorithm are:
    - I use two sucessive 7-bit filters. This seems to work better than one alone. It creates a 
      delay in sensing a state change (the inevitable tradeoff of smoothing the input), but for 
      manually operated buttons the delay is not excessive.
    - When the output state changes, I saturate the filters to the new value. This,
      hopefully, provides better supression of bouncing in particular
    - I feed back the previous state into the second filter. The real reason for this
      is an artifact of the underlying numbers: (0xff>>1)+(0xff>>2)+(0xff>>2) is 0xFD, not 0xFF.
    - I use two of the bits in the 16-bit word to store state. This means that sizeof(DebounceFilter)
      is just the two bytes for the filter. and one byte to manage the time-based sampling.
    - The rate limiting sample is farmed out to a subclass
 
    In normal use, you pass a DebounceFilter a series of samples
    via the addSample method, and read the debounced result with state(). convenience method stateChanged()
    indicates that the most recent sample caused the state to change.
 */

class DebounceFilter {
protected:
    /*
        holds 2 7-bit values, being the two "filters", and 2 state bits. The state bits hold
        the current output state (which is used to drive the schmitt trigger) and whether or not
        the state changed last sample (which is used to detect up/down events 
     */
    word filter;
    
public:
    byte risingThreshhold = 0x90;
    byte fallingThreshhold = 0x70;
    
    //! A filter initialised to false
    DebounceFilter() : DebounceFilter(false) {}
    //! A filter initialsed to the specified initial state
    DebounceFilter(boolean initialState);
    
    //! push a sample into the filter
    void addSample(boolean sample);
    //! debounced state
    boolean state();
    //! true if the most recently pushed sample caused the state to change. Time-based sampling is correctly managed.
    boolean stateChanged();
    //! reset to the given state. This will cause stateChanged() to return false irrespective of the prior state .
    void reset(boolean state);
};


class DebounceFilter4ms : public DebounceFilter {
protected:
    /* holds (byte)(millis() >> 2), rounding off millis to 4ms.
     (0xFF<<2) ms is about 1 second - any sketch should be sampling its buttons faster than that.
     */
    byte mostRecent4ms;
public:
    //! A filter initialised to false
    DebounceFilter4ms() : DebounceFilter(false) {}
    //! A filter initialsed to the specified initial state
    DebounceFilter4ms(boolean initialState): DebounceFilter(initialState) {}
    
    //! push a sample into the filter, samples are rate limited at one per 4ms
    void addSampleRateLimited(boolean sample);

};

//! A debounced digital input.
/*!
    This class builds a model on a digital input pin. The constructor initialises the pin to INPUT_PULLUP and reads its initial state.
 */

class DebouncedInput : protected DebounceFilter4ms {
protected:
    int pin;
public:
    using DebounceFilter::risingThreshhold;
    using DebounceFilter::fallingThreshhold;
    
    //! A DebouncedInput unconnected to a pin
    DebouncedInput();
    
    //! Set up pin as INPUT_PULLUP, and use it as a source of input
    DebouncedInput(int pin);
    
    //! attach the DebouncedInput to a pin
    void attach(int pin);
    //! detach the DebouncedInput from its assigned pin
    void detach();
    //! is the input currently attached to a pin?
    boolean attached();
    
    //! read the pin, true if the debounced signal is HIGH
    boolean read();
    //! true if the debounced signal is HIGH
    boolean high();
    //! true if the debounced signal is LOW. This is a a convenience method for pushbuttons.
    boolean low();
    //! true if the debounced signal changed state after the most recent read()
    boolean changing();
    //! true if the debounced signal went from HIGH to LOW after the most recent read()
    boolean falling();
    //! true if the debounced signal went from LOW to HIGH after the most recent read()
    boolean rising();
    
    //! read the pin, true if the debounced signal changed state
    inline boolean readChanging() { read(); return changing();}
    //! read the pin, true if the debounced signal went from HIGH to LOW
    inline boolean readFalling() { read(); return falling();}
    //! read the pin, true if the debounced signal went from LOW to HIGH
    inline boolean readRising() { read(); return rising();}
};

#endif
