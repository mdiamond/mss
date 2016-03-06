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

std::vector<float> yuv_to_rgb(std::vector<float> yuv)
{
    float r, g, b;

    r = yuv[0] + (1.14 * yuv[2]);
    g = yuv[0] - (.395 * yuv[1]) - (.581 * yuv[2]);
    b = yuv[0] + (2.032 * yuv[1]);

    std::vector<float> rgb = {r, g, b};

    return rgb;
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
    } while(euclidian_distance < .7 || euclidian_distance > .9);

    rgb = yuv_to_rgb(yuv);
    text_rgb = yuv_to_rgb(text_yuv);

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
