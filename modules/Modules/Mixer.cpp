/*
 * Matthew Diamond 2015
 * Member functions for the mixer class.
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
#include "Modules/Mixer.hpp"

// Included graphics classes
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

/**************************
 * MIXER MEMBER FUNCTIONS *
 **************************/

/*
 * Constructor.
 */
Mixer::Mixer() :
    Module(MIXER),
    auto_attenuate(true)
{
    // All multiplier floats should start at 1
    for(unsigned int i = 0; i < inputs.size(); i ++)
        if(i % 2 == 1)
        {
            inputs[i].val = 1;
        }
}

/*
 * Destructor.
 */
Mixer::~Mixer()
{}

/*
 * Process all dependencies, then sum and attenuate all signal inputs.
 */
void Mixer::process()
{
    // Process any dependencies
    process_dependencies();

    short num_channels = 0;

    // Reset the output buffer
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        out[i] = 0;
    }

    // For each sample
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        // Update parameters
        update_input_vals(i);

        num_channels = 0;

        // Determine how many channels are active
        for(unsigned int j = 0; j < inputs.size(); j += 2)
            if(inputs[j].live)
            {
                num_channels ++;
            }

        // For each live signal, fetch the relevant sample, multiply by the
        // associated input multiplier, then add it to the associated sample in
        // the output buffer
        for(unsigned int j = 0; j < inputs.size(); j += 2)
            if(inputs[j].live)
            {
                out[i] += inputs[j].val * inputs[j + 1].val;
            }

        // If auto attenuation is enabled, divide the signal by the number of
        // signals active
        if(auto_attenuate)
            if(num_channels != 0)
            {
                out[i] /= num_channels;
            }
    }

    processed = true;
}

/*
 * Calculate the locations of graphics objects unique to this module type, add
 * them to the map of graphics object locations.
 */
void Mixer::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Waveform viewer location
    location = {upper_left.x, upper_left.y + 15, MODULE_WIDTH, 34};
    graphics_object_locations["waveform"] = location;

    // Signals text graphics object location
    location = {upper_left.x + 2, location.y + 37, 0, 0};
    graphics_object_locations["signals text"] = location;

    // Signal and signal multipler related graphics object locations
    for(int i = 1; i <= 5; i ++)
    {
        location = {upper_left.x, location.y + 10, (MODULE_WIDTH / 2) - 9, 9};
        graphics_object_locations["signal " + std::to_string(i) + " text box"] = location;
        location = {location.x + location.w + 1, location.y, 7, 9};
        graphics_object_locations["signal " + std::to_string(i) + " toggle button"] = location;
        location = {location.x + location.w + 1,
                    location.y, (MODULE_WIDTH / 2) - 8, 9};
        graphics_object_locations["signal " + std::to_string(i) + " multiplier text box"] = location;
        location = {location.x + location.w + 1, location.y, 7, 9};
        graphics_object_locations["signal " + std::to_string(i) + " multiplier toggle button"] = location;
    }

    // Audio on/off related graphics object locations
    location = {upper_left.x, location.y + 10, 0, 0};
    graphics_object_locations["auto attenuate on/off text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH, 9};
    graphics_object_locations["auto attenuate on/off toggle button"] = location;
}

/*
 * Initialize graphics objects unique to this module type, add them to the
 * map of graphics objects.
 */
