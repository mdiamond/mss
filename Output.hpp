/*
 * Matthew Diamond 2015
 * The output module. All audio requested by the callback
 * function comes through here. This module simply
 * processes the modules it depends upon, then makes available
 * full audio buffers to the audio callback function. This file
 * defines the class and includes any files or libraries necessary
 * for Output.cpp.
 */

#ifndef synth_output_h
#define synth_output_h

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included classes
#include "Module.hpp"

/***************************
 * OUTPUT CLASS DEFINITION *
 ***************************/

class Output: public Module
{
  public:
    // Module information
    std::string name;
    int type;
    // Input buffers
    std::vector<float> *input_l;
    std::vector<float> *input_r;
    // Constructor and destructor
    Output(void);
    virtual ~Output(void);
    // Member functions
    virtual void process();
    virtual void render();
};

#endif
