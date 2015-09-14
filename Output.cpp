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
  color.r = 255;
  color.g = 0;
  color.b = 0;
  color.a = 255;
  input_l = NULL;
  input_r = NULL;
}

/*
 * Dummy function
 */
Output::~Output(void)
{
  input_l = new vector<float>(BUFFER_SIZE, 0);
  input_r = new vector<float>(BUFFER_SIZE, 0);
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
