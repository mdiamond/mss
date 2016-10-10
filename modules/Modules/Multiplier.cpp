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

// Included graphics classes
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

/*******************************
 * MULTIPLIER MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor.
 */
Multiplier::Multiplier() :
    Module(MULTIPLIER)
{
    // The signal input needs to be 0, while the others
    // need to be 1 to start out
    inputs[MULTIPLIER_SIGNAL].val = 0;
    inputs[MULTIPLIER_MULTIPLIER].val = 1;
    inputs[MULTIPLIER_DRY_WET].val = 1;
}

/*
 * Destructor.
 */
Multiplier::~Multiplier()
{

}

/*
 * Process all dependencies, then multiply the original signal by
 * 1 - the control values, and multiply the original signal by
 * the control values scaled. One done, sum the two to get the
 * final output signal for the multiplier module.
 */
void Multiplier::process()
{
    // Process any dependencies
    process_dependencies();

    if(!inputs[MULTIPLIER_SIGNAL].live)
        inputs[MULTIPLIER_SIGNAL].val = 0;

    for(unsigned short i = 0; i < output.size(); i ++)
    {
        update_input_vals(i);

        output[i] = (inputs[MULTIPLIER_SIGNAL].val * (1 - inputs[MULTIPLIER_DRY_WET].val)) +
                       (inputs[MULTIPLIER_SIGNAL].val * inputs[MULTIPLIER_MULTIPLIER].val * inputs[MULTIPLIER_DRY_WET].val);
    }

    processed = true;
}

/*
 * Update parameters at the k rate.
 */
void Multiplier::update_control_values()
{

}

/*
 * Calculate the locations of graphics objects unique to this module type.
 */
void Multiplier::calculate_unique_graphics_object_locations()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        x_input_toggle_button, w_input_toggle_button,
        w_waveform, h_waveform,
        x_signal_multiplier, x_signal_input_toggle_button, w_signals,
        y3, y4, y5, y6, y7;

    x_text = upper_left.x + 2;
    x_text_box = upper_left.x;
    w_text_box = (MODULE_WIDTH) - 11;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box + 1;
    w_input_toggle_button = 10;
    w_waveform = MODULE_WIDTH;
    h_waveform = 135;
    y3 = upper_left.y + 23;
    y4 = y3 + 136;
    y5 = y4 + 15;
    y6 = y5 + 15;
    y7 = y6 + 15;
    x_signal_multiplier = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_signals = (MODULE_WIDTH / 2) - 11;
    x_signal_input_toggle_button = x_text_box + w_signals + 1;

    graphics_object_locations.push_back({x_text, y4, 0, 0});
    graphics_object_locations.push_back({x_text, y6, 0, 0});
    graphics_object_locations.push_back({x_text_box, y3, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y5, w_signals, h_text_box});
    graphics_object_locations.push_back({x_signal_multiplier, y5, w_signals - 1, h_text_box});
    graphics_object_locations.push_back({x_text_box, y7, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_signal_input_toggle_button, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y7, w_input_toggle_button, h_text_box});
}

/*
 * Initialize all graphics objects unique to this module type, and add them to the array
 * of graphics objects.
 */
