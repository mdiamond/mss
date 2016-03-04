/*
 * Matthew Diamond 2015
 * Member functions for the Filter class.
 * Filtering functionality adapted from the math on
 * the following webpage:
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
#include "Filter.hpp"
#include "../Graphics_Objects/Button.hpp"
#include "../Graphics_Objects/Input_Text_Box.hpp"
#include "../Graphics_Objects/Input_Toggle_Button.hpp"
#include "../Graphics_Objects/Text.hpp"
#include "../Graphics_Objects/Toggle_Button.hpp"
#include "../Graphics_Objects/Waveform.hpp"

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
    input_floats[FILTER_FREQUENCY_CUTOFF] = 12500;
    input_floats[FILTER_Q] = 1;
}

/*
 * Destructor.
 */
Filter::~Filter()
{

}

/*
 * Process all dependencies, then
 * fill the output buffer with a waveform given
 * the data contained within this class and the
 * audio device information.
 */
void Filter::process()
{
    // Process any dependencies
    process_dependencies();

    if(inputs_live[FILTER_SIGNAL])
    {
        for(unsigned int j = 0; j < dependencies.size(); j ++)
            if(inputs_live[j])
                input_floats[j] = inputs[j]->at(0);

        double w0 = (input_floats[FILTER_FREQUENCY_CUTOFF] / SAMPLE_RATE) * 2 * M_PI;
        double alpha = sin(w0) / (2 * input_floats[FILTER_Q]);

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
                iir_coefficients[0] = input_floats[FILTER_Q] * alpha;
                iir_coefficients[1] = 0;
                iir_coefficients[2] = (input_floats[FILTER_Q] * -1) * alpha;
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

        for(int i = 0; i < BUFFER_SIZE; i ++)
        {
            output[i] = (iir_coefficients[0] / iir_coefficients[3]) * inputs[FILTER_SIGNAL]->at(i)
                        + (iir_coefficients[1] / iir_coefficients[3] * x1)
                        + (iir_coefficients[2] / iir_coefficients[3] * x2)
                        - (iir_coefficients[4] / iir_coefficients[3] * y1)
                        - (iir_coefficients[5] / iir_coefficients[3] * y2);

            x2 = x1;
            x1 = inputs[FILTER_SIGNAL]->at(i);
            y2 = y1;
            y1 = output[i];
        }
    }

    else
    for(int i = 0; i < BUFFER_SIZE; i ++)
        output[i] = 0;

    processed = true;
}

/*
 * Update parameters at the k rate.
 */
void Filter::update_control_values()
{

}

/*
 * Calculate the locations of graphics objects unique to this module type.
 */
void Filter::calculate_unique_graphics_object_locations()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        x_input_toggle_button, w_input_toggle_button,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10, y11, y12,
        x_range_high, x_range_low_input_toggle_button, w_range,
        w_wave_selector;

    x_text = upper_left.x + 2;
    x_text_box = upper_left.x;
    w_text_box = MODULE_WIDTH - 11;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box + 1;
    w_input_toggle_button = 10;
    w_waveform = MODULE_WIDTH;
    h_waveform = 104;
    y3 = upper_left.y + 23;
    y4 = y3 + 46;
    y5 = y4 + 15;
    y6 = y5 + 16;
    y7 = y6 + 13;
    y8 = y7 + 16;
    y9 = y8 + 15;
    y10 = y9 + 15;
    y11 = y10 + 15;
    y12 = y11 + 27;
    x_range_high = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_range = (MODULE_WIDTH / 2) - 11;
    x_range_low_input_toggle_button = x_text_box + w_range + 1;
    w_wave_selector = ((MODULE_WIDTH) / 3) - 1;

    graphics_object_locations.push_back({x_text, y8, 0, 0});
    graphics_object_locations.push_back({x_text, y10, 0, 0});
    graphics_object_locations.push_back({x_text_box, y3, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y9, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y11, w_range, h_text_box});
    graphics_object_locations.push_back({x_range_high, y11, w_range - 1, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y9, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_range_low_input_toggle_button, y11, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y11, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_text_box, y12, w_wave_selector, h_text_box});
    graphics_object_locations.push_back({x_text_box + w_wave_selector + 2, y12, w_wave_selector, h_text_box});
    graphics_object_locations.push_back({x_text_box + ((w_wave_selector + 2) * 2), y12, w_wave_selector - 1, h_text_box});
}

/*
 * Initialize all graphics objects unique to this module type, and add them to the array
 * of graphics objects.
 */
