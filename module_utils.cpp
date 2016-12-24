/*
 * Matthew Diamond 2015
 * Useful functions related to modules.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL.h"
#include "SDL_ttf.h"

// Included files
#include "main.hpp"
#include "module_utils.hpp"

// Included modules classes
#include "Module.hpp"

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * Create a module of the type specified.
 */
void create_module(int type)
{
    Module *module;

    switch(type)
    {
    case Module::ADSR:
        module = new Adsr();
        break;
    case Module::DELAY:
        module = new Delay();
        break;
    case Module::FILTER:
        module = new Filter();
        break;
    case Module::MIXER:
        module = new Mixer();
        break;
    case Module::MULTIPLIER:
        module = new Multiplier();
        break;
    case Module::NOISE:
        module = new Noise();
        break;
    case Module::OSCILLATOR:
        module = new Oscillator();
        break;
    case Module::SAH:
        module = new Sah();
        break;
    }

    module->initialize_graphics_objects();

    bool push_on_back = false;
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] == NULL)
        {
            MODULES[i] = module;
            push_on_back = false;
            break;
        }

        if(i == MODULES.size() - 1)
        {
            push_on_back = true;
        }
    }

    if(push_on_back)
    {
        MODULES.push_back(module);
    }

    MODULES_CHANGED = true;
}

/*
 * Given the name of a module, return a pointer to it if
 * it exists, or nullptr if it doesn't.
 */
Module *find_module(std::string *name)
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != nullptr)
        {
            if(MODULES[i]->get_name() == *name
               || MODULES[i]->get_short_name() == *name)
            {
                return MODULES[i];
            }
        }
    }

    return nullptr;
}

/*
 * This function takes a module type as input and returns
 * the first number that is not already contained in the name of
 * other modules of this type. For example, if there exist filters 1-6
 * and a filter 8, this function should return 7 if passed FILTER as
 * its argument.
 */
int find_available_module_number(int module_type)
{
    unsigned int current_num;
    std::vector<bool> nums;

    for(unsigned int i = 1; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != NULL && MODULES[i]->module_type == module_type)
        {
            current_num = stoi(
                MODULES[i]->name.substr(MODULES[i]->name.find(" ")).substr(1));

            while(nums.size() < current_num)
            {
                nums.push_back(false);
            }

            nums[current_num - 1] = true;
        }
    }

    for(unsigned int i = 0; i < nums.size(); i ++)
    {
        if(!nums[i])
        {
            return i + 1;
        }
    }

    return nums.size() + 1;
}

/*
 * Find and return the index of an empty spot in the vector of pointers to
 * modules.
 */
int find_available_module_slot()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(MODULES[i] == NULL)
        {
            return i;
        }

    return MODULES.size();
}

/*
 * Find and return the SDL_Rect representing the location of a module in the
 * window given its index in the vector of pointers to modules.
 */
SDL_Rect find_module_location(int number)
{
    int x_num, y_num, x, y;
    SDL_Rect location;

    x_num = ((number % MODULES_PER_PAGE) % MODULES_PER_ROW);
    y_num = ((number % MODULES_PER_PAGE) / MODULES_PER_ROW);

    x = ((x_num * MODULE_WIDTH) + (x_num * MODULE_SPACING));
    y = ((y_num * MODULE_HEIGHT) + (y_num * MODULE_SPACING));

    location = {x, y, MODULE_WIDTH, MODULE_HEIGHT};

    return location;
}

/*
 * Generate a random color, then decide whether white or black contrasts more
 * with it. Return the randomly generated color and either white or black in
 * a vector.
 */
std::vector<SDL_Color> generate_module_colors()
{
    SDL_Color color1, color2;

    color1.r = rand() % 256;
    color1.g = rand() % 256;
    color1.b = rand() % 256;
    color1.a = 255;

    double luminosity = (color1.r * .3) + (color1.g * .59) + (color1.b * .11);

    color2 = luminosity > 128 ? BLACK : WHITE;

    std::vector<SDL_Color> colors = {color1, color2};
    return colors;
}

