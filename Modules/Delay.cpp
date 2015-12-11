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
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "../function_forwarder.hpp"
#include "../graphics_object_utils.hpp"
#include "../image_processing.hpp"
#include "../main.hpp"
#include "../signal_processing.hpp"

// Included classes
#include "../Module.hpp"
#include "Delay.hpp"
#include "../Graphics_Objects/Button.hpp"
#include "../Graphics_Objects/Input_Text_Box.hpp"
#include "../Graphics_Objects/Input_Toggle_Button.hpp"
#include "../Graphics_Objects/Text.hpp"
#include "../Graphics_Objects/Toggle_Button.hpp"
#include "../Graphics_Objects/Waveform.hpp"

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
    input_floats[DELAY_MAX_DELAY_TIME] = 5000;
    input_floats[DELAY_DELAY_TIME] = 1000;
    input_floats[DELAY_FEEDBACK_AMOUNT] = 0;
    input_floats[DELAY_WET_DRY] = 1;

    circular_buffer = std::vector<float>(input_floats[DELAY_MAX_DELAY_TIME] / 1000.0 * SAMPLE_RATE, 0);
    delay_samples = input_floats[DELAY_DELAY_TIME] / 1000.0 * SAMPLE_RATE;
}

/*
 * Destructor.
 */
Delay::~Delay()
{

}

/*
 * Calculate a linearly interpolated sample from the buffer to return.
 * This sample may technically be in-between samples.
 */
float Delay::calculate_wet_sample()
{
    float x0, x1, y0, y1, interpolated_sample;
    float delayed_sample = current_sample - delay_samples;
    while(delayed_sample < 0)
        delayed_sample += circular_buffer.size();

    x0 = ((int) delayed_sample) % circular_buffer.size();
    x1 = ((int) (delayed_sample + 1)) % circular_buffer.size();
    y0 = circular_buffer[x0];
    y1 = circular_buffer[x1];

    interpolated_sample = y0 + ((y1 - y0) * ((delayed_sample - x0) / (x1 - x0)));

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

    // Per sample
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        for(unsigned int j = 1; j < dependencies.size(); j ++)
            if(inputs_live[j])
                input_floats[j] = inputs[j]->at(i);

        if(input_floats[DELAY_MAX_DELAY_TIME] != previous_max_delay_time)
        {
            reset_buffer();
            previous_max_delay_time = input_floats[DELAY_MAX_DELAY_TIME];
        }

        if(input_floats[DELAY_DELAY_TIME] != previous_delay_time)
        {
            delay_samples = input_floats[DELAY_DELAY_TIME] / 1000.0 * SAMPLE_RATE;
            previous_delay_time = input_floats[DELAY_DELAY_TIME];
        }

        if(inputs_live[DELAY_SIGNAL] && input_floats[DELAY_MAX_DELAY_TIME] >= input_floats[DELAY_DELAY_TIME])
        {
            // Apply the dry signal
            output[i] = (1 - input_floats[DELAY_WET_DRY]) * inputs[DELAY_SIGNAL]->at(i);

            // Apply the linearly interpolated wet signal
            float wet_sample = calculate_wet_sample();
            output[i] += input_floats[DELAY_WET_DRY] * wet_sample;

            // Update the sample in the circular buffer
            circular_buffer[current_sample] = input_floats[DELAY_FEEDBACK_AMOUNT] * wet_sample;
            circular_buffer[current_sample] += inputs[DELAY_SIGNAL]->at(i);

            // Move on to the next sample
            current_sample ++;
            current_sample = fmod(current_sample, ((double) circular_buffer.size()));
        }
        else if(input_floats[DELAY_MAX_DELAY_TIME] < input_floats[DELAY_DELAY_TIME])
            std::cout << RED_STDOUT << name << " delay time is greater than max delay time!" << DEFAULT_STDOUT << std::endl;
    }

    processed = true;
}

/*
 * Update parameters at the k rate.
 */
void Delay::update_control_values()
{

}

/*
 * Calculate the locations of graphics objects unique to this module type.
 */
void Delay::calculate_unique_graphics_object_locations()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        x_input_toggle_button, w_input_toggle_button,
        w_waveform, h_waveform,
        x_signal_cv, x_signal_input_toggle_button, w_signals,
        y3, y4, y5, y6, y7, y8, y9;

    x_text = upper_left.x + 2;
    x_text_box = upper_left.x;
    w_text_box = MODULE_WIDTH - 11;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box + 1;
    w_input_toggle_button = 10;
    w_waveform = MODULE_WIDTH;
    h_waveform = 104;
    y3 = upper_left.y + 23;
    y4 = y3 + 106;
    y5 = y4 + 15;
    y6 = y5 + 15;
    y7 = y6 + 15;
    y8 = y7 + 15;
    y9 = y8 + 15;
    x_signal_cv = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_signals = (MODULE_WIDTH / 2) - 11;
    x_signal_input_toggle_button = x_text_box + w_signals + 1;

    graphics_object_locations.push_back({upper_left.x + MODULE_WIDTH - 19,
                                         upper_left.y, 9, 15});
    graphics_object_locations.push_back({x_text, y4, 0, 0});
    graphics_object_locations.push_back({x_text, y6, 0, 0});
    graphics_object_locations.push_back({x_text, y8, 0, 0});
    graphics_object_locations.push_back({x_text_box, y3, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y5, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y7, w_signals, h_text_box});
    graphics_object_locations.push_back({x_signal_cv, y7, w_signals - 1, h_text_box});
    graphics_object_locations.push_back({x_text_box, y9, w_signals, h_text_box});
    graphics_object_locations.push_back({x_signal_cv, y9, w_signals - 1, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_signal_input_toggle_button, y7, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y7, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_signal_input_toggle_button, y9, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y9, w_input_toggle_button, h_text_box});
}

/*
 * Initialize all graphics objects unique to this module type, and add them to the array
 * of graphics objects.
 */
void Delay::initialize_unique_graphics_objects()
{
    std::vector<std::string> names, texts, prompt_texts, text_offs;
    std::vector<SDL_Rect> locations;
    std::vector<SDL_Color *> colors, background_colors, color_offs, text_colors, text_color_ons, text_color_offs;
    std::vector<TTF_Font *> fonts;
    std::vector<float> range_lows, range_highs;
    std::vector<int> input_nums;
    std::vector<std::vector<float> *> buffers;
    std::vector<Module *> parents;
    std::vector<bool> bs;

    std::vector<Graphics_Object *> tmp_graphics_objects;

    Button *button;
    button = new Button(name + " reset buffer (button)",
                    graphics_object_locations[DELAY_RESET_BUFFER_BUTTON],
                    &text_color, &color, "0", this);
    graphics_objects.push_back(button);

    names = {name + " signal (text)", name + " max delay time and delay time (text)",
             name + " feedback amount and wet/dry (text)"};
    locations = {graphics_object_locations[DELAY_SIGNAL_TEXT],
                 graphics_object_locations[DELAY_MAX_DELAY_TIME_AND_DELAY_TIME_TEXT],
                 graphics_object_locations[DELAY_FEEDBACK_AMOUNT_AND_WET_DRY_TEXT]};
    colors = std::vector<SDL_Color *>(3, &text_color);
    texts = {"SIGNAL INPUT:", "MAX DELAY & DELAY:", "FEEDBACK & WET/DRY:"};
    fonts = std::vector<TTF_Font *>(3, FONT_REGULAR);

    tmp_graphics_objects = initialize_text_objects(names, locations, colors, texts, fonts);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {name + " waveform visualizer (waveform)"};
    locations = {graphics_object_locations[DELAY_OUTPUT_WAVEFORM]};
    colors = {&color};
    background_colors = {&text_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&output};

    tmp_graphics_objects = initialize_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {name + " signal (input text box)", name + " max delay time (input text box)",
             name + " delay time (input text box)", name + " feedback amount (input text box)",
             name + " wet/dry (input text box)"};
    locations = {graphics_object_locations[DELAY_SIGNAL_INPUT_TEXT_BOX],
                 graphics_object_locations[DELAY_MAX_DELAY_TIME_INPUT_TEXT_BOX],
                 graphics_object_locations[DELAY_DELAY_TIME_INPUT_TEXT_BOX],
                 graphics_object_locations[DELAY_FEEDBACK_AMOUNT_INPUT_TEXT_BOX],
                 graphics_object_locations[DELAY_WET_DRY_INPUT_TEXT_BOX]};
    colors = std::vector<SDL_Color *>(5, &text_color);
    text_colors = std::vector<SDL_Color *>(5, &color);
    prompt_texts = std::vector<std::string>(5, "# or input");
    fonts = std::vector<TTF_Font *>(5, FONT_SMALL);
    parents = std::vector<Module *>(5, this);
    input_nums = {DELAY_SIGNAL, DELAY_MAX_DELAY_TIME,
                  DELAY_DELAY_TIME, DELAY_FEEDBACK_AMOUNT, DELAY_WET_DRY};

    initialize_input_text_box_objects(names, locations, colors, text_colors, prompt_texts, fonts, parents, input_nums);

    names = {name + " signal (input toggle button)",
             name + " max delay time (input toggle button)",
             name + " delay time (input toggle button)",
             name + " feedback amount (input toggle button)",
             name + " wet/dry (input toggle button)"};
    locations = {graphics_object_locations[DELAY_SIGNAL_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[DELAY_MAX_DELAY_TIME_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[DELAY_DELAY_TIME_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[DELAY_FEEDBACK_AMOUNT_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[DELAY_WET_DRY_INPUT_TOGGLE_BUTTON]};
    colors = std::vector<SDL_Color *>(5, &RED);
    color_offs = std::vector<SDL_Color *>(5, &text_color);
    text_color_ons = std::vector<SDL_Color *>(5, &WHITE);
    text_color_offs = std::vector<SDL_Color *>(5, &color);
    fonts = std::vector<TTF_Font *>(5, FONT_SMALL);
    texts = std::vector<std::string>(5, "I");
    text_offs = texts;
    bs = std::vector<bool>(5, false);
    parents = std::vector<Module *>(5, this);
    input_nums = {DELAY_SIGNAL, DELAY_MAX_DELAY_TIME,
                  DELAY_DELAY_TIME, DELAY_FEEDBACK_AMOUNT, DELAY_WET_DRY};

    initialize_input_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                       text_color_offs, fonts, texts, text_offs, bs, parents, input_nums);
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
    circular_buffer = std::vector<float>(input_floats[DELAY_MAX_DELAY_TIME] / 1000.0 * SAMPLE_RATE, 0);

    std::cout << name << " buffer reset" << std::endl;
}
