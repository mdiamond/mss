/*
 * Matthew Diamond 2015
 * Member functions for the Output class.
 */

// Include header file
#include "Output.hpp"

using namespace std;

/***************************
 * OUTPUT MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor
 */
Output::Output(void)
{
  name = "output";
  type = OUTPUT;
  input_l = NULL;
  input_r = NULL;
}

/*
 * Dummy function
 */
Output::~Output(void)
{

}

/*
 * This function simply calls upon dependencies for
 * processing. The output module depends on all other
 * modules.
 */
void Output::process()
{
  process_depends();
}

/*
 * This function uses the renderer to create a representation
 * of this module in the window.
 */
void Output::render()
{

}