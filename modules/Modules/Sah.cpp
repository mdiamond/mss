/*
 * Matthew Diamond 2015
 * Member functions for the Sample and hold class.
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
#include "Modules/Sah.hpp"

// Included graphics classes
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

/*******************************
 * SAH MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor.
 */
Sah::Sah() :
    Module(SAH)
{
    inputs[SAH_SIGNAL].val = 0;
    inputs[SAH_HOLD_TIME].val = 500;
}

/*
 * Destructor.
 */
Sah::~Sah()
{}

/*
 * Process all dependencies, then start sampling and holding.
 */
void Sah::process()
{
    // Process any dependencies
    process_dependencies();

    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        update_input_vals(i);

        if(!inputs[SAH_SIGNAL].live)
        {
            inputs[SAH_SIGNAL].val = 0;
        }

        // If the amount of time until the next sample has passed, update the
        // sample to hold, update the hold time, then update the time to next
        // sample
        if(time_to_next_sample <= 0)
        {
            sample = inputs[SAH_SIGNAL].val;
            time_to_next_sample = inputs[SAH_HOLD_TIME].val;
        }

        // Set the output samples to the currently held sample,
        // then decrement the time to next sample by a single sample
        // in ms
        out[i] = sample;
        time_to_next_sample -= ((double) 1000.0 / (double) SAMPLE_RATE);
    }

    processed = true;
}

/*
 * Calculate the locations of graphics objects unique to this module type.
 */
void Sah::calculate_unique_graphics_object_locations()
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

    graphics_object_locations.push_back({upper_left.x + MODULE_WIDTH - 19,
                                         upper_left.y, 9, 15
                                        });
    graphics_object_locations.push_back({x_text, y4, 0, 0});
    graphics_object_locations.push_back({x_text, y6, 0, 0});
    graphics_object_locations.push_back({x_text_box, y3, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y5, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y7, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y7, w_input_toggle_button, h_text_box});
}

/*
 * Initialize all graphics objects unique to this module type, and add them to the array
 * of graphics objects.
 */
void Sah::initialize_unique_graphics_objects()
{
    std::vector<std::string> names, texts, prompt_texts, text_offs;
    std::vector<SDL_Rect> locations;
    std::vector<SDL_Color> colors, background_colors, color_offs, text_colors,
        text_color_ons, text_color_offs;
    std::vector<TTF_Font *> fonts;
    std::vector<float> range_lows, range_highs;
    std::vector<int> input_nums;
    std::vector<std::vector<float> *> buffers;
    std::vector<Module *> parents;
    std::vector<bool> bs;
    std::vector<Input_Text_Box *> input_text_boxes;
    std::vector<Input_Toggle_Button *> input_toggle_buttons;

    std::vector<Graphics_Object *> tmp_graphics_objects;

    Button *button;
    button = new Button(name + " reset sampler (button)",
                        graphics_object_locations[SAH_RESET_SAMPLER_BUTTON],
                        secondary_module_color, primary_module_color, "0", this);
    graphics_objects.push_back(button);

    names = {name + " signal input (text)", name + " hold time (text)"};
    locations = {graphics_object_locations[SAH_INPUT_TEXT],
                 graphics_object_locations[SAH_HOLD_TIME_TEXT]
                };
    colors = std::vector<SDL_Color>(2, secondary_module_color);
    texts = {"SIGNAL:", "HOLD TIME:"};
    fonts = std::vector<TTF_Font *>(2, FONT_REGULAR);

    tmp_graphics_objects = initialize_text_objects(names, locations, colors, texts,
                                                   fonts);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(),
                            tmp_graphics_objects.end());

    names = {name + " waveform visualizer (waveform)"};
    locations = {graphics_object_locations[SAH_OUTPUT_WAVEFORM]};
    colors = {primary_module_color};
    background_colors = {secondary_module_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&out};

    tmp_graphics_objects = initialize_waveform_objects(names, locations, colors,
                                                       background_colors, range_lows, range_highs, buffers);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(),
                            tmp_graphics_objects.end());

    names = {name + " signal (input text box)",
             name + " hold time (input text box)"
            };
    locations = {graphics_object_locations[SAH_SIGNAL_INPUT_TEXT_BOX],
                 graphics_object_locations[SAH_HOLD_TIME_INPUT_TEXT_BOX]
                };
    colors = std::vector<SDL_Color>(2, secondary_module_color);
    text_colors = std::vector<SDL_Color>(2, primary_module_color);
    prompt_texts = {"input", "# or input"};
    fonts = std::vector<TTF_Font *>(2, FONT_REGULAR);
    parents = std::vector<Module *>(2, this);
    input_nums = {SAH_SIGNAL, SAH_HOLD_TIME};
    input_toggle_buttons = std::vector<Input_Toggle_Button *>(2, NULL);

    initialize_input_text_box_objects(names, locations, colors, text_colors,
                                      prompt_texts, fonts, parents, input_nums, input_toggle_buttons);

    names = {name + " signal input (input toggle button)", name + " hold time input (input toggle button)"};
    locations = {graphics_object_locations[SAH_SIGNAL_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[SAH_HOLD_TIME_INPUT_TOGGLE_BUTTON]
                };
    colors = std::vector<SDL_Color>(2, RED);
    color_offs = std::vector<SDL_Color>(2, secondary_module_color);
    text_color_ons = std::vector<SDL_Color>(2, WHITE);
    text_color_offs = std::vector<SDL_Color>(2, primary_module_color);
    fonts = std::vector<TTF_Font *>(2, FONT_REGULAR);
    texts = std::vector<std::string>(2, "I");
    text_offs = texts;
    bs = std::vector<bool>(2, false);
    parents = std::vector<Module *>(2, this);
    input_nums = {SAH_SIGNAL, SAH_HOLD_TIME};

    input_text_boxes = {(Input_Text_Box *) graphics_objects[SAH_SIGNAL_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[SAH_HOLD_TIME_INPUT_TEXT_BOX]
                       };

    initialize_input_toggle_button_objects(names, locations, colors, color_offs,
                                           text_color_ons, text_color_offs,
                                           fonts, texts, text_offs, bs, parents,
                                           input_nums, input_text_boxes);

    ((Input_Text_Box *)
     graphics_objects[SAH_SIGNAL_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[SAH_SIGNAL_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[SAH_HOLD_TIME_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[SAH_HOLD_TIME_INPUT_TOGGLE_BUTTON];
}

/*
 * Reset the time to next sample.
 */
void Sah::reset_sampler()
{
    time_to_next_sample = 0;
}

std::string Sah::get_unique_text_representation()
{
    return "";
}

/*
 * Handle button presses. Sah button presses are used to reset the sampler.
 */
void Sah::button_function(Button *button)
{
    if(button == graphics_objects[MODULE_REMOVE_MODULE_BUTTON])
    {
        delete this;
    }
    else if(button == graphics_objects[SAH_RESET_SAMPLER_BUTTON])
    {
        reset_sampler();
    }
}

/*
 * Sah has no toggle buttons. This is a dummy function.
 */
void Sah::toggle_button_function(Toggle_Button *toggle_button)
{}

