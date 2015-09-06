/*
 * Matthew Diamond 2015
 * A base class to represent a generic synthesizer module.
 * Other classes will extend this class and represent different
 * types of modules.
 */

// Included libraries
#include <cmath>
#include <iostream>
#include <string>

// Included SDL components
#include "SDL2/SDL.h"

// Included classes
#include "Module.hpp"

Module::Module(void)
{
  current_amplitude = 0;
}

Module::~Module(void)
{

}
