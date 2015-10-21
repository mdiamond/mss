/*
 * Matthew Diamond 2015
 * Member functions for the Module class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "function_forwarder.hpp"
#include "image_processing.hpp"
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
 * Constructor.
 */
Module::Module()
{
    // Pick a random background and text color
    color.r = rand() % 128;
    text_color.r = (rand() % 128) + 128;
    color.g = rand() % 128;
    text_color.g = (rand() % 128) + 128;
    color.b = rand() % 128;
    text_color.b = (rand() % 128) + 128;
    color.a = 255;
    text_color.a = 255;

    processed = false;
}

/*
 * Destructor.
 */
Module::~Module()
{
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
        delete graphics_objects[i];
}

/*
 * This function calls upon the modules dependencies
 * to process samples.
 */
void Module::process_dependencies()
{
    for(unsigned int i = 0; i < dependencies.size(); i ++)
        if(dependencies[i] != NULL && !dependencies[i]->processed)
            dependencies[i]->process();
}

void Module::calculate_upper_left()
{
    int x, y;
    x = ((number % (MODULES_PER_ROW * MODULES_PER_COLUMN)) % MODULES_PER_ROW);
    y = ((number % (MODULES_PER_ROW * MODULES_PER_COLUMN)) / MODULES_PER_ROW);
    upper_left.x = ((x * MODULE_WIDTH) + (x * MODULE_SPACING));
    upper_left.y = ((y * MODULE_HEIGHT) + (y * MODULE_SPACING));
}

/*
 * Call upon each module to update its control values.
 */
void Module::update_control_values()
{
    update_unique_control_values();
}

void Module::update_graphics_objects()
{
    update_unique_graphics_objects();
}

/*
 * Calculate the locations of all graphics objects, then
 * call upon the module to caluclate the locations of
 * any graphics objects that are unique to the module type.
 */
void Module::calculate_graphics_objects()
{
    SDL_Rect location;
    string object_name;
    Rect *rect;
    Text *text;

    // Calculate the modules top left pixel location in the window
    calculate_upper_left();

    // If the graphics objects have not yet been initialized
    if(graphics_objects.size() == 0)
    {
        // graphics_object[0] is the outermost rectangle used to represent the module
        location = {upper_left.x, upper_left.y, MODULE_WIDTH, MODULE_HEIGHT};
        rect = new Rect("border (rect)", &location, &WHITE);
        graphics_objects.push_back(rect);

        // graphics_object[1] is the slightly smaller rectangle within the outermost
        // rectangle
        location = {upper_left.x + MODULE_BORDER_WIDTH,
                    upper_left.y + MODULE_BORDER_WIDTH,
                    MODULE_WIDTH - (2 * MODULE_BORDER_WIDTH),
                    MODULE_HEIGHT - (2 * MODULE_BORDER_WIDTH)};
        rect = new Rect("inner_border (rect)", &location, &color);
        graphics_objects.push_back(rect);

        // graphics_object[2] is the objects name
        location = {upper_left.x + MODULE_BORDER_WIDTH + 2,
                    upper_left.y + MODULE_BORDER_WIDTH + 5, 0, 0};
        text = new Text("module name (text)", &location, &text_color, name, FONT_BOLD);
        graphics_objects.push_back(text);
    }

    // If they have already been initialized, just update their locations
    else
    {
        location = {upper_left.x, upper_left.y, MODULE_WIDTH, MODULE_HEIGHT};
        graphics_objects[MODULE_BORDER_RECT]->update_location(&location);

        location = {upper_left.x + MODULE_BORDER_WIDTH,
                    upper_left.y + MODULE_BORDER_WIDTH,
                    MODULE_WIDTH - (2 * MODULE_BORDER_WIDTH),
                    MODULE_HEIGHT - (2 * MODULE_BORDER_WIDTH)};
        graphics_objects[MODULE_INNER_BORDER_RECT]->update_location(&location);

        location = {upper_left.x + MODULE_BORDER_WIDTH + 2,
            upper_left.y + MODULE_BORDER_WIDTH + 5, 0, 0};
        graphics_objects[MODULE_NAME_TEXT]->update_location(&location);
    }

    calculate_unique_graphics_objects();
}

void Module::set(float val, int input_num)
{
    inputs[input_num] = NULL;
    dependencies[input_num] = NULL;
    input_floats[input_num] = val;
    inputs_live[input_num] = false;
}

void Module::set(Module *src, int input_num)
{
    inputs[input_num] = &src->output;
    dependencies[input_num] = src;
    inputs_live[input_num] = true;
    SELECTING_SRC = false;
    reset_alphas();
}

void Module::cancel_input(int input_num)
{
    inputs[input_num] = NULL;
    dependencies[input_num] = NULL;
    inputs_live[input_num] = false;
}
