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

// Helper functions
Module *find_module(std::string *, std::vector<Module *> *);
Module *find_module_as_source(std::string *, std::vector<Module *> *, Module *);
bool can_floatify(std::string *);

// Module initialization function
void create_module(int);

// Page switching function
void increment_page_number(int);

// Function forwarder
void function_forwarder(Graphics_Object *);

#endif
