/*
 * Matthew Diamond 2015
 * Member functions for the Module class.
 */

// Included libraries
#include <cmath>
#include <iostream>
#include <string>

// Included SDL components
#include "SDL2/SDL.h"

// Included classes
#include "Module.hpp"

/*
 * Dummy function
 */
Module::Module(void)
{

}

/*
 * Dummy function
 */
Module::~Module(void)
{

}

/*
 * This function calls upon the modules dependencies
 * to process samples.
 */
void Module::process_depends()
{
  for(int i = 0; i < depends.size(); i ++)
  {
    depends[i]->process();
  }
}