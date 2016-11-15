/*
 * Matthew Diamond 2015
 * Member functions for the Delay class.
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
#include "Modules/Delay.hpp"

// Included graphics classes
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

/**************************
 * DELAY MEMBER FUNCTIONS *
 **************************/

/*
 * Constructor.
 */
Delay::Delay() :
    Module(DELAY),
    circular_buffer(std::vector<float>(BUFFER_SIZE, 0)),
    current_sample(0),
    previous_max_delay_time(0),
    previous_delay_time(0)
{
    inputs[DELAY_MAX_DELAY_TIME].val = 5000;
    inputs[DELAY_DELAY_TIME].val = 2000;
    inputs[DELAY_FEEDBACK_AMOUNT].val = 0;
    inputs[DELAY_WET_DRY].val = 1;

    circular_buffer = std::vector<float>(inputs[DELAY_MAX_DELAY_TIME].val
                                         / 1000.0 * SAMPLE_RATE, 0);
    delay_samples = inputs[DELAY_DELAY_TIME].val / 1000.0 * SAMPLE_RATE;
}

/*
 * Destructor.
 */
Delay::~Delay()
{}

/*
 * Calculate a linearly interpolated sample from the buffer to return. This
 * sample may technically be in-between samples.
 */
float Delay::calculate_wet_sample()
{
    float x0, x1, y0, y1, interpolated_sample;
    float delayed_sample = current_sample - delay_samples;
    while(delayed_sample < 0)
    {
        delayed_sample += circular_buffer.size();
    }

    x0 = ((int) delayed_sample) % circular_buffer.size();
    x1 = ((int)(delayed_sample + 1)) % circular_buffer.size();
    y0 = circular_buffer[x0];
    y1 = circular_buffer[x1];

    interpolated_sample = y0 + ((y1 - y0) * ((delayed_sample - x0)
                                             / (x1 - x0)));

    return interpolated_sample;
}

/*
 * Process all dependencies, then
 * fill the output buffer with a waveform given
 * the data contained within this class and the
 * audio device information.
 */
