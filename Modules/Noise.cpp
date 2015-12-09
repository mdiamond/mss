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
#include "Noise.hpp"
#include "../Graphics_Objects/Button.hpp"
#include "../Graphics_Objects/Input_Text_Box.hpp"
#include "../Graphics_Objects/Input_Toggle_Button.hpp"
#include "../Graphics_Objects/Text.hpp"
#include "../Graphics_Objects/Toggle_Button.hpp"
#include "../Graphics_Objects/Waveform.hpp"

/**************************
 * NOISE MEMBER FUNCTIONS *
 **************************/

/*
 * Constructor.
 */
Noise::Noise() :
    Module(NOISE)
{
    noise_type = 1;
}

/*
 * Destructor.
 */
Noise::~Noise()
{

}

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
    for(unsigned short i = 0; i < output.size(); i ++)
    {
        output[i] = produce_white_noise_sample();
    }

    processed = true;
}

/*
 * Update parameters at the k rate.
 */
void Noise::update_control_values()
{
    
}

/*
 * Calculate the locations of graphics objects unique to this module type.
 */
void Noise::calculate_unique_graphics_object_locations()
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
    h_waveform = 167;
    y3 = upper_left.y + 23;
    y4 = y3 + 46;
    y5 = y4 + 15;   
    y6 = y5 + 16;
    y7 = y6 + 15;
    y8 = y7 + 16;
    y9 = y8 + 15;
    y10 = y9 + 16;
    y11 = y10 + 15;
    y12 = y11 + 27;
    x_range_high = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_range = (MODULE_WIDTH / 2) - 11;
    x_range_low_input_toggle_button = x_text_box + w_range + 1;
    w_wave_selector = ((MODULE_WIDTH) / 4) - 1;

    graphics_object_locations.push_back({x_text_box, y3, w_waveform, h_waveform});
    // graphics_object_locations.push_back({x_text_box, y12, w_wave_selector, h_text_box});
    // graphics_object_locations.push_back({x_text_box + w_wave_selector + 2, y12, w_wave_selector, h_text_box});
    // graphics_object_locations.push_back({x_text_box + ((w_wave_selector + 2) * 2), y12, w_wave_selector, h_text_box});
    // graphics_object_locations.push_back({x_text_box + ((w_wave_selector + 2) * 3), y12, w_wave_selector, h_text_box});
}

/*
 * Initialize all graphics objects unique to this module type, and add them to the array
 * of graphics objects.
 */
void Noise::initialize_unique_graphics_objects()
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

    names = {name + " waveform visualizer (waveform)"};
    locations = {graphics_object_locations[NOISE_OUTPUT_WAVEFORM]};
    colors = {&color};
    background_colors = {&text_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&output};

    tmp_graphics_objects = initialize_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    // names = {name + " sin toggle (toggle button)", name + " tri toggle (toggle button)",
    //          name + " saw toggle (toggle button)", name + " sqr toggle (toggle button)"};
    // locations = {graphics_object_locations[OSCILLATOR_SIN_WAVE_TOGGLE_BUTTON],
    //              graphics_object_locations[OSCILLATOR_TRI_WAVE_TOGGLE_BUTTON],
    //              graphics_object_locations[OSCILLATOR_SAW_WAVE_TOGGLE_BUTTON],
    //              graphics_object_locations[OSCILLATOR_SQR_WAVE_TOGGLE_BUTTON]};
    // colors = std::vector<SDL_Color *>(4, &text_color);
    // color_offs = std::vector<SDL_Color *>(4, &color);
    // text_color_ons = std::vector<SDL_Color *>(4, &color);
    // text_color_offs = std::vector<SDL_Color *>(4, &text_color);
    // fonts = std::vector<TTF_Font *>(4, FONT_REGULAR);
    // texts = {"SIN", "TRI", "SAW", "SQR"};
    // text_offs = {"SIN", "TRI", "SAW", "SQR"};
    // bs = {sin_on, tri_on, saw_on, sqr_on};
    // parents = std::vector<Module *>(4, this);

    // tmp_graphics_objects = initialize_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
    //                                                     text_color_offs, fonts, texts, text_offs, bs, parents);
    // graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());
}

/*
 * Switch to outputting the given noise type.
 */
void Noise::switch_noise_type(int _noise_type)
{
    // sin_on = false;
    // tri_on = false;
    // saw_on = false;
    // sqr_on = false;
    // ((Toggle_Button *) graphics_objects[OSCILLATOR_SIN_WAVE_TOGGLE_BUTTON])->b = false;
    // ((Toggle_Button *) graphics_objects[OSCILLATOR_TRI_WAVE_TOGGLE_BUTTON])->b = false;
    // ((Toggle_Button *) graphics_objects[OSCILLATOR_SAW_WAVE_TOGGLE_BUTTON])->b = false;
    // ((Toggle_Button *) graphics_objects[OSCILLATOR_SQR_WAVE_TOGGLE_BUTTON])->b = false;

    // if(_waveform_type == SIN)
    // {
    //     waveform_type = SIN;
    //     sin_on = true;
    //     ((Toggle_Button *) graphics_objects[OSCILLATOR_SIN_WAVE_TOGGLE_BUTTON])->b = true;
    //     std::cout << name << " is now outputting a sine wave" << std::endl;
    // }
    // else if(_waveform_type == TRI)
    // {
    //     waveform_type = TRI;
    //     tri_on = true;
    //     ((Toggle_Button *) graphics_objects[OSCILLATOR_TRI_WAVE_TOGGLE_BUTTON])->b = true;
    //     std::cout << name << " is now outputting a triangle wave" << std::endl;
    // }
    // else if(_waveform_type == SAW)
    // {
    //     waveform_type = SAW;
    //     saw_on = true;
    //     ((Toggle_Button *) graphics_objects[OSCILLATOR_SAW_WAVE_TOGGLE_BUTTON])->b = true;
    //     std::cout << name << " is now outputting a sawtooth wave" << std::endl;
    // }
    // else
    // {
    //     waveform_type = SQR;
    //     sqr_on = true;
    //     ((Toggle_Button *) graphics_objects[OSCILLATOR_SQR_WAVE_TOGGLE_BUTTON])->b = true;
    //     std::cout << name << " is now outputting a square wave" << std::endl;
    // }

    // waveform_type = _waveform_type;
}

std::string Noise::get_unique_text_representation()
{
    return "";
}
