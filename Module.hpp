/*
 * Matthew Diamond 2015
 * The header for a base class to represent a generic synthesizer module.
 * Other classes will extend this class and represent different
 * types of modules. See Module.cpp for implementations of different
 * modules.
 */

#ifndef module_h
#define module_h

// Included libraries
#include <cmath>
#include <iostream>
#include <queue>

// Included SDL components
#include "SDL2/SDL.h"

class Module
{
  protected:
    float current_amplitude;
  public:
    string name;
    Module(string *);
}

#endif
