/*
 * Matthew Diamond 2015
 * Header file for the utils functions. This file 
 * declares any functions.
 */

#ifndef synth_function_forwarder_h
#define synth_function_forwarder_h

/************
 * INCLUDES *
 ************/

// Included classes
#include "Graphics_Object.hpp"
#include "Module.hpp"

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

Module *find_module(std::string *, std::vector<Module *> *);

bool can_floatify(std::string *);

void function_forwarder(Graphics_Object *);

#endif
