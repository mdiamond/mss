/*
 * Matthew Diamond 2015
 * Header file for the testing functions. This file includes
 * any files or libraries necessary for tests.cpp and declares any
 * functions.
 */

#ifndef synth_tests_h
#define synth_tests_h

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "signal_processing.hpp"

/*************************
 * FUNCTION DECLARATIONS *
 *************************/

// Run tests
int run_tests(void);

#endif
