/*
 * Matthew Diamond 2015
 * The header for a base class to represent a generic synthesizer module.
 * Other classes will extend this class and represent different
 * types of modules. See Module.cpp for implementations of different
 * modules.
 */

#ifndef synth_module_h
#define synth_module_h

// Included libraries
#include <cmath>
#include <iostream>
#include <queue>
#include <string>

// Included SDL components
#include "SDL2/SDL.h"

enum Modules
{
  OUTPUT,
  OSCILLATOR,
};

class Module
{
  public:
    std::string name;
    int type;
    float current_amplitude;
    Module(void);
    virtual ~Module(void);
    virtual void process(int) = 0;
};

#endif
