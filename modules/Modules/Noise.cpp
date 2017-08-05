/*
 * Matthew Diamond 2015
 * Member functions for the Noise class.
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
#include "Modules/Noise.hpp"

/**************************
 * NOISE MEMBER FUNCTIONS *
 **************************/

/*
 * Constructor.
 */
Noise::Noise() :
    Module(NOISE)
{
    inputs[NOISE_RANGE_LOW].val = -1;
    inputs[NOISE_RANGE_HIGH].val = 1;
}

/*
 * Destructor.
 */
Noise::~Noise()
{}

/*
 * Produce a random number in the range [-1, 1].
 */
double Noise::produce_white_noise_sample()
{
    return scale_sample(((float) rand() / RAND_MAX) * 2, 0, 2, -1, 1);
}

/*
 * Fill the output buffer with samples depending on the type
 * of noise selected.
 */
void Noise::process()
{
    process_dependencies();

    for(unsigned short i = 0; i < out.size(); i ++)
    {
        update_input_vals(i);

        out[i] = produce_white_noise_sample();
        out[i] = scale_sample(out[i], -1, 1, inputs[NOISE_RANGE_LOW].val,
                                 inputs[NOISE_RANGE_HIGH].val);
    }

    processed = true;
}

/*
 * Handle user interactions with graphics objects. First call the module class
 * version of this function to handle events that might happen to any module.
 * If nothing happens in the module class version of the function, then handle
 * events specific to this module type here.
 */
bool Noise::handle_event(Graphics_Object *g)
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
void Noise::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Waveform viewer location
    location = {upper_left.x, upper_left.y + 15, MODULE_WIDTH, 74};
    graphics_object_locations["waveform"] = location;

    // Range low related graphics object locations
    location = {upper_left.x + 2, location.y + 77, 0, 0};
    graphics_object_locations["range low text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["range low text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["range low toggle button"] = location;

    // Range high related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["range high text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["range high text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["range high toggle button"] = location;
}

/*
 * Initialize graphics objects unique to this module type, add them to the
 * map of graphics objects.
 */
void Noise::initialize_unique_graphics_objects()
{
    // Initialize text objects
    graphics_objects["range low text"] =
        new Text(name + " ranges text",
                 graphics_object_locations["ranges text"],
                 secondary_module_color, "RANGE LOW (#):");
    graphics_objects["range high text"] =
        new Text(name + " range high text",
                 graphics_object_locations["range high text"],
                 secondary_module_color, "RANGE HIGH (#):");

    // Initialize waveform viewer
    graphics_objects["waveform"] =
        new Waveform(name + " waveform",
                     graphics_object_locations["waveform"],
                     primary_module_color, secondary_module_color, &out);

    // Initialize text boxes
    graphics_objects["range low text box"] =
        new Text_Box(name + " range low text box",
                     graphics_object_locations["range low text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["range high text box"] =
        new Text_Box(name + " range high text box",
                     graphics_object_locations["range high text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);

    // Initialize toggle buttons
    graphics_objects["range low toggle button"] =
        new Toggle_Button(name + " range low toggle button",
                          graphics_object_locations["range low toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["range high toggle button"] =
        new Toggle_Button(name + " range high toggle button",
                          graphics_object_locations["range high toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);

    // Store pointers to these graphics objects in the necessary data
    // structures
    text_box_to_input_num[(Text_Box *) graphics_objects["range low text box"]] = NOISE_RANGE_LOW;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["range low toggle button"]] = NOISE_RANGE_LOW;
    inputs[NOISE_RANGE_LOW].text_box = (Text_Box *) graphics_objects["range low text box"];
    inputs[NOISE_RANGE_LOW].toggle_button = (Toggle_Button *) graphics_objects["range low toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["range high text box"]] = NOISE_RANGE_HIGH;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["range high toggle button"]] = NOISE_RANGE_HIGH;
    inputs[NOISE_RANGE_HIGH].text_box = (Text_Box *) graphics_objects["range high text box"];
    inputs[NOISE_RANGE_HIGH].toggle_button = (Toggle_Button *) graphics_objects["range high toggle button"];
}

std::string Noise::get_unique_text_representation()
{
    return "";
}