void Filter::initialize_unique_graphics_objects()
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

    names = {name + " signal (text)", name + " frequency cutoff and q (text)"};
    locations = {graphics_object_locations[FILTER_SIGNAL_TEXT],
                 graphics_object_locations[FILTER_FREQUENCY_CUTOFF_AND_Q_TEXT]};
    colors = std::vector<SDL_Color *>(2, &text_color);
    texts = {"INPUT SIGNAL:", "CUTOFF & Q:"};
    fonts = std::vector<TTF_Font *>(2, FONT_REGULAR);

    tmp_graphics_objects = initialize_text_objects(names, locations, colors, texts, fonts);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {name + " waveform visualizer (waveform)"};
    locations = {graphics_object_locations[FILTER_OUTPUT_WAVEFORM]};
    colors = {&color};
    background_colors = {&text_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&output};

    tmp_graphics_objects = initialize_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {name + " signal (input text box)", name + " frequency cutoff (input text box)",
             name + " q (input text box)"};
    locations = {graphics_object_locations[FILTER_SIGNAL_INPUT_TEXT_BOX],
                 graphics_object_locations[FILTER_FREQUENCY_CUTOFF_INPUT_TEXT_BOX],
                 graphics_object_locations[FILTER_Q_INPUT_TEXT_BOX]};
    colors = std::vector<SDL_Color *>(3, &text_color);
    text_colors = std::vector<SDL_Color *>(3, &color);
    prompt_texts = std::vector<std::string>(3, "# or input");
    prompt_texts[0] = "input";
    fonts = std::vector<TTF_Font *>(3, FONT_SMALL);
    parents = std::vector<Module *>(3, this);
    input_nums = {FILTER_SIGNAL, FILTER_FREQUENCY_CUTOFF, FILTER_Q};

    initialize_input_text_box_objects(names, locations, colors, text_colors, prompt_texts, fonts, parents, input_nums);

    names = {name + " signal (input toggle button)",
             name + " frequency cutoff (input toggle button)",
             name + " q (input toggle button)"};
    locations = {graphics_object_locations[FILTER_SIGNAL_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[FILTER_FREQUENCY_CUTOFF_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[FILTER_Q_INPUT_TOGGLE_BUTTON]};
    colors = std::vector<SDL_Color *>(3, &RED);
    color_offs = std::vector<SDL_Color *>(3, &text_color);
    text_color_ons = std::vector<SDL_Color *>(3, &WHITE);
    text_color_offs = std::vector<SDL_Color *>(3, &color);
    fonts = std::vector<TTF_Font *>(3, FONT_SMALL);
    texts = std::vector<std::string>(3, "I");
    text_offs = texts;
    bs = std::vector<bool>(3, false);
    parents = std::vector<Module *>(3, this);
    input_nums = {FILTER_SIGNAL, FILTER_FREQUENCY_CUTOFF, FILTER_Q};

    initialize_input_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                       text_color_offs, fonts, texts, text_offs, bs, parents, input_nums);

    names = {name + " lowpass toggle (toggle button)", name + " bandpass toggle (toggle button)",
             name + " highpass toggle (toggle button)"};
    locations = {graphics_object_locations[FILTER_LP_TOGGLE_BUTTON],
                 graphics_object_locations[FILTER_BP_TOGGLE_BUTTON],
                 graphics_object_locations[FILTER_HP_TOGGLE_BUTTON]};
    colors = std::vector<SDL_Color *>(3, &text_color);
    color_offs = std::vector<SDL_Color *>(3, &color);
    text_color_ons = std::vector<SDL_Color *>(3, &color);
    text_color_offs = std::vector<SDL_Color *>(3, &text_color);
    fonts = std::vector<TTF_Font *>(3, FONT_REGULAR);
    texts = {"LP", "BP", "HP"};
    text_offs = {"LP", "BP", "HP"};
    bs = {lowpass_on, bandpass_on, highpass_on};
    parents = std::vector<Module *>(3, this);

    tmp_graphics_objects = initialize_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                                        text_color_offs, fonts, texts, text_offs, bs, parents);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());
}

/*
 * Switch to outputting the given waveform type.
 */
void Filter::switch_filter(int _filter_type)
{
    lowpass_on = false;
    bandpass_on = false;
    highpass_on = false;
    ((Toggle_Button *) graphics_objects[FILTER_LP_TOGGLE_BUTTON])->b = false;
    ((Toggle_Button *) graphics_objects[FILTER_BP_TOGGLE_BUTTON])->b = false;
    ((Toggle_Button *) graphics_objects[FILTER_HP_TOGGLE_BUTTON])->b = false;

    if(_filter_type == LOWPASS)
    {
        lowpass_on = true;
        ((Toggle_Button *) graphics_objects[FILTER_LP_TOGGLE_BUTTON])->b = true;
        std::cout << name << " is now a low pass filter" << std::endl;
    }
    else if(_filter_type == BANDPASS)
    {
        bandpass_on = true;
        ((Toggle_Button *) graphics_objects[FILTER_BP_TOGGLE_BUTTON])->b = true;
        std::cout << name << " is now a bandpass filter" << std::endl;
    }
    else
    {
        highpass_on = true;
        ((Toggle_Button *) graphics_objects[FILTER_HP_TOGGLE_BUTTON])->b = true;
        std::cout << name << " is now a highpass filter" << std::endl;
    }

    filter_type = _filter_type;
}

std::string Filter::get_unique_text_representation()
{
    return "";
}
