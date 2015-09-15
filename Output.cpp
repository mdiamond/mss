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

  this->audio.input_l = new vector<float>(BUFFER_SIZE, 0);
  this->audio.input_r = new vector<float>(BUFFER_SIZE, 0);
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
 * Copy all data from the audio data struct to the
 * graphics data struct to make it available for
 * rendering.
 */
void Output::copy_graphics_data()
{
  this->graphics.input_l = new vector<float>(*(this->audio.input_l));
  this->graphics.input_r = new vector<float>(*(this->audio.input_r));
}

/*
 * This function uses the renderer to create a representation
 * of this module in the window.
 */
void Output::render()
{

}
