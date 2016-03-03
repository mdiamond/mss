/*
 * Matthew Diamond 2015
 * Header file for useful functions related to modules.
 */

#ifndef synth_module_utils_h
#define synth_module_utils_h

/************
 * INCLUDES *
 ************/

// Included files
#include "main.hpp"

// Included classes
#include "Module.hpp"

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// A function for determining what number should be in a new module's name
int find_available_module_number(int);

// A function for determining where in the MODULES vector to place
// a new module
int find_available_module_slot();

#endif
