/*
 * Matthew Diamond 2017
 * Member functions for the input class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL.h"
#include "SDL_ttf.h"

// Included files
#include "graphics_object_utils.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included modules classes
#include "Module.hpp"
#include "Modules/Input.hpp"

/**************************
 * INPUT MEMBER FUNCTIONS *
 **************************/

/*
 * Constructor.
 */
Input::Input() :
    Module(INPUT)
{}

/*
 * Destructor.
 */
Input::~Input()
{}

/*
 * Simply copy the contents of the input buffer associated with the specified
 * input source into this modules output buffer.
 */
void Input::process()
{
    int i = 0;
    for(auto it = out.begin(); it != out.end(); it ++)
    {
        *it = AUDIO_IN[i];
        i ++;
    }

    processed = true;
}

/*
 * Handle user interactions with graphics objects. First call the module class
 * version of this function to handle events that might happen to any module.
 * If nothing happens in the module class version of the function, then handle
 * events specific to this module type here.
 */
bool Input::handle_event(Graphics_Object *g)
{
    // If g is null, take no action, return false
    if(g == nullptr)
    {
        return false;
    }
    // If none of the above, handle events that apply to all modules, return
    // true if an event is handled
    else if(Module::handle_event(g))
    {
        return true;
    }

    // If none of the above, return false
    return false;
}

/*
 * Calculate the locations of graphics objects unique to this module type, add
 * them to the map of graphics object locations.
 */
void Input::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Waveform viewer location
    location = {upper_left.x, upper_left.y + 15, MODULE_WIDTH, 116};
    graphics_object_locations["waveform"] = location;
}

/*
 * Initialize graphics objects unique to this module type, add them to the
 * map of graphics objects.
 */
void Input::initialize_unique_graphics_objects()
{
    // Initialize waveform viewer
    graphics_objects["waveform"] =
        new Waveform(name + " waveform",
                     graphics_object_locations["waveform"],
                     primary_module_color, secondary_module_color, &out);
}

std::string Input::get_unique_text_representation()
{
    return "";
}