void Mixer::initialize_unique_graphics_objects()
{
    // Initialize text objects
    graphics_objects["signals text"] =
        new Text(name + " signals text",
                 graphics_object_locations["signals text"],
                 secondary_module_color, "SIGNALS & MULTIPLIERS:");
    graphics_objects["auto attenuate on/off text"] =
        new Text(name + " auto attenuate on/off text",
                 graphics_object_locations["auto attenuate on/off text"],
                 secondary_module_color, "AUTO ATTENUATE:");

    // Initialize waveform viewer
    graphics_objects["waveform"] =
        new Waveform(name + " waveform",
                     graphics_object_locations["waveform"],
                     primary_module_color, secondary_module_color, &out);

    // Initialize input text boxes
    graphics_objects["signal 1 text box"] =
        new Input_Text_Box(name + " signal 1 text box",
                           graphics_object_locations["signal 1 text box"],
                           secondary_module_color, primary_module_color,
                           "input", this, MIXER_SIGNAL_1, NULL);
    graphics_objects["signal 1 multiplier text box"] =
        new Input_Text_Box(name + " signal 1 multiplier text box",
                           graphics_object_locations["signal 1 multiplier text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, MIXER_SIGNAL_1_MULTIPLIER, NULL);
    graphics_objects["signal 2 text box"] =
        new Input_Text_Box(name + " signal 2 text box",
                           graphics_object_locations["signal 2 text box"],
                           secondary_module_color, primary_module_color,
                           "input", this, MIXER_SIGNAL_2, NULL);
    graphics_objects["signal 2 multiplier text box"] =
        new Input_Text_Box(name + " signal 2 multiplier text box",
                           graphics_object_locations["signal 2 multiplier text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, MIXER_SIGNAL_2_MULTIPLIER, NULL);
    graphics_objects["signal 3 text box"] =
        new Input_Text_Box(name + " signal 3 text box",
                           graphics_object_locations["signal 3 text box"],
                           secondary_module_color, primary_module_color,
                           "input", this, MIXER_SIGNAL_3, NULL);
    graphics_objects["signal 3 multiplier text box"] =
        new Input_Text_Box(name + " signal 3 multiplier text box",
                           graphics_object_locations["signal 3 multiplier text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, MIXER_SIGNAL_3_MULTIPLIER, NULL);
    graphics_objects["signal 4 text box"] =
        new Input_Text_Box(name + " signal 4 text box",
                           graphics_object_locations["signal 4 text box"],
                           secondary_module_color, primary_module_color,
                           "input", this, MIXER_SIGNAL_4, NULL);
    graphics_objects["signal 4 multiplier text box"] =
        new Input_Text_Box(name + " signal 4 multiplier text box",
                           graphics_object_locations["signal 4 multiplier text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, MIXER_SIGNAL_4_MULTIPLIER, NULL);
    graphics_objects["signal 5 text box"] =
        new Input_Text_Box(name + " signal 5 text box",
                           graphics_object_locations["signal 5 text box"],
                           secondary_module_color, primary_module_color,
                           "input", this, MIXER_SIGNAL_5, NULL);
    graphics_objects["signal 5 multiplier text box"] =
        new Input_Text_Box(name + " signal 5 multiplier text box",
                           graphics_object_locations["signal 5 multiplier text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, MIXER_SIGNAL_5_MULTIPLIER, NULL);

    // Initialize input toggle buttons
    graphics_objects["signal 1 toggle button"] =
        new Input_Toggle_Button(name + " signal 1 toggle button",
                                graphics_object_locations["signal 1 toggle button"],
                                secondary_module_color, primary_module_color,
                                this, MIXER_SIGNAL_1,
                                (Input_Text_Box *) graphics_objects["signal 1 text box"]);
    graphics_objects["signal 1 multiplier toggle button"] =
        new Input_Toggle_Button(name + " signal 1 multiplier toggle button",
                                graphics_object_locations["signal 1 multiplier toggle button"],
                                secondary_module_color, primary_module_color,
                                this, MIXER_SIGNAL_1_MULTIPLIER,
                                (Input_Text_Box *) graphics_objects["signal 1 multiplier text box"]);
    graphics_objects["signal 2 toggle button"] =
        new Input_Toggle_Button(name + " signal 2 toggle button",
                                graphics_object_locations["signal 2 toggle button"],
                                secondary_module_color, primary_module_color,
                                this, MIXER_SIGNAL_2,
                                (Input_Text_Box *) graphics_objects["signal 2 text box"]);
    graphics_objects["signal 2 multiplier toggle button"] =
        new Input_Toggle_Button(name + " signal 2 multiplier toggle button",
                                graphics_object_locations["signal 2 multiplier toggle button"],
                                secondary_module_color, primary_module_color,
                                this, MIXER_SIGNAL_2_MULTIPLIER,
                                (Input_Text_Box *) graphics_objects["signal 2 multiplier text box"]);
    graphics_objects["signal 3 toggle button"] =
        new Input_Toggle_Button(name + " signal 3 toggle button",
                                graphics_object_locations["signal 3 toggle button"],
                                secondary_module_color, primary_module_color,
                                this, MIXER_SIGNAL_3,
                                (Input_Text_Box *) graphics_objects["signal 3 text box"]);
    graphics_objects["signal 3 multiplier toggle button"] =
        new Input_Toggle_Button(name + " signal 3 multiplier toggle button",
                                graphics_object_locations["signal 3 multiplier toggle button"],
                                secondary_module_color, primary_module_color,
                                this, MIXER_SIGNAL_3_MULTIPLIER,
                                (Input_Text_Box *) graphics_objects["signal 3 multiplier text box"]);
    graphics_objects["signal 4 toggle button"] =
        new Input_Toggle_Button(name + " signal 4 toggle button",
                                graphics_object_locations["signal 4 toggle button"],
                                secondary_module_color, primary_module_color,
                                this, MIXER_SIGNAL_4,
                                (Input_Text_Box *) graphics_objects["signal 4 text box"]);
    graphics_objects["signal 4 multiplier toggle button"] =
        new Input_Toggle_Button(name + " signal 4 multiplier toggle button",
                                graphics_object_locations["signal 4 multiplier toggle button"],
                                secondary_module_color, primary_module_color,
                                this, MIXER_SIGNAL_4_MULTIPLIER,
                                (Input_Text_Box *) graphics_objects["signal 4 multiplier text box"]);
    graphics_objects["signal 5 toggle button"] =
        new Input_Toggle_Button(name + " signal 5 toggle button",
                                graphics_object_locations["signal 5 toggle button"],
                                secondary_module_color, primary_module_color,
                                this, MIXER_SIGNAL_5,
                                (Input_Text_Box *) graphics_objects["signal 5 text box"]);
    graphics_objects["signal 5 multiplier toggle button"] =
        new Input_Toggle_Button(name + " signal 5 multiplier toggle button",
                                graphics_object_locations["signal 5 multiplier toggle button"],
                                secondary_module_color, primary_module_color,
                                this, MIXER_SIGNAL_5_MULTIPLIER,
                                (Input_Text_Box *) graphics_objects["signal 5 multiplier text box"]);

    // Point input text boxes to their associated input toggle buttons
    ((Input_Text_Box *)
     graphics_objects["signal 1 text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal 1 toggle button"];
    ((Input_Text_Box *)
     graphics_objects["signal 1 multiplier text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal 1 multiplier toggle button"];
    ((Input_Text_Box *)
     graphics_objects["signal 2 text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal 2 toggle button"];
    ((Input_Text_Box *)
     graphics_objects["signal 2 multiplier text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal 2 multiplier toggle button"];
    ((Input_Text_Box *)
     graphics_objects["signal 3 text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal 3 toggle button"];
    ((Input_Text_Box *)
     graphics_objects["signal 3 multiplier text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal 3 multiplier toggle button"];
    ((Input_Text_Box *)
     graphics_objects["signal 4 text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal 4 toggle button"];
    ((Input_Text_Box *)
     graphics_objects["signal 4 multiplier text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal 4 multiplier toggle button"];
    ((Input_Text_Box *)
     graphics_objects["signal 5 text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal 5 toggle button"];
    ((Input_Text_Box *)
     graphics_objects["signal 5 multiplier text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal 5 multiplier toggle button"];

    // Initialize filter type toggle buttons
    graphics_objects["auto attenuate on/off toggle button"] =
        new Toggle_Button(name + " auto attenuate on/off toggle button",
                          graphics_object_locations["auto attenuate on/off toggle button"],
                          secondary_module_color, primary_module_color,
                          primary_module_color, secondary_module_color,
                          FONT, "ON", "OFF", true, this);
}

std::string Mixer::get_unique_text_representation()
{
    return "";
}

/*
 * Handle button presses. Mixer button presses are used to remove the module.
 */
void Mixer::button_function(Button *button)
{
    if(button == graphics_objects["remove module button"])
    {
        delete this;
    }
}

/*
 * Mixer has no toggle buttons. This is a dummy function.
 */
void Mixer::toggle_button_function(Toggle_Button *toggle_button)
{
    if(toggle_button == graphics_objects["auto attenuate on/off toggle button"])
    {
        auto_attenuate = !auto_attenuate;
    }
}

