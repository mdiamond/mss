/*
 * Matthew Diamond 2015
 * Member functions for the Module class.
 */

// Include header file
#include "Module.hpp"

// Included files
// main.hpp must be included here instead of Module.hpp
// because it declares external Module variables.
#include "main.hpp"

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

/*
 * This function recalculates the modules position in the
 * window given only its module number.
 */
void Module::calculate_upper_left(int i)
{
  int num_x, num_y;
  num_x = (i % MODULES_PER_ROW);
  num_y = (i / MODULES_PER_ROW);
  upper_left.x = (num_x *(WINDOW_WIDTH / MODULES_PER_ROW)) +
                 (num_x * MODULE_SPACING);
  upper_left.y = (num_y *(WINDOW_WIDTH / MODULES_PER_ROW)) +
                 (num_y * MODULE_SPACING);

}

/*
 * This function renders a border for the module
 */
void Module::render_border()
{
  border = {upper_left.x, upper_left.y, MODULE_WIDTH, MODULE_HEIGHT};
  SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255);
  SDL_RenderFillRect(RENDERER, &border);
}

/*
 * This function renders an inner border for the module
 * using the color defined within the module.
 */
void Module::render_inner_border()
{
  inner_border = {upper_left.x + MODULE_BORDER_WIDTH,
                  upper_left.y + MODULE_BORDER_WIDTH,
                  MODULE_WIDTH - (2 * MODULE_BORDER_WIDTH),
                  MODULE_HEIGHT - (2 * MODULE_BORDER_WIDTH)};
  SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(RENDERER, &inner_border);
}
