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
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Text.hpp"

using namespace std;

/***************************
 * MODULE MEMBER FUNCTIONS *
 ***************************/

/*
 * Dummy function.
 */
Module::Module()
{
    color.r = rand() % 256;
    text_color.r = 256 - color.r;
    color.g = rand() % 256;
    text_color.g = 256 - color.g;
    color.b = rand() % 256;
    text_color.b = 256 - color.b;
    color.a = 255;
    text_color.a = 255;
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
 * This function renders a border for the module.
 */
Graphics_Object *Module::calculate_border()
{
    SDL_Rect _border = {upper_left.x, upper_left.y, MODULE_WIDTH, MODULE_HEIGHT};
    string object_name = "border";
    Rect *border = new Rect(&object_name, &_border, &WHITE);
    return border;
}

/*
 * This function renders an inner border for the module
 * using the color defined within the module.
 */
Graphics_Object *Module::calculate_inner_border()
{
    SDL_Rect _inner_border = {upper_left.x + MODULE_BORDER_WIDTH,
                              upper_left.y + MODULE_BORDER_WIDTH,
                              MODULE_WIDTH - (2 * MODULE_BORDER_WIDTH),
                              MODULE_HEIGHT - (2 * MODULE_BORDER_WIDTH)};
    string object_name = "inner_border";
    Rect *inner_border = new Rect(&object_name, &_inner_border, &color);
    return inner_border;
}

Graphics_Object *Module::calculate_name()
{
    SDL_Surface *surface = TTF_RenderText_Blended(FONT_BOLD, name.c_str(), text_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(RENDERER, surface);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    int x = upper_left.x + 5;
    int y = upper_left.y + 5;
    SDL_Rect dstrect = {x, y, width, height};
    SDL_RenderCopy(RENDERER, texture, NULL, &dstrect);
    string object_name = "module name";
    Text *module_name = new Text(&object_name, &dstrect, &text_color, texture);
    return module_name;
}

/*
 * This function recalculates the modules position in the
 * window given only its module number.
 */
void Module::calculate_graphics_objects()
{
    graphics_objects.clear();
    int x, y;
    x = (number % MODULES_PER_ROW);
    y = (number / MODULES_PER_ROW);
    upper_left.x = (x *(WINDOW_WIDTH / MODULES_PER_ROW)) +
                   (x * MODULE_SPACING);
    upper_left.y = (y *(WINDOW_WIDTH / MODULES_PER_ROW)) +
                   (y * MODULE_SPACING);
    graphics_objects.push_back(calculate_border());
    graphics_objects.push_back(calculate_inner_border());
    graphics_objects.push_back(calculate_name());
    calculate_unique_graphics_objects();
}

void Module::update_graphics_objects()
{
    update_unique_graphics_objects();
}

/*
 * This function calls render on all graphics objects associated with
 * this module.
 */
void Module::render_module()
{
    for(unsigned int i = 0; i < this->graphics_objects.size(); i ++)
    {
        (this->graphics_objects)[i]->render_graphics_object();
    }
}
