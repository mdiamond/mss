/*
 * Matthew Diamond 2015
 * Member functions for the Module class.
 */

// Include header file
#include "Module.hpp"

/***************************
 * MODULE MEMBER FUNCTIONS *
 ***************************/

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
  for(unsigned int i = 0; i < depends.size(); i ++)
  {
    depends[i]->process();
  }
}
