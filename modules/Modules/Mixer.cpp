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

/**************************
 * MIXER MEMBER FUNCTIONS *
 **************************/

/*
 * Constructor.
 */
Mixer::Mixer() :
    Module(MIXER),
    auto_attenuate(false)
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
    short num_channels = 0;

    // Process any dependencies
    process_dependencies();

    // Reset the output buffer
    std::fill(out.begin(), out.end(), 0);

    // For each sample
    for(unsigned int i = 0; i < BUFFER_SIZE; i ++)
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
 * Handle user interactions with graphics objects. First call the module class
 * version of this function to handle events that might happen to any module.
 * If nothing happens in the module class version of the function, then handle
 * events specific to this module type here.
 */
bool Mixer::handle_event(Graphics_Object *g)
{
    // If g is null, take no action, return false
    if(g == nullptr)
    {
        return false;
    }
    // Handle auto attenuate toggle button
    else if(g == graphics_objects["auto attenuate toggle button"])
    {
        toggle_auto_attenuation();
        return true;
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
    graphics_object_locations["auto attenuate text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH, 9};
    graphics_object_locations["auto attenuate toggle button"] = location;
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
    graphics_objects["auto attenuate text"] =
        new Text(name + " auto attenuate text",
                 graphics_object_locations["auto attenuate text"],
                 secondary_module_color, "AUTO ATTENUATE:");

    // Initialize waveform viewer
    graphics_objects["waveform"] =
        new Waveform(name + " waveform",
                     graphics_object_locations["waveform"],
                     primary_module_color, secondary_module_color, &out);

    // Initialize text boxes
    graphics_objects["signal 1 text box"] =
        new Text_Box(name + " signal 1 text box",
                     graphics_object_locations["signal 1 text box"],
                     secondary_module_color, primary_module_color,
                     "input", (Graphics_Listener *) this);
    graphics_objects["signal 1 multiplier text box"] =
        new Text_Box(name + " signal 1 multiplier text box",
                     graphics_object_locations["signal 1 multiplier text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["signal 2 text box"] =
        new Text_Box(name + " signal 2 text box",
                     graphics_object_locations["signal 2 text box"],
                     secondary_module_color, primary_module_color,
                     "input", (Graphics_Listener *) this);
    graphics_objects["signal 2 multiplier text box"] =
        new Text_Box(name + " signal 2 multiplier text box",
                     graphics_object_locations["signal 2 multiplier text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["signal 3 text box"] =
        new Text_Box(name + " signal 3 text box",
                     graphics_object_locations["signal 3 text box"],
                     secondary_module_color, primary_module_color,
                     "input", (Graphics_Listener *) this);
    graphics_objects["signal 3 multiplier text box"] =
        new Text_Box(name + " signal 3 multiplier text box",
                     graphics_object_locations["signal 3 multiplier text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["signal 4 text box"] =
        new Text_Box(name + " signal 4 text box",
                     graphics_object_locations["signal 4 text box"],
                     secondary_module_color, primary_module_color,
                     "input", (Graphics_Listener *) this);
    graphics_objects["signal 4 multiplier text box"] =
        new Text_Box(name + " signal 4 multiplier text box",
                     graphics_object_locations["signal 4 multiplier text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);
    graphics_objects["signal 5 text box"] =
        new Text_Box(name + " signal 5 text box",
                     graphics_object_locations["signal 5 text box"],
                     secondary_module_color, primary_module_color,
                     "input", (Graphics_Listener *) this);
    graphics_objects["signal 5 multiplier text box"] =
        new Text_Box(name + " signal 5 multiplier text box",
                     graphics_object_locations["signal 5 multiplier text box"],
                     secondary_module_color, primary_module_color,
                     "# or input", (Graphics_Listener *) this);

    // Initialize toggle buttons
    graphics_objects["signal 1 toggle button"] =
        new Toggle_Button(name + " signal 1 toggle button",
                          graphics_object_locations["signal 1 toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["signal 1 multiplier toggle button"] =
        new Toggle_Button(name + " signal 1 multiplier toggle button",
                          graphics_object_locations["signal 1 multiplier toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["signal 2 toggle button"] =
        new Toggle_Button(name + " signal 2 toggle button",
                          graphics_object_locations["signal 2 toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["signal 2 multiplier toggle button"] =
        new Toggle_Button(name + " signal 2 multiplier toggle button",
                          graphics_object_locations["signal 2 multiplier toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["signal 3 toggle button"] =
        new Toggle_Button(name + " signal 3 toggle button",
                          graphics_object_locations["signal 3 toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["signal 3 multiplier toggle button"] =
        new Toggle_Button(name + " signal 3 multiplier toggle button",
                          graphics_object_locations["signal 3 multiplier toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["signal 4 toggle button"] =
        new Toggle_Button(name + " signal 4 toggle button",
                          graphics_object_locations["signal 4 toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["signal 4 multiplier toggle button"] =
        new Toggle_Button(name + " signal 4 multiplier toggle button",
                          graphics_object_locations["signal 4 multiplier toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["signal 5 toggle button"] =
        new Toggle_Button(name + " signal 5 toggle button",
                          graphics_object_locations["signal 5 toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);
    graphics_objects["signal 5 multiplier toggle button"] =
        new Toggle_Button(name + " signal 5 multiplier toggle button",
                          graphics_object_locations["signal 5 multiplier toggle button"],
                          secondary_module_color, secondary_module_color,
                          RED, primary_module_color, "I", "I", false,
                          (Graphics_Listener *) this);

    // Store pointers to these graphics objects in the necessary data
    // structures
    text_box_to_input_num[(Text_Box *) graphics_objects["signal 1 text box"]] = MIXER_SIGNAL_1;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal 1 toggle button"]] = MIXER_SIGNAL_1;
    inputs[MIXER_SIGNAL_1].text_box = (Text_Box *) graphics_objects["signal 1 text box"];
    inputs[MIXER_SIGNAL_1].toggle_button = (Toggle_Button *) graphics_objects["signal 1 toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["signal 1 multiplier text box"]] = MIXER_SIGNAL_1_MULTIPLIER;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal 1 multiplier toggle button"]] = MIXER_SIGNAL_1_MULTIPLIER;
    inputs[MIXER_SIGNAL_1_MULTIPLIER].text_box = (Text_Box *) graphics_objects["signal 1 multiplier text box"];
    inputs[MIXER_SIGNAL_1_MULTIPLIER].toggle_button = (Toggle_Button *) graphics_objects["signal 1 multiplier toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["signal 2 text box"]] = MIXER_SIGNAL_2;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal 2 toggle button"]] = MIXER_SIGNAL_2;
    inputs[MIXER_SIGNAL_2].text_box = (Text_Box *) graphics_objects["signal 2 text box"];
    inputs[MIXER_SIGNAL_2].toggle_button = (Toggle_Button *) graphics_objects["signal 2 toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["signal 2 multiplier text box"]] = MIXER_SIGNAL_2_MULTIPLIER;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal 2 multiplier toggle button"]] = MIXER_SIGNAL_2_MULTIPLIER;
    inputs[MIXER_SIGNAL_2_MULTIPLIER].text_box = (Text_Box *) graphics_objects["signal 2 multiplier text box"];
    inputs[MIXER_SIGNAL_2_MULTIPLIER].toggle_button = (Toggle_Button *) graphics_objects["signal 2 multiplier toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["signal 3 text box"]] = MIXER_SIGNAL_3;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal 3 toggle button"]] = MIXER_SIGNAL_3;
    inputs[MIXER_SIGNAL_3].text_box = (Text_Box *) graphics_objects["signal 3 text box"];
    inputs[MIXER_SIGNAL_3].toggle_button = (Toggle_Button *) graphics_objects["signal 3 toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["signal 3 multiplier text box"]] = MIXER_SIGNAL_3_MULTIPLIER;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal 3 multiplier toggle button"]] = MIXER_SIGNAL_3_MULTIPLIER;
    inputs[MIXER_SIGNAL_3_MULTIPLIER].text_box = (Text_Box *) graphics_objects["signal 3 multiplier text box"];
    inputs[MIXER_SIGNAL_3_MULTIPLIER].toggle_button = (Toggle_Button *) graphics_objects["signal 3 multiplier toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["signal 4 text box"]] = MIXER_SIGNAL_4;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal 4 toggle button"]] = MIXER_SIGNAL_4;
    inputs[MIXER_SIGNAL_4].text_box = (Text_Box *) graphics_objects["signal 4 text box"];
    inputs[MIXER_SIGNAL_4].toggle_button = (Toggle_Button *) graphics_objects["signal 4 toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["signal 4 multiplier text box"]] = MIXER_SIGNAL_4_MULTIPLIER;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal 4 multiplier toggle button"]] = MIXER_SIGNAL_4_MULTIPLIER;
    inputs[MIXER_SIGNAL_4_MULTIPLIER].text_box = (Text_Box *) graphics_objects["signal 4 multiplier text box"];
    inputs[MIXER_SIGNAL_4_MULTIPLIER].toggle_button = (Toggle_Button *) graphics_objects["signal 4 multiplier toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["signal 5 text box"]] = MIXER_SIGNAL_5;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal 5 toggle button"]] = MIXER_SIGNAL_5;
    inputs[MIXER_SIGNAL_5].text_box = (Text_Box *) graphics_objects["signal 5 text box"];
    inputs[MIXER_SIGNAL_5].toggle_button = (Toggle_Button *) graphics_objects["signal 5 toggle button"];
    text_box_to_input_num[(Text_Box *) graphics_objects["signal 5 multiplier text box"]] = MIXER_SIGNAL_5_MULTIPLIER;
    toggle_button_to_input_num[(Toggle_Button *) graphics_objects["signal 5 multiplier toggle button"]] = MIXER_SIGNAL_5_MULTIPLIER;
    inputs[MIXER_SIGNAL_5_MULTIPLIER].text_box = (Text_Box *) graphics_objects["signal 5 multiplier text box"];
    inputs[MIXER_SIGNAL_5_MULTIPLIER].toggle_button = (Toggle_Button *) graphics_objects["signal 5 multiplier toggle button"];

    // Initialize filter type toggle buttons
    graphics_objects["auto attenuate toggle button"] =
        new Toggle_Button(name + " auto attenuate toggle button",
                          graphics_object_locations["auto attenuate toggle button"],
                          secondary_module_color, primary_module_color,
                          primary_module_color, secondary_module_color,
                          "ON", "OFF", false, (Graphics_Listener *) this);
}

void Mixer::toggle_auto_attenuation()
{
    auto_attenuate = !auto_attenuate;

    ((Toggle_Button *) graphics_objects["auto attenuate toggle button"])->toggle();

    std::cout << name << " auto attenuation is now "
              << (auto_attenuate ? "on" : "off") << std::endl;
}

std::string Mixer::get_unique_text_representation()
{
    return "";
}

