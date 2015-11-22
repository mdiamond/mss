/*
 * Matthew Diamond 2015
 * Functions for constructing batches of graphics objects.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "image_processing.hpp"
#include "main.hpp"

// Included classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Text_Box.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

/**********************************
 * BATCH GRAPHICS OBJECT CREATORS *
 **********************************/

/*
 * Initialize a batch of button objects given arrays of contructor inputs.
 * Return a vector of the contructed graphics objects.
 */
std::vector<Graphics_Object *> initialize_button_objects(std::vector<std::string> names, std::vector<SDL_Rect> locations,
                                                         std::vector<SDL_Color> colors, std::vector<SDL_Color> text_colors,
                                                         std::vector<std::string> texts, std::vector<Module *> parents)
{
    std::vector<Graphics_Object *> graphics_objects = std::vector<Graphics_Object *>();
    Button *button = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        button = new Button(names[i], locations[i], colors[i], text_colors[i], texts[i], parents[i]);
        graphics_objects.push_back(button);
    }

    return graphics_objects;
}

/*
 * Initialize a batch of text objects given arrays of contructor inputs.
 * Return a vector of the contructed graphics objects.
 */
std::vector<Graphics_Object *> initialize_text_objects(std::vector<std::string> names, std::vector<SDL_Rect> locations,
                                                       std::vector<SDL_Color> colors, std::vector<std::string> texts,
                                                       std::vector<TTF_Font *> fonts)
{
    std::vector<Graphics_Object *> graphics_objects = std::vector<Graphics_Object *>();
    Text *text = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        text = new Text(names[i], locations[i], colors[i], texts[i], fonts[i]);
        graphics_objects.push_back(text);
    }

    return graphics_objects;
}

/*
 * Initialize a batch of toggle button objects given arrays of contructor inputs.
 * Return a vector of the contructed graphics objects.
 */
std::vector<Graphics_Object *> initialize_toggle_button_objects(std::vector<std::string> names, std::vector<SDL_Rect> locations, std::vector<SDL_Color> colors,
                                                                std::vector<SDL_Color> color_offs, std::vector<SDL_Color> text_color_ons, std::vector<SDL_Color> text_color_offs,
                                                                std::vector<TTF_Font *> fonts,
                                                                std::vector<std::string> text_ons, std::vector<std::string> text_offs,
                                                                std::vector<bool> bs, std::vector<Module *> parents)
{
    std::vector<Graphics_Object *> graphics_objects = std::vector<Graphics_Object *>();
    Toggle_Button *toggle_button = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        toggle_button = new Toggle_Button(names[i], locations[i], colors[i], color_offs[i], text_color_ons[i],
                                          text_color_offs[i], fonts[i], text_ons[i], text_offs[i], bs[i], parents[i]);
        graphics_objects.push_back(toggle_button);
    }

    return graphics_objects;
}

/*
 * Initialize a batch of waveform objects given arrays of contructor inputs.
 * Return a vector of the contructed graphics objects.
 */
std::vector<Graphics_Object *> initialize_waveform_objects(std::vector<std::string> names, std::vector<SDL_Rect> locations,
                                                           std::vector<SDL_Color> colors, std::vector<SDL_Color> background_colors,
                                                           std::vector<float> range_lows, std::vector<float> range_highs,
                                                           std::vector<std::vector<float> *> buffers)
{
    std::vector<Graphics_Object *> graphics_objects = std::vector<Graphics_Object *>();
    Waveform *waveform = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        waveform = new Waveform(names[i], locations[i], colors[i], background_colors[i], range_lows[i],
                                range_highs[i], buffers[i]);
        graphics_objects.push_back(waveform);
    }

    return graphics_objects;
}
