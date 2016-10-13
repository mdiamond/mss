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
 * it exists, or NULL if it doesn't.
 */
Module *find_module(std::string *name, std::vector<Module *> *modules)
{
    for(unsigned int i = 0; i < modules->size(); i ++)
    {
        if(MODULES[i] != NULL)
            if(modules->at(i)->get_name() == *name
               || modules->at(i)->get_short_name() == *name)
            {
                return modules->at(i);
            }
    }

    return NULL;
}

/*
 * Given the name of a module, return a pointer to it if it exists,
 * is not the same module as the destination module, and is not the
 * output module. Otherwise, return NULL and print an error message.
 */
Module *find_module_as_source(std::string *name, std::vector<Module *> *modules,
                              Module *dst)
{
    Module *src = find_module(name, &MODULES);
    if(src == NULL)
    {
        std::cout << RED_STDOUT << "Input could not be set, no such module"
                  << DEFAULT_STDOUT << std::endl;
    }
    else if(src == MODULES[0])
    {
        std::cout << RED_STDOUT
                  << "The output module does not output any signals "
                  "accessible within the context of this software"
                  << DEFAULT_STDOUT << std::endl;
        return NULL;
    }
    else if(src == dst)
    {
        std::cout << RED_STDOUT << "No module may output to itself"
                  << DEFAULT_STDOUT << std::endl;
        return NULL;
    }

    return src;
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

int find_available_module_slot()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(MODULES[i] == NULL)
        {
            return i;
        }

    return MODULES.size();
}

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

std::vector<float> rgb_to_yuv(std::vector<float> rgb)
{
    float y, u, v;

    y = (.299 * rgb[0]) + (.587 * rgb[1]) + (.114 * rgb[2]);
    u = .492 * (rgb[2] - y);
    v = .877 * (rgb[0] - y);

    std::vector<float> yuv = {y, u, v};

    return yuv;
}

std::vector<SDL_Color> generate_module_colors()
{
    SDL_Color color;
    SDL_Color text_color;
    float euclidian_distance;
    std::vector<float> yuv, text_yuv, rgb, text_rgb;

    rgb = std::vector<float>(3, 0);
    text_rgb = std::vector<float>(3, 0);

    do
    {
        rgb[0] = (rand() % 256) / 255.0;
        rgb[1] = (rand() % 256) / 255.0;
        rgb[2] = (rand() % 256) / 255.0;
        text_rgb[0] = (rand() % 256) / 255.0;
        text_rgb[1] = (rand() % 256) / 255.0;
        text_rgb[2] = (rand() % 256) / 255.0;

        yuv = rgb_to_yuv(rgb);
        text_yuv = rgb_to_yuv(text_rgb);

        euclidian_distance = sqrt(pow(yuv[0] - text_yuv[0], 2) +
                                  pow(yuv[1] - text_yuv[1], 2) +
                                  pow(yuv[2] - text_yuv[2], 2));
    }
    while(euclidian_distance < .7 || euclidian_distance > .9);

    color.r = rgb[0] * 255.0;
    color.g = rgb[1] * 255.0;
    color.b = rgb[2] * 255.0;
    color.a = 255;
    text_color.r = text_rgb[0] * 255.0;
    text_color.g = text_rgb[1] * 255.0;
    text_color.b = text_rgb[2] * 255.0;
    text_color.a = 255;

    std::vector<SDL_Color> colors = {color, text_color};
    return colors;
}

