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
#include "function_forwarder.hpp"
#include "graphics_object_utils.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included modules classes
#include "Module.hpp"
#include "Modules/Noise.hpp"

// Included graphics classes
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

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
    process_dependencies();

    for(unsigned short i = 0; i < output.size(); i ++)
    {
        update_input_vals(i);

        output[i] = produce_white_noise_sample();
        output[i] = scale_sample(output[i], -1, 1, inputs[NOISE_RANGE_LOW].val, inputs[NOISE_RANGE_HIGH].val);
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
    h_waveform = 138;
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
    graphics_object_locations.push_back({x_text, y10, 0, 0});
    graphics_object_locations.push_back({x_text_box, y11, w_range, h_text_box});
    graphics_object_locations.push_back({x_range_high, y11, w_range - 1, h_text_box});
    graphics_object_locations.push_back({x_range_low_input_toggle_button, y11, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y11, w_input_toggle_button, h_text_box});
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
    std::vector<Input_Text_Box *> input_text_boxes;
    std::vector<Input_Toggle_Button *> input_toggle_buttons;

    std::vector<Graphics_Object *> tmp_graphics_objects;

    names = {name + " waveform visualizer (waveform)"};
    locations = {graphics_object_locations[NOISE_OUTPUT_WAVEFORM]};
    colors = {&primary_module_color};
    background_colors = {&secondary_module_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&output};

    tmp_graphics_objects = initialize_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {name + " range (text)"};
    locations = {graphics_object_locations[NOISE_RANGE_TEXT]};
    colors = std::vector<SDL_Color *>(1, &secondary_module_color);
    texts = {"RANGE LOW & HIGH:"};
    fonts = std::vector<TTF_Font *>(1, FONT_REGULAR);

    tmp_graphics_objects = initialize_text_objects(names, locations, colors, texts, fonts);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {name + " range low (input text box)", name + " range high (input text box)"};
    locations = {graphics_object_locations[NOISE_RANGE_LOW_INPUT_TEXT_BOX],
                 graphics_object_locations[NOISE_RANGE_HIGH_INPUT_TEXT_BOX]};
    colors = std::vector<SDL_Color *>(2, &secondary_module_color);
    text_colors = std::vector<SDL_Color *>(2, &primary_module_color);
    prompt_texts = std::vector<std::string>(2, "# or input");
    fonts = std::vector<TTF_Font *>(2, FONT_SMALL);
    parents = std::vector<Module *>(2, this);
    input_nums = {NOISE_RANGE_LOW, NOISE_RANGE_HIGH};
    input_toggle_buttons = std::vector<Input_Toggle_Button *>(2, NULL);

    initialize_input_text_box_objects(names, locations, colors, text_colors, prompt_texts, fonts, parents, input_nums, input_toggle_buttons);

    names = {name + " range low (input toggle button)", name + " range high (input toggle button)"};
    locations = {graphics_object_locations[NOISE_RANGE_LOW_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[NOISE_RANGE_HIGH_INPUT_TOGGLE_BUTTON]};
    colors = std::vector<SDL_Color *>(2, &RED);
    color_offs = std::vector<SDL_Color *>(2, &secondary_module_color);
    text_color_ons = std::vector<SDL_Color *>(2, &WHITE);
    text_color_offs = std::vector<SDL_Color *>(2, &primary_module_color);
    fonts = std::vector<TTF_Font *>(2, FONT_SMALL);
    texts = std::vector<std::string>(2, "I");
    text_offs = texts;
    bs = std::vector<bool>(2, false);
    parents = std::vector<Module *>(2, this);
    input_nums = {NOISE_RANGE_LOW, NOISE_RANGE_HIGH};

    input_text_boxes = {(Input_Text_Box *) graphics_objects[NOISE_RANGE_LOW_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[NOISE_RANGE_HIGH_INPUT_TEXT_BOX]};

    initialize_input_toggle_button_objects(names, locations, colors, color_offs,
                                           text_color_ons, text_color_offs,
                                           fonts, texts, text_offs, bs, parents,
                                           input_nums, input_text_boxes);

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

    ((Input_Text_Box *) graphics_objects[NOISE_RANGE_LOW_INPUT_TEXT_BOX])->input_toggle_button = (Input_Toggle_Button *) graphics_objects[NOISE_RANGE_LOW_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *) graphics_objects[NOISE_RANGE_HIGH_INPUT_TEXT_BOX])->input_toggle_button = (Input_Toggle_Button *) graphics_objects[NOISE_RANGE_HIGH_INPUT_TOGGLE_BUTTON];
}

std::string Noise::get_unique_text_representation()
{
    return "";
}

/*
 * Handle button presses. Noise button presses are used to remove the module.
 */
void Noise::button_function(Button *button)
{
    if(button == graphics_objects[MODULE_REMOVE_MODULE_BUTTON])
        delete this;
}

/*
 * Noise has no toggle buttons. This is a dummy function.
 */
void Noise::toggle_button_function(Toggle_Button *toggle_button)
{

}