void Delay::process()
{
    // Process any dependencies
    process_dependencies();

    // Update parameters
    update_input_vals(0);

    // If the max delay time has changed, reset the delay buffer
    if(inputs[DELAY_MAX_DELAY_TIME].val != previous_max_delay_time)
    {
        reset_buffer();
        previous_max_delay_time = inputs[DELAY_MAX_DELAY_TIME].val;
    }

    // Per sample
    for(unsigned int i = 0; i < BUFFER_SIZE; i ++)
    {
        // Update parameters
        update_input_vals(i);

        if(!inputs[DELAY_SIGNAL].live)
        {
            inputs[DELAY_SIGNAL].val = 0;
        }

        // If the delay time has changed, update the number of samples for which
        // to delay, then store the new delay time as the new previous delay
        // time
        if(inputs[DELAY_DELAY_TIME].val != previous_delay_time)
        {
            delay_samples = inputs[DELAY_DELAY_TIME].val / 1000.0 * SAMPLE_RATE;
            previous_delay_time = inputs[DELAY_DELAY_TIME].val;
        }

        if(inputs[DELAY_MAX_DELAY_TIME].val >= inputs[DELAY_DELAY_TIME].val)
        {
            // Apply the dry signal
            out[i] = (1 - inputs[DELAY_WET_DRY].val)
                        * inputs[DELAY_SIGNAL].val;

            // Apply the linearly interpolated wet signal
            float wet_sample = calculate_wet_sample();
            out[i] += inputs[DELAY_WET_DRY].val * wet_sample;

            // Update the sample in the circular buffer
            circular_buffer[current_sample] = inputs[DELAY_FEEDBACK_AMOUNT].val
                                              * wet_sample;
            circular_buffer[current_sample] += inputs[DELAY_SIGNAL].val;

            // Move on to the next sample
            current_sample ++;
            current_sample = fmod(current_sample,
                                  ((double) circular_buffer.size()));
        }
        else if(inputs[DELAY_MAX_DELAY_TIME].val < inputs[DELAY_DELAY_TIME].val)
        {
            std::cout << RED_STDOUT << name
                      << " delay time is greater than max delay time!"
                      << DEFAULT_STDOUT << std::endl;
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
bool Delay::handle_event(Graphics_Object *g)
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
    // Handle the reset buffer button
    else if(g == graphics_objects["reset buffer button"])
    {
        reset_buffer();
        return true;
    }

    // If none of the above happen, return false
    return false;
}

/*
 * Calculate the locations of graphics objects unique to this module type, add
 * them to the map of graphics object locations.
 */
void Delay::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Reset buffer button location
    location = {upper_left.x + MODULE_WIDTH - 15, upper_left.y, 7, 9};
    graphics_object_locations["reset buffer button"] = location;

    // Waveform viewer location
    location = {upper_left.x, location.y + 15, MODULE_WIDTH, 54};
    graphics_object_locations["waveform"] = location;

    // Signal input related graphics object locations
    location = {upper_left.x + 2, location.y + 57, 0, 0};
    graphics_object_locations["signal text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["signal text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["signal toggle button"] = location;

    // Max delay time/delay time related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["max delay time/delay time text"] = location;
    location = {upper_left.x, location.y + 10, (MODULE_WIDTH / 2) - 1, 9};
    graphics_object_locations["max delay time text box"] = location;
    location = {location.x + location.w + 1, location.y, (MODULE_WIDTH / 2) - 8, 9};
    graphics_object_locations["delay time text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["delay time toggle button"] = location;

    // Feedback amount/wet/dry amount related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["feedback amount/wet/dry amount text"] = location;
    location = {upper_left.x, location.y + 10, (MODULE_WIDTH / 2) - 9, 9};
    graphics_object_locations["feedback amount text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["feedback amount toggle button"] = location;
    location = {location.x + location.w + 1, location.y, (MODULE_WIDTH / 2) - 8, 9};
    graphics_object_locations["wet/dry amount text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["wet/dry amount toggle button"] = location;
}

/*
 * Initialize graphics objects unique to this module type, add them to the
 * map of graphics objects.
 */
void Delay::initialize_unique_graphics_objects()
{
    // Initialize reset buffer button
    graphics_objects["reset buffer button"] =
        new Button(name + " reset buffer button",
                   graphics_object_locations["reset buffer button"],
                   secondary_module_color, primary_module_color, "0", this);

    // Initialize text objects
    graphics_objects["signal text"] =
        new Text(name + " signal text",
                 graphics_object_locations["signal text"],
                 secondary_module_color, "SIGNAL INPUT:");
    graphics_objects["max delay time/delay time text"] =
        new Text(name + " max delay time/delay time text",
                 graphics_object_locations["max delay time/delay time text"],
                 secondary_module_color, "MAX DELAY & DELAY (ms):");
    graphics_objects["feedback amount/wet/dry amount text"] =
        new Text(name + " feedback amount/wet/dry amount text",
                 graphics_object_locations["feedback amount/wet/dry amount text"],
                 secondary_module_color, "FEEDBACK (#) & WET/DRY (0-1):");

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
                           "input", this, DELAY_SIGNAL, NULL);
    graphics_objects["max delay time text box"] =
        new Input_Text_Box(name + " max delay time text box",
                           graphics_object_locations["max delay time text box"],
                           secondary_module_color, primary_module_color,
                           "#", this, DELAY_MAX_DELAY_TIME, NULL);
    graphics_objects["delay time text box"] =
        new Input_Text_Box(name + " delay time text box",
                           graphics_object_locations["delay time text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, DELAY_DELAY_TIME, NULL);
    graphics_objects["feedback amount text box"] =
        new Input_Text_Box(name + " feedback amount text box",
                           graphics_object_locations["feedback amount text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, DELAY_FEEDBACK_AMOUNT, NULL);
    graphics_objects["wet/dry amount text box"] =
        new Input_Text_Box(name + " wet/dry amount text box",
                           graphics_object_locations["wet/dry amount text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, DELAY_WET_DRY, NULL);

    // Initialize input toggle buttons
    graphics_objects["signal toggle button"] =
        new Input_Toggle_Button(name + " signal toggle button",
                                graphics_object_locations["signal toggle button"],
                                secondary_module_color, primary_module_color,
                                this, DELAY_SIGNAL,
                                (Input_Text_Box *) graphics_objects["signal text box"]);
    graphics_objects["delay time toggle button"] =
        new Input_Toggle_Button(name + " delay time toggle button",
                                graphics_object_locations["delay time toggle button"],
                                secondary_module_color, primary_module_color,
                                this, DELAY_DELAY_TIME,
                                (Input_Text_Box *) graphics_objects["delay time text box"]);
    graphics_objects["feedback amount toggle button"] =
        new Input_Toggle_Button(name + " feedback amount toggle button",
                                graphics_object_locations["feedback amount toggle button"],
                                secondary_module_color, primary_module_color,
                                this, DELAY_FEEDBACK_AMOUNT,
                                (Input_Text_Box *) graphics_objects["feedback amount text box"]);
    graphics_objects["wet/dry amount toggle button"] =
        new Input_Toggle_Button(name + " wet/dry amount toggle button",
                                graphics_object_locations["wet/dry amount toggle button"],
                                secondary_module_color, primary_module_color,
                                this, DELAY_WET_DRY,
                                (Input_Text_Box *) graphics_objects["wet/dry amount text box"]);


    // Point input text boxes to their associated input toggle buttons
    ((Input_Text_Box *)
     graphics_objects["signal text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["signal toggle button"];
    ((Input_Text_Box *)
     graphics_objects["delay time text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["delay time toggle button"];
    ((Input_Text_Box *)
     graphics_objects["feedback amount text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["feedback amount toggle button"];
    ((Input_Text_Box *)
     graphics_objects["wet/dry amount text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["wet/dry amount toggle button"];
}

std::string Delay::get_unique_text_representation()
{
    return "";
}

/*
 * Reset this delay's buffer
 */
void Delay::reset_buffer()
{
    circular_buffer = std::vector<float>(inputs[DELAY_MAX_DELAY_TIME].val
                                         / 1000.0 * SAMPLE_RATE, 0);

    std::cout << name << " buffer reset" << std::endl;
}

