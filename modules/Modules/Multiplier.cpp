/*
 * Matthew Diamond 2015
 * Member functions for the Multiplier class.
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
#include "Modules/Multiplier.hpp"

/*******************************
 * MULTIPLIER MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor.
 */
Multiplier::Multiplier() :
    Module(MULTIPLIER)
{
    // The signal input needs to be 0, while the others need to be 1 to start
    // out
    inputs[MULTIPLIER_SIGNAL].val = 0;
    inputs[MULTIPLIER_MULTIPLIER].val = 1;
    inputs[MULTIPLIER_DRY_WET].val = 1;
}

/*
 * Destructor.
 */
Multiplier::~Multiplier()
{}

/*
 * Process all dependencies, then multiply the original signal by 1 - the
 * control values, and multiply the original signal by the control values
 * scaled. One done, sum the two to get the final output signal for the
 * multiplier module.
 */
void Multiplier::process()
{
    // Process any dependencies
    process_dependencies();

    if(!inputs[MULTIPLIER_SIGNAL].live)
    {
        inputs[MULTIPLIER_SIGNAL].val = 0;
    }

    for(unsigned short i = 0; i < out.size(); i ++)
    {
        update_input_vals(i);

        out[i] = (inputs[MULTIPLIER_SIGNAL].val
                     * (1 - inputs[MULTIPLIER_DRY_WET].val))
                    + (inputs[MULTIPLIER_SIGNAL].val
                       * inputs[MULTIPLIER_MULTIPLIER].val
                       * inputs[MULTIPLIER_DRY_WET].val);
    }

    processed = true;
}

/*
 * Handle user interactions with graphics objects. First call the module class
 * version of this function to handle events that might happen to any module.
 * If nothing happens in the module class version of the function, then handle
 * events specific to this module type here.
 */
bool Multiplier::handle_event(Graphics_Object *g)
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
void Multiplier::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Waveform viewer location
    location = {upper_left.x, upper_left.y + 15, MODULE_WIDTH, 54};
    graphics_object_locations["waveform"] = location;

    // Input signal related graphics object locations
    location = {upper_left.x + 2, location.y + 57, 0, 0};
    graphics_object_locations["signal text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["signal text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["signal toggle button"] = location;

    // Signal multiplier related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["signal multiplier text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["signal multiplier text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["signal multiplier toggle button"] = location;

    // Dry/wet amount related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["dry/wet amount text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["dry/wet amount text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["dry/wet amount toggle button"] = location;
}

/*
 * Initialize graphics objects unique to this module type, add them to the
 * map of graphics objects.
 */
void Multiplier::initialize_unique_graphics_objects()
{
    // Initialize text objects
    graphics_objects["signal text"] =
        new Text(name + " signal text",
                 graphics_object_locations["signal text"],
                 secondary_module_color, "INPUT SIGNAL:");
    graphics_objects["signal multiplier text"] =
        new Text(name + " signal multiplier text",
                 graphics_object_locations["signal multiplier text"],
                 secondary_module_color, "SIGNAL MULTIPLIER:");
    graphics_objects["dry/wet amount text"] =
        new Text(name + " dry/wet amount text",
                 graphics_object_locations["dry/wet amount text"],
                 secondary_module_color, "DRY/WET (0-1):");

    // Initialize waveform viewer
    graphics_objects["waveform"] =
        new Waveform(name + " waveform",
                     graphics_object_locations["waveform"],
                     primary_module_color, secondary_module_color, &out);

    // Initialize text boxes
    graphics_objects["signal text box"] =
        new Text_Box(name + " signal text box",
                     graphics_object_locations["signal text box"],
                     secondary_module_color, primary_module_color,
                     "input", (Graphics_Listener *) this);
    graphics_objects["signal multiplier text box"] =
        new Text_Box(name + " signal multiplier text box",
                     graphics_object_locations["signal multiplier text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["dry/wet amount text box"] =
        new Text_Box(name + " dry/wet amount text box",
                     graphics_object_locations["dry/wet amount text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);

    // Initialize toggle buttons
    graphics_objects["signal toggle button"] =
        new Toggle_Button(name + " signal toggle button",
                          graphics_object_locations["signal toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["signal multiplier toggle button"] =
        new Toggle_Button(name + " signal multiplier toggle button",
                          graphics_object_locations["signal multiplier toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["dry/wet amount toggle button"] =
        new Toggle_Button(name + " dry/wet amount toggle button",
                          graphics_object_locations["dry/wet amount toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);

    // Store pointers to these graphics objects in the necessary data
    // structures
    text_box_to_input_num[(Text_Box *) graphics_objects["signal text box"]] = MULTIPLIER_SIGNAL;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal toggle button"]] = MULTIPLIER_SIGNAL;
    inputs[MULTIPLIER_SIGNAL].text_box = (Text_Box *) graphics_objects["signal text box"];
    inputs[MULTIPLIER_SIGNAL].toggle_button = (Toggle_Button *) graphics_objects["signal toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["signal multiplier text box"]] = MULTIPLIER_MULTIPLIER;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal multiplier toggle button"]] = MULTIPLIER_MULTIPLIER;
    inputs[MULTIPLIER_MULTIPLIER].text_box = (Text_Box *) graphics_objects["signal multiplier text box"];
    inputs[MULTIPLIER_MULTIPLIER].toggle_button = (Toggle_Button *) graphics_objects["signal multiplier toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["dry/wet amount text box"]] = MULTIPLIER_DRY_WET;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["dry/wet amount toggle button"]] = MULTIPLIER_DRY_WET;
    inputs[MULTIPLIER_DRY_WET].text_box = (Text_Box *) graphics_objects["dry/wet amount text box"];
    inputs[MULTIPLIER_DRY_WET].toggle_button = (Toggle_Button *) graphics_objects["dry/wet amount toggle button"];
}

std::string Multiplier::get_unique_text_representation()
{
    return "";
}

