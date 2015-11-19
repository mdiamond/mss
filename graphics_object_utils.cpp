/*
 * Matthew Diamond 2015
 * Functions for doing batch operations on graphics objects.
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

using namespace std;

/**********************************
 * BATCH GRAPHICS OBJECT CREATORS *
 **********************************/

vector<Graphics_Object *> initialize_text_objects(vector<string> names, vector<SDL_Rect> locations,
                         vector<SDL_Color> colors, vector<string> texts,
                         vector<TTF_Font *> fonts)
{
	vector<Graphics_Object *> graphics_objects = vector<Graphics_Object *>();
    Text *text = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        text = new Text(names[i], locations[i], colors[i], texts[i], fonts[i]);
        graphics_objects.push_back(text);
    }

    return graphics_objects;
}

vector<Graphics_Object *> initialize_toggle_button_objects(vector<string> names, vector<SDL_Rect> locations, vector<SDL_Color> colors,
                                  vector<SDL_Color> color_offs, vector<SDL_Color> text_color_ons, vector<SDL_Color> text_color_offs,
                                  vector<TTF_Font *> fonts,
                                  vector<string> text_ons, vector<string> text_offs,
                                  vector<bool> bs, vector<Module *> parents)
{
	vector<Graphics_Object *> graphics_objects = vector<Graphics_Object *>();
    Toggle_Button *toggle_button = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        toggle_button = new Toggle_Button(names[i], locations[i], colors[i], color_offs[i], text_color_ons[i],
                                          text_color_offs[i], fonts[i], text_ons[i], text_offs[i], bs[i], parents[i]);
        graphics_objects.push_back(toggle_button);
    }

    return graphics_objects;
}

vector<Graphics_Object *> initialize_waveform_objects(vector<string> names, vector<SDL_Rect> locations,
                             vector<SDL_Color> colors, vector<SDL_Color> background_colors,
                             vector<float> range_lows, vector<float> range_highs,
                             vector<vector<float> *> buffers)
{
	vector<Graphics_Object *> graphics_objects = vector<Graphics_Object *>();
    Waveform *waveform = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        waveform = new Waveform(names[i], locations[i], colors[i], background_colors[i], range_lows[i],
                                range_highs[i], buffers[i]);
        graphics_objects.push_back(waveform);
    }

    return graphics_objects;
}
