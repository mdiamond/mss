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
 * other modules of this type. For example, if there exists filters 1-6
 * and a filter 8, this function should return 7 if passed FILTER as
 * its argument.
 */
int find_available_module_number(int module_type)
{
    std::vector<bool> nums;

    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i]->type == module_type)
        {
            unsigned int current_num = stoi(MODULES[i]->name.substr(MODULES[i]->name.find(" ")).substr(1));
            std::cout << current_num << std::endl;
            while(nums.size() < current_num)
                nums.push_back(false);
            nums[current_num - 1] = true;
        }
    }

    for(unsigned int i = 0; i < nums.size(); i ++)
        std::cout << nums[i] << ", ";
    std::cout << std::endl;

    for(unsigned int i = 0; i < nums.size(); i ++)
    {
        if(!nums[i])
            return i + 2;
    }

    return nums.size() + 1;
}
