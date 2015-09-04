/*
 * ButtonDebounce - a library for debouncing digital inputs.
 */

//! Debounce algorithm implementation.
/*!
    An implementation of 
    cactusjack's [Elegant debouncing solution with software Schmitt trigger emulation](http://forum.arduino.cc/index.php?topic=125297),
    with a few tweaks. This class is purely about the algorithm.
 
    The tweaks to cactusjack's algorithm are:
    - I use two sucessive 7-bit filters. This seems to work better than one alone. It creates a 
      delay in sensing a state change (the inevitable tradeoff of smoothing the input), but for 
      manually operated buttons the delay is not excessive.
    - When the output state changes, I saturate the filters to the new value. This,
      hopefully, provides better supression of bouncing in particular
    - I feed back the previous state into the second filter. The real reason for this
      is an artifact of the underlying numbers: (0xff>>1)+(0xff>>2)+(0xff>>2) is 0xFD, not 0xFF.
    - I use two of the bits in the 16-bit word to store state. This means that sizeof(DebounceFilter)
      is just the two bytes for the filter.
    - cactusjack's time-based sampling is not done here. It it moved into DebouncedButton, and 
      does not use delay()
 
    In normal use, you pass it a series of samples
    via the addSample method, and read the debounced result with state(). A convenience method stateChanged()
    indicates that the most recent sample caused the state to change.
 */

class DebounceFilter {
    friend class DebouncedButton;
private:
    word filter;
public:
    //! A filter initialised to false
    DebounceFilter();
    //! A filter initialsed to the specified initial state
    DebounceFilter(boolean initialState);
    //! push a sample into the filter
    void addSample(boolean sample);
    //! debounced state
    boolean state();
    //! true if the most recently pushed sample caused the state to change
    boolean stateChanged();
    //! reset to the given state. This will cause stateChanged() to return false irrespective of the prior state .
    void reset(boolean state);
};

//! A debounced digital input.
/*!
    This class builds a "button" model on a digital input pin, for use with manual inputs.
    It initialises the pin to INPUT_PULLUP, and treats LOW as "button pressed" and
    HIGH as "button not pressed".
 
    Samples are rate limited to no more than once per 4ms in order to provide consistent filter behaviour. This class is intended for use
    with manually operated buttons and switches, and may be unsuitable for faster inputs.
 
 */

class DebouncedButton {
private:
    int pin;
    DebounceFilter filter;
    // holds (byte)(millis() >> 2), rounding off millis to 4ms. (0xFF<<2) ms is about 1 second - any sketch should be sampling its buttons fatser than that.
    byte mostRecentSample;
public:
    //! Set up pin as INPUT_PULLUP, and use it as a source of input
    DebouncedButton(int pin);
    //! read the pin, true if the debounced signal is pressed (LOW)
    /*!
        This method will only feed samples into the filter at a rate of about once per
        4ms (provided it is called at short enough intervals). However, the state change functions
        act as if it had. This means that even if the pin is not read, if read() is called twice in sucession
        and the pin has not changed, then changed(), isPress(), and isRelease() will return false after the second call.
     */
    boolean read();
    //! true if the debounced signal is pressed (LOW)
    boolean pressed();
    //! true if the debounced signal changed state after the most recent read()
    boolean changed();
    //! true if the debounced signal went from released (HIGH) to pressed (LOW) after the most recent read()
    boolean isPress();
    //! true if the debounced signal went from pressed (LOW) to released (HIGH) after the most recent read()
    boolean isRelease();
    
    //! read the pin, true if the debounced signal changed state
    inline boolean readChanged() { read(); return isChanged();}
    //! read the pin, true if the debounced signal went from released (HIGH) to pressed (LOW)
    inline boolean readPress() { read(); return isPress();}
    //! read the pin, true if the debounced signal went from pressed (LOW) to released (HIGH)
    inline boolean readRelease() { read(); return isRelease();}
    
};