/*
 * Matthew Diamond 2015
 * Member functions for the Filter class. Filtering functionality adapted from
 * the math on the following webpage:
 * http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
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
#include "Modules/Filter.hpp"

// Included graphics classes
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

/***************************
 * FILTER MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Filter::Filter() :
    Module(FILTER),
    iir_samples(std::vector<float>(6, 0)),
    iir_coefficients(std::vector<float>(6, 1 / 16.0)),
    y1(0), y2(0), x1(0), x2(0),
    filter_type(LOWPASS), lowpass_on(true), bandpass_on(false),
    highpass_on(false)
{
    inputs[FILTER_FREQUENCY_CUTOFF].val = 12500;
    inputs[FILTER_Q].val = 1;
}

/*
 * Destructor.
 */
Filter::~Filter()
{}

/*
 * Process all dependencies, then fill the output buffer with a waveform given
 * the data contained within this class and the audio device information.
 */
void Filter::process()
{
    // Process any dependencies
    process_dependencies();

    // Update parameters
    update_input_vals(0);

    // Once per buffer, update the coefficients
    double w0 = (inputs[FILTER_FREQUENCY_CUTOFF].val / SAMPLE_RATE) * 2 * M_PI;
    double alpha = sin(w0) / (2 * inputs[FILTER_Q].val);

    // Use different calculations for the coefficients of each filter type
    switch(filter_type)
    {
    case LOWPASS:
        iir_coefficients[0] = (1 - cos(w0)) / 2;
        iir_coefficients[1] = 1 - cos(w0);
        iir_coefficients[2] = (1 - cos(w0)) / 2;
        iir_coefficients[3] = 1 + alpha;
        iir_coefficients[4] = -2 * cos(w0);
        iir_coefficients[5] = 1 - alpha;
        break;
    case BANDPASS:
        iir_coefficients[0] = inputs[FILTER_Q].val * alpha;
        iir_coefficients[1] = 0;
        iir_coefficients[2] = (inputs[FILTER_Q].val * -1) * alpha;
        iir_coefficients[3] = 1 + alpha;
        iir_coefficients[4] = -2 * cos(w0);
        iir_coefficients[5] = 1 - alpha;
        break;
    case HIGHPASS:
        iir_coefficients[0] = (1 + cos(w0)) / 2;
        iir_coefficients[1] = (1 + cos(w0)) * -1;
        iir_coefficients[2] = (1 + cos(w0)) / 2;
        iir_coefficients[3] = 1 + alpha;
        iir_coefficients[4] = -2 * cos(w0);
        iir_coefficients[5] = 1 - alpha;
        break;
    }

    // Filter the buffer with the determined coefficients
    for(unsigned int i = 0; i < BUFFER_SIZE; i ++)
    {
        // Update parameters
        update_input_vals(i);

        if(!inputs[FILTER_SIGNAL].live)
        {
            inputs[FILTER_SIGNAL].val = 0;
        }

        out[i] = (iir_coefficients[0] / iir_coefficients[3])
                    * inputs[FILTER_SIGNAL].val
                    + (iir_coefficients[1] / iir_coefficients[3] * x1)
                    + (iir_coefficients[2] / iir_coefficients[3] * x2)
                    - (iir_coefficients[4] / iir_coefficients[3] * y1)
                    - (iir_coefficients[5] / iir_coefficients[3] * y2);

        x2 = x1;
        x1 = inputs[FILTER_SIGNAL].val;
        y2 = y1;
        y1 = out[i];
    }

    processed = true;
}

/*
 * Handle user interactions with graphics objects. First call the module class
 * version of this function to handle events that might happen to any module.
 * If nothing happens in the module class version of the function, then handle
 * events specific to this module type here.
 */
bool Filter::handle_event(Graphics_Object *g)
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
    // Handle a click of the lowpass toggle button
    else if(g == graphics_objects["lowpass toggle button"])
    {
        switch_filter(LOWPASS);
        return true;
    }
    // Handle a click of the bandpass toggle button
    else if(g == graphics_objects["bandpass toggle button"])
    {
        switch_filter(BANDPASS);
        return true;
    }
    // Handle a click of the highpass toggle button
    else if(g == graphics_objects["highpass toggle button"])
    {
        switch_filter(HIGHPASS);
        return true;
    }

    // If none of the above happen, return false
    return false;
}

/*
 * Calculate the locations of graphics objects unique to this module type, add
 * them to the map of graphics object locations.
 */
void Filter::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Waveform viewer location
    location = {upper_left.x, upper_left.y + 15, MODULE_WIDTH, 54};
    graphics_object_locations["waveform"] = location;

    // Signal input related graphics object locations
    location = {upper_left.x + 2, location.y + 57, 0, 0};
    graphics_object_locations["signal text"] = location;
    location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
    graphics_object_locations["signal text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["signal toggle button"] = location;

    // Frequency cutoff/q related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["frequency cutoff/q text"] = location;
    location = {upper_left.x, location.y + 10, (MODULE_WIDTH / 2) - 9, 9};
    graphics_object_locations["frequency cutoff text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["frequency cutoff toggle button"] = location;
    location = {location.x + location.w + 1, location.y,
                (MODULE_WIDTH / 2) - 8, 9};
    graphics_object_locations["q text box"] = location;
    location = {location.x + location.w + 1, location.y, 7, 9};
    graphics_object_locations["q toggle button"] = location;

    // Filter type related graphics object locations
    location = {upper_left.x + 2, location.y + 10, 0, 0};
    graphics_object_locations["filter type text"] = location;
    location = {upper_left.x, location.y + 10, (MODULE_WIDTH / 3) - 1, 9};
    graphics_object_locations["lowpass toggle button"] = location;
    location = {location.x + location.w + 1, location.y, location.w, 9};
    graphics_object_locations["bandpass toggle button"] = location;
    location = {location.x + location.w + 1, location.y, location.w, 9};
    graphics_object_locations["highpass toggle button"] = location;
}

/*
 * Initialize graphics objects unique to this module type, add them to the
 * map of graphics objects.
 */
void Filter::initialize_unique_graphics_objects()
{
    // Initialize text objects
    graphics_objects["signal text"] =
        new Text(name + " signal text",
                 graphics_object_locations["signal text"],
                 secondary_module_color, "INPUT SIGNAL:");
    graphics_objects["frequency cutoff/q text"] =
        new Text(name + " frequency cutoff/q text",
                 graphics_object_locations["frequency cutoff/q text"],
                 secondary_module_color, "CUTOFF (Hz) & Q (#):");
    graphics_objects["filter type text"] =
        new Text(name + " filter type text",
                 graphics_object_locations["filter type text"],
                 secondary_module_color, "FILTER TYPE:");

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
                           "input", this, FILTER_SIGNAL, NULL);
    graphics_objects["frequency cutoff text box"] =
        new Input_Text_Box(name + " frequency cutoff text box",
                           graphics_object_locations["frequency cutoff text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, FILTER_FREQUENCY_CUTOFF, NULL);
    graphics_objects["q text box"] =
        new Input_Text_Box(name + " q text box",
                           graphics_object_locations["q text box"],
                           secondary_module_color, primary_module_color,
                           "# or input", this, FILTER_Q, NULL);

    // Initialize input toggle buttons
    graphics_objects["signal toggle button"] =
        new Input_Toggle_Button(name + " signal toggle button",
                                graphics_object_locations["signal toggle button"],
                                secondary_module_color, primary_module_color,
                                this, FILTER_SIGNAL,
                                (Input_Text_Box *) graphics_objects["signal text box"]);
    graphics_objects["frequency cutoff toggle button"] =
        new Input_Toggle_Button(name + " frequency cutoff toggle button",
                                graphics_object_locations["frequency cutoff toggle button"],
                                secondary_module_color, primary_module_color,
                                this, FILTER_FREQUENCY_CUTOFF,
                                (Input_Text_Box *) graphics_objects["frequency cutoff text box"]);
    graphics_objects["q toggle button"] =
        new Input_Toggle_Button(name + " q toggle button",
                                graphics_object_locations["q toggle button"],
                                secondary_module_color, primary_module_color,
                                this, FILTER_Q,
                                (Input_Text_Box *) graphics_objects["q text box"]);

    // Point input text boxes to their associated input toggle buttons
    ((Input_Text_Box *)
     graphics_objects["signal text box"])->input_toggle_button =
        (Input_Toggle_Button *) graphics_objects["signal toggle button"];
    ((Input_Text_Box *)
     graphics_objects["frequency cutoff text box"])->input_toggle_button =
        (Input_Toggle_Button *)
        graphics_objects["frequency cutoff toggle button"];
    ((Input_Text_Box *)
     graphics_objects["q text box"])->input_toggle_button =
        (Input_Toggle_Button *) graphics_objects["q toggle button"];

    // Initialize filter type toggle buttons
    graphics_objects["lowpass toggle button"] =
        new Toggle_Button(name + " lowpass toggle button",
                          graphics_object_locations["lowpass toggle button"],
                          secondary_module_color, primary_module_color,
                          primary_module_color, secondary_module_color,
                          FONT, "LP", "LP", lowpass_on, this);
    graphics_objects["bandpass toggle button"] =
        new Toggle_Button(name + " bandpass toggle button",
                          graphics_object_locations["bandpass toggle button"],
                          secondary_module_color, primary_module_color,
                          primary_module_color, secondary_module_color,
                          FONT, "BP", "BP", bandpass_on, this);
    graphics_objects["highpass toggle button"] =
        new Toggle_Button(name + " highpass toggle button",
                          graphics_object_locations["highpass toggle button"],
                          secondary_module_color, primary_module_color,
                          primary_module_color, secondary_module_color,
                          FONT, "HP", "HP", highpass_on, this);
}

/*
 * Switch to outputting the given waveform type.
 */
void Filter::switch_filter(FilterType filter_type_)
{
    lowpass_on = false;
    bandpass_on = false;
    highpass_on = false;
    ((Toggle_Button *) graphics_objects["lowpass toggle button"])->b = false;
    ((Toggle_Button *) graphics_objects["bandpass toggle button"])->b = false;
    ((Toggle_Button *) graphics_objects["highpass toggle button"])->b = false;

    switch(filter_type_)
    {
    case LOWPASS:
        lowpass_on = true;
        ((Toggle_Button *) graphics_objects["lowpass toggle button"])->b = true;
        std::cout << name << " is now a low pass filter" << std::endl;
        break;
    case BANDPASS:
        bandpass_on = true;
        ((Toggle_Button *) graphics_objects["bandpass toggle button"])->b = true;
        std::cout << name << " is now a bandpass filter" << std::endl;
        break;
    case HIGHPASS:
        highpass_on = true;
        ((Toggle_Button *) graphics_objects["highpass toggle button"])->b = true;
        std::cout << name << " is now a highpass filter" << std::endl;
        break;
    }

    filter_type = filter_type_;
}

std::string Filter::get_unique_text_representation()
{
    return "";
}

