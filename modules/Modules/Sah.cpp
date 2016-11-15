/*
 * Matthew Diamond 2015
 * Member functions for the Sample and hold class.
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
#include "function_forwarder.hpp"
#include "graphics_object_utils.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included modules classes
#include "Module.hpp"
#include "Modules/Sah.hpp"

// Included graphics classes
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

/*******************************
 * SAH MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor.
 */
Sah::Sah() :
    Module(SAH)
{
    inputs[SAH_SIGNAL].val = 0;
    inputs[SAH_HOLD_TIME].val = 500;
}

/*
 * Destructor.
 */
Sah::~Sah()
{}

/*
 * Process all dependencies, then start sampling and holding.
 */
void Sah::process()
{
    // Process any dependencies
    process_dependencies();

    for(unsigned int i = 0; i < BUFFER_SIZE; i ++)
    {
        update_input_vals(i);

        if(!inputs[SAH_SIGNAL].live)
        {
            inputs[SAH_SIGNAL].val = 0;
        }

        // If the amount of time until the next sample has passed, update the
        // sample to hold, update the hold time, then update the time to next
        // sample
        if(time_to_next_sample <= 0)
        {
            sample = inputs[SAH_SIGNAL].val;
            time_to_next_sample = inputs[SAH_HOLD_TIME].val;
        }

        // Set the output samples to the currently held sample,
        // then decrement the time to next sample by a single sample
        // in ms
        out[i] = sample;
        time_to_next_sample -= ((double) 1000.0 / (double) SAMPLE_RATE);
    }

    processed = true;
}

/*
 * Handle user interactions with graphics objects. First call the module class
 * version of this function to handle events that might happen to any module.
 * If nothing happens in the module class version of the function, then handle
 * events specific to this module type here.
 */
bool Sah::handle_event(Graphics_Object *g)
{
    // if g is null, return false
    if(g == nullptr)
    {
        return false;
    }
    // Handle events that apply to all modules, return true if an event
    // is handled
    else if(Module::handle_event(g))
    {
        return true;
    }
    else if(g == graphics_objects["reset sampler button"])
    {
        reset_sampler();
        return true;
    }

    // If none of the above happen, return false
    return false;
}

/*
 * Calculate the locations of graphics objects unique to this module type, add
 * them to the map of graphics object locations.
 */
void Sah::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Reset sampler button location
    location = {upper_left.x + MODULE_WIDTH - 15, upper_left.y, 7, 9};
    graphics_object_locations["reset sampler button"] = location;

    // Waveform viewer location
    location = {upper_left.x, upper_left.y + 15, MODULE_WIDTH, 74};
    graphics_object_locations["waveform"] = location;

    // Input signal related graphics object locations
    location = {upper_left.x + 2, location.y + 77, 0, 0};
    graphics_object_locations["signal text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["signal text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["signal toggle button"] = location;

    // Hold time related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["hold time text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["hold time text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["hold time toggle button"] = location;
}

/*
 * Initialize graphics objects unique to this module type, add them to the
 * map of graphics objects.
 */
void Sah::initialize_unique_graphics_objects()
{
    // Initialize reset buffer button
    graphics_objects["reset sampler button"] =
        new Button(name + " reset sampler button",
                   graphics_object_locations["reset sampler button"],
                   secondary_module_color, primary_module_color, "0", this);

    // Initialize text objects
    graphics_objects["signal text"] =
        new Text(name + " signal text",
                 graphics_object_locations["signal text"],
                 secondary_module_color, "INPUT SIGNAL:");
    graphics_objects["hold time text"] =
        new Text(name + " hold time text",
                 graphics_object_locations["hold time text"],
                 secondary_module_color, "HOLD TIME (ms):");

    // Initialize waveform viewer
    graphics_objects["waveform"] =
        new Waveform(name + " waveform",
                     graphics_object_locations["waveform"],
                     primary_module_color, secondary_module_color, &out);

    // Initialize input text boxes
    graphics_objects["signal text box"] =
        new Input_Text_Box(name + " signal text box",
                           graphics_object_locations["signal text box"],
                           secondary_module_color, primary_module_color,
                           "input", this, SAH_SIGNAL, NULL);
    graphics_objects["hold time text box"] =
        new Input_Text_Box(name + " hold time text box",
                           graphics_object_locations["hold time text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, SAH_HOLD_TIME, NULL);

    // Initialize input toggle buttons
    graphics_objects["signal toggle button"] =
        new Input_Toggle_Button(name + " signal toggle button",
                                graphics_object_locations["signal toggle button"],
                                secondary_module_color, primary_module_color,
                                this, SAH_SIGNAL,
                                (Input_Text_Box *) graphics_objects["signal text box"]);
    graphics_objects["hold time toggle button"] =
        new Input_Toggle_Button(name + " hold time toggle button",
                                graphics_object_locations["hold time toggle button"],
                                secondary_module_color, primary_module_color,
                                this, SAH_HOLD_TIME,
                                (Input_Text_Box *) graphics_objects["hold time text box"]);

    // Point input text boxes to their associated input toggle buttons
    ((Input_Text_Box *)
     graphics_objects["signal text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal toggle button"];
    ((Input_Text_Box *)
     graphics_objects["hold time text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["hold time toggle button"];
}

/*
 * Reset the time to next sample.
 */
void Sah::reset_sampler()
{
    time_to_next_sample = 0;
}

std::string Sah::get_unique_text_representation()
{
    return "";
}

