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
  render_border();
  SDL_Rect inner = {upper_left.x + MODULE_BORDER_WIDTH,
                    upper_left.y + MODULE_BORDER_WIDTH,
                    MODULE_WIDTH - (2 * MODULE_BORDER_WIDTH),
                    MODULE_HEIGHT - (2 * MODULE_BORDER_WIDTH)};
  SDL_SetRenderDrawColor(RENDERER, 255, 0, 0, 255);
  SDL_RenderFillRect(RENDERER, &inner);
}
