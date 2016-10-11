/*
 * Matthew Diamond 2015
 * Header file for useful functions related to modules.
 */

#ifndef SYNTH_MODULE_UTILS_HPP
#define SYNTH_MODULE_UTILS_HPP

/************
 * INCLUDES *
 ************/

// Included files
#include "main.hpp"

// Included modules classes
#include "Module.hpp"
#include "Modules/Adsr.hpp"
#include "Modules/Delay.hpp"
#include "Modules/Filter.hpp"
#include "Modules/Mixer.hpp"
#include "Modules/Multiplier.hpp"
#include "Modules/Noise.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"
#include "Modules/Sah.hpp"

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// Module initialization function
void create_module(int);

// A function for finding a module, and finding a module as a source for an
// input
Module *find_module(std::string *, std::vector<Module *> *);
Module *find_module_as_source(std::string *, std::vector<Module *> *, Module *);

// A function for determining what number should be in a new module's name
int find_available_module_number(int);

// A function for determining where in the MODULES vector to place
// a new module
int find_available_module_slot();

// A function for determining the location of a module based on its number
SDL_Rect find_module_location(int);

// A function for generating colors for a module
std::vector<SDL_Color> generate_module_colors();

#endif

