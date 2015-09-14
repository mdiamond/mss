/*
 * Matthew Diamond 2015
 * The oscillator module. This module is capable of
 * generating a sine, square, triangle, and sawtooth
 * wave given a frequency and pulse width. It can then
 * be modulated with some other oscillator. The
 * resulting waveform can then be processed or output.
 * This module fills its output buffer with the generated signal.
 * This file defines the class and includes any files or
 * libraries necessary for Output.cpp.
 */

#ifndef synth_oscillator_h
#define synth_oscillator_h

/************
 * INCLUDES *
 ************/

// Included libraries
#include <cmath>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "main.hpp"

// Included classes
#include "Module.hpp"

/*******************************
 * OSCILLATOR CLASS DEFINITION *
 *******************************/

class Oscillator: public Module
{
  public:
    // Oscillator settings and frequency modulator/
    // modulation settings
    float frequency;
    float phase;
    float amplitude;
    Oscillator *modulator;
    int fm_on;
    float modulation_index;
    // Output buffer
    std::vector<float> *output;
    // Constructor and destructor
    Oscillator(std::string *);
    virtual ~Oscillator(void);
    // member functions
    virtual void process();
    virtual void render();
};

#endif
