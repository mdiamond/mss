/*
 * Matthew Diamond 2015
 * The oscillator module. This module is capable of
 * generating a sine, square, triangle, and sawtooth
 * wave given a frequency and pulse width. It can then
 * be modulated with some other input wave or by an internal
 * oscillator. The resulting waveform can then be output.
 */

#ifndef synth_oscillator_h
#define synth_oscillator_h

// Included libraries
#include <string>
#include <vector>

// Included files
#include "main.hpp"

// Included classes
#include "Module.hpp"

class Oscillator: public Module
{
  public:
    std::string name;
    int type;
    float frequency;
    float phase;
    float amplitude;
    int fm_on;
    float index;
    Oscillator *modulator;
    std::vector<float> *output;
    Oscillator(std::string *);
    virtual void process();
    virtual ~Oscillator(void);
};

#endif