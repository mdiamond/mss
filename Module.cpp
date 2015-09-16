/*
 * Matthew Diamond 2015
 * Member functions for the Module class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "main.hpp"

// Included classes
#include "Module.hpp"

/***************************
 * MODULE MEMBER FUNCTIONS *
 ***************************/

/*
 * Dummy function.
 */
Module::Module()
{

}

/*
 * Dummy function.
 */
Module::~Module()
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
 * This function renders a border for the module.
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

void Module::render_name(SDL_Color *color)
{
    SDL_Surface *surface = TTF_RenderText_Blended(FONT_BOLD, name.c_str(), *color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(RENDERER, surface);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    int x = upper_left.x + 5;
    int y = upper_left.y + 5;
    SDL_Rect dstrect = {x, y, width, height};
    SDL_RenderCopy(RENDERER, texture, NULL, &dstrect);
}
