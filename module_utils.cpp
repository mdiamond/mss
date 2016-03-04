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
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "main.hpp"

// Included classes
#include "Module.hpp"

/********************************
 * FIND AVAILABLE MODULE NUMBER *
 ********************************/

/*
 * This function takes a module type as input and returns
 * the first number that is not already contained in the name of
 * other modules of this type. For example, if there exist filters 1-6
 * and a filter 8, this function should return 7 if passed FILTER as
 * its argument.
 */
int find_available_module_number(int module_type)
{
    std::vector<bool> nums;

    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != NULL)
        {
            if(MODULES[i]->type == module_type)
            {
                unsigned int current_num = stoi(MODULES[i]->name.substr(MODULES[i]->name.find(" ")).substr(1));

                while(nums.size() < current_num)
                    nums.push_back(false);

                nums[current_num - 1] = true;
            }
        }
    }

    for(unsigned int i = 0; i < nums.size(); i ++)
    {
        if(!nums[i])
            return i + 1;
    }

    return nums.size() + 1;
}

int find_available_module_slot()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(MODULES[i] == NULL)
            return i;

    return MODULES.size();
}

std::vector<SDL_Color> generate_module_colors()
{
    SDL_Color color;
    SDL_Color text_color;
    float euclidian_distance;

    do
    {
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
        text_color.r = rand() % 256;
        text_color.g = rand() % 256;
        text_color.b = rand() % 256;

        euclidian_distance = sqrt(pow(color.r - text_color.r, 2) +
                                  pow(color.g - text_color.g, 2) +
                                  pow(color.b - text_color.b, 2));
    } while(euclidian_distance < 250);

    color.a = 255;
    text_color.a = 255;

    std::vector<SDL_Color> colors = {color, text_color};
    return colors;
}
