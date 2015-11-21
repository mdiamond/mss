/*
 * Matthew Diamond 2015
 * Header file for the function forwarder.
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
Module *find_module_as_source(std::string *, std::vector<Module *> *, Module *);

bool can_floatify(std::string *);

void add_oscillator();
void add_multiplier();
void add_mixer();
void next_page();
void previous_page();

void function_forwarder(Graphics_Object *);

#endif
