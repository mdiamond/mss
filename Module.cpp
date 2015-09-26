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
    // Pick a random background and text color
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
 * This function calculates the module's outer border
 */
Graphics_Object *Module::calculate_border()
{
    SDL_Rect _border = {upper_left.x, upper_left.y, MODULE_WIDTH, MODULE_HEIGHT};
    string object_name = "border (rect)";
    Rect *border = new Rect(&object_name, &_border, &WHITE);
    return border;
}

/*
 * This function calculates the location of the module's
 * inner border.
 */
Graphics_Object *Module::calculate_inner_border()
{
    SDL_Rect _inner_border = {upper_left.x + MODULE_BORDER_WIDTH,
                              upper_left.y + MODULE_BORDER_WIDTH,
                              MODULE_WIDTH - (2 * MODULE_BORDER_WIDTH),
                              MODULE_HEIGHT - (2 * MODULE_BORDER_WIDTH)};
    string object_name = "inner_border (rect)";
    Rect *inner_border = new Rect(&object_name, &_inner_border, &color);
    return inner_border;
}

/*
 * This function calculates the location of the module's name.
 */
Graphics_Object *Module::calculate_name()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 2;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 2;
    SDL_Rect location = {x, y, 0, 0};
    string object_name = "module name (text)";
    Text *module_name = new Text(&object_name, &location, &text_color, NULL, &name, FONT_BOLD);
    return module_name;
}

/*
 * calculate the locations of all graphics objects, then
 * call upon the module to caluclate the locations of
 * any graphics objects that are unique to the module type.
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