void Multiplier::initialize_unique_graphics_objects()
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

    names = {name + " signal & multiplier input (text)", name + " dry/wet (text)"};
    locations = {graphics_object_locations[MULTIPLIER_INPUT_TEXT],
                 graphics_object_locations[MULTIPLIER_DRY_WET_TEXT]};
    colors = std::vector<SDL_Color *>(2, &secondary_module_color);
    texts = {"SIGNAL & MULTIPLIER:", "DRY/WET:"};
    fonts = std::vector<TTF_Font *>(2, FONT_REGULAR);

    tmp_graphics_objects = initialize_text_objects(names, locations, colors, texts, fonts);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {name + " waveform visualizer (waveform)"};
    locations = {graphics_object_locations[MULTIPLIER_OUTPUT_WAVEFORM]};
    colors = {&primary_module_color};
    background_colors = {&secondary_module_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&output};

    tmp_graphics_objects = initialize_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {name + " signal (input text box)", name + " multiplier (input text box)",
             name + " dry/wet (input text box)"};
    locations = {graphics_object_locations[MULTIPLIER_SIGNAL_INPUT_TEXT_BOX],
                 graphics_object_locations[MULTIPLIER_MULTIPLIER_INPUT_TEXT_BOX],
                 graphics_object_locations[MULTIPLIER_DRY_WET_INPUT_TEXT_BOX]};
    colors = std::vector<SDL_Color *>(3, &secondary_module_color);
    text_colors = std::vector<SDL_Color *>(3, &primary_module_color);
    prompt_texts = {"input", "# or input", "# or input"};
    fonts = std::vector<TTF_Font *>(3, FONT_SMALL);
    parents = std::vector<Module *>(3, this);
    input_nums = {MULTIPLIER_SIGNAL, MULTIPLIER_MULTIPLIER, MULTIPLIER_DRY_WET};
    input_toggle_buttons = std::vector<Input_Toggle_Button *>(3, NULL);

    initialize_input_text_box_objects(names, locations, colors, text_colors, prompt_texts, fonts, parents, input_nums, input_toggle_buttons);

    names = {name + " signal input (input toggle button)", name + " input (input toggle button)",
             name + " dry/wet input (input toggle button)"};
    locations = {graphics_object_locations[MULTIPLIER_SIGNAL_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MULTIPLIER_MULTIPLIER_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MULTIPLIER_DRY_WET_INPUT_TOGGLE_BUTTON]};
    colors = std::vector<SDL_Color *>(3, &RED);
    color_offs = std::vector<SDL_Color *>(3, &secondary_module_color);
    text_color_ons = std::vector<SDL_Color *>(3, &WHITE);
    text_color_offs = std::vector<SDL_Color *>(3, &primary_module_color);
    fonts = std::vector<TTF_Font *>(3, FONT_SMALL);
    texts = std::vector<std::string>(3, "I");
    text_offs = texts;
    bs = std::vector<bool>(3, false);
    parents = std::vector<Module *>(3, this);
    input_nums = {MULTIPLIER_SIGNAL, MULTIPLIER_MULTIPLIER,
                  MULTIPLIER_DRY_WET};

    input_text_boxes = {(Input_Text_Box *) graphics_objects[MULTIPLIER_SIGNAL_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MULTIPLIER_MULTIPLIER_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MULTIPLIER_DRY_WET_INPUT_TEXT_BOX]};

    initialize_input_toggle_button_objects(names, locations, colors, color_offs,
                                           text_color_ons, text_color_offs,
                                           fonts, texts, text_offs, bs, parents,
                                           input_nums, input_text_boxes);

    ((Input_Text_Box *) graphics_objects[MULTIPLIER_SIGNAL_INPUT_TEXT_BOX])->input_toggle_button = (Input_Toggle_Button *) graphics_objects[MULTIPLIER_SIGNAL_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *) graphics_objects[MULTIPLIER_MULTIPLIER_INPUT_TEXT_BOX])->input_toggle_button = (Input_Toggle_Button *) graphics_objects[MULTIPLIER_MULTIPLIER_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *) graphics_objects[MULTIPLIER_DRY_WET_INPUT_TEXT_BOX])->input_toggle_button = (Input_Toggle_Button *) graphics_objects[MULTIPLIER_DRY_WET_INPUT_TOGGLE_BUTTON];
}

std::string Multiplier::get_unique_text_representation()
{
    return "";
}

/*
 * Handle button presses. Multiplier button presses are used to remove the
 * module.
 */
void Multiplier::button_function(Button *button)
{
    if(button == graphics_objects[MODULE_REMOVE_MODULE_BUTTON])
        delete this;
}

/*
 * Multiplier has no toggle buttons. This is a dummy function.
 */
void Multiplier::toggle_button_function(Toggle_Button *toggle_button)
{

}
