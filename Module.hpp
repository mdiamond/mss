/*
 * Matthew Diamond 2015
 * The header for a base class to represent a generic synthesizer module.
 * Other classes will extend this class and represent different
 * types of modules. The following classes are derived from the Module class:
 *   - Output
 *   - Oscillator
 */

#ifndef synth_module_h
#define synth_module_h

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

/*************************
 * TYPES OF MODULES ENUM *
 *************************/

enum Modules
{
  OUTPUT,
  OSCILLATOR,
};

/***************************
 * MODULE CLASS DEFINITION *
 ***************************/

class Module
{
  public:
    std::string name;
    int type;
    std::vector<Module *> depends;
    Module(void);
    virtual ~Module(void);
    virtual void process() = 0;
    void process_depends();
};

#endif