/**
 * DebounceInput - a library for debouncing digital inputs.
 * See DebounceInput.h for details.
 *
 * Copyright (c) Paul Murray, 2015. Released into the public domain
 * under the unlicense http://unlicense.org .
 */

#include "DebounceInput.h"

#define OUTPUT_MASK 0x0100
#define CHANGE_MASK 0x0001
#define FILTER_1 0x00FE
#define FILTER_2 0xFE00
#define FILTERS 0xFEFE

//! A filter initialsed to the specified initial state
DebounceFilter::DebounceFilter(boolean initialState) {
    reset(initialState);
}


//! push a sample into the filter
void DebounceFilter::addSample(boolean sample) {
    // multiply both filters by .75
    // this blatts the status bits, so we will use a local var

    word mul = ((filter>>1) & 0x7F7F) + ((filter>>2) & 0x3F3F);

    // add the sample to filter1.
    // note that 7f+3f, the maximum, is 65 short of ff. So we treat
    // the signal as being 65.
    
    if(sample) mul += 65;
    
    // add filter1>>2 to filter 2.
    mul += (mul<<6)&0x3f00;
    
    // filter 2 maxes out at 7f+3f+3f. This is 2 short of ff, so we
    // make up the difference by feeding back the previous state
    
    if(filter&OUTPUT_MASK) mul += 0x0200;
    
    // and now for the schmitt trigger.
    // the limits of 60 for the lower and A0 for the upper are experimental
    
    if(filter&OUTPUT_MASK) {
        if((mul >> 8) < fallingThreshhold) {
            filter = CHANGE_MASK;
        }
        else {
            filter = (mul&FILTERS) | OUTPUT_MASK;
        }
    }
    else {
        if((mul >> 8) > risingThreshhold) {
            filter = FILTERS | CHANGE_MASK | OUTPUT_MASK;
        }
        else {
            filter = (mul&FILTERS);
        }
    }
    
}

//! debounced state
boolean DebounceFilter::state() {
    return (filter & OUTPUT_MASK) != 0;

}

//! true if the most recently pushed sample caused the state to change. Time-based sampling is correctly managed.
boolean DebounceFilter::stateChanged() {
    return (filter & CHANGE_MASK) != 0;
}

//! reset to the given state. This will cause stateChanged() to return false irrespective of the prior state .
void DebounceFilter::reset(boolean state) {
    if(state) {
        filter = FILTERS | OUTPUT_MASK;
    }
    else {
        filter = 0;
    }
}

//! push a sample into the filter
void DebounceFilter4ms::addSampleRateLimited(boolean sample) {
    byte now = ((byte)(((word)millis())>>2));
    if(now == mostRecent4ms) {
        // we dont push the sample, but we do
        // treat the state as if we had.
        filter &= ~CHANGE_MASK;
    }
    else {
        addSample(sample);
        mostRecent4ms = now;
    }
}


DebouncedInput::DebouncedInput() {
    this->pin = 255;
}

DebouncedInput::DebouncedInput(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
    reset(digitalRead(pin));
}

void DebouncedInput::attach(int pin) {
    this->pin = pin;
    reset(digitalRead(pin));
}

void DebouncedInput::detach() {
    this->pin = 255;
    reset(false);
}

boolean DebouncedInput::attached() {
    return this->pin == 255;
}

boolean DebouncedInput::read() {
    if(pin == 255) return false;
    addSampleRateLimited(digitalRead(pin));
    return (filter & OUTPUT_MASK) != 0;
}


DebouncedAnalogInput::DebouncedAnalogInput() {
    this->pin = 255;
}

DebouncedAnalogInput::DebouncedAnalogInput(int pin) {
    this->pin = pin;
    reset(analogRead(pin) >= 16);
}

void DebouncedAnalogInput::attach(int pin) {
    this->pin = pin;
    reset(analogRead(pin) >= 16);
}

void DebouncedAnalogInput::detach() {
    this->pin = 255;
    reset(false);
}

boolean DebouncedAnalogInput::attached() {
    return this->pin == 255;
}

boolean DebouncedAnalogInput::read() {
    if(pin == 255) return false;
    addSampleRateLimited(analogRead(pin) >= 16);
    return (filter & OUTPUT_MASK) != 0;
}



boolean AbstractDebouncedInput::read() {
    return true;
}

boolean AbstractDebouncedInput::high() {
    return (filter & OUTPUT_MASK) != 0;
}

boolean AbstractDebouncedInput::low() {
    return (filter & OUTPUT_MASK) == 0;
}

boolean AbstractDebouncedInput::changing() {
    return (filter & CHANGE_MASK) != 0;
}

boolean AbstractDebouncedInput::falling() {
    return (filter & (CHANGE_MASK|OUTPUT_MASK)) == CHANGE_MASK;
}

boolean AbstractDebouncedInput::rising() {
    return (filter & (CHANGE_MASK|OUTPUT_MASK)) == (CHANGE_MASK|OUTPUT_MASK);
}


