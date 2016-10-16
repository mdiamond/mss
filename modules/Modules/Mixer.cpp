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
 * Calculate the locations of graphics objects unique to this module type.
 */
void Mixer::calculate_unique_graphics_object_locations()
{
    int x_text, x_text_box, x_text_box_2, w_text_box, h_text_box,
        x_input_toggle_button, x_input_toggle_button_2, w_input_toggle_button,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10, y11, y12;

    x_text = upper_left.x + 2;
    x_text_box = upper_left.x;
    x_text_box_2 = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_text_box = (MODULE_WIDTH / 2) - 12;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box + 1;
    x_input_toggle_button_2 = x_text_box_2 + w_text_box + 1;
    w_input_toggle_button = 10;
    w_waveform = MODULE_WIDTH;
    h_waveform = 46;
    y3 = upper_left.y + 23;
    y4 = y3 + 47;
    y5 = y4 + 15;
    y6 = y5 + 17;
    y7 = y6 + 17;
    y8 = y7 + 17;
    y9 = y8 + 17;
    y10 = y9 + 17;
    y11 = y10 + 17;
    y12 = y11 + 17;

    graphics_object_locations.push_back({x_text, y4, 0, 0});
    graphics_object_locations.push_back({x_text_box, y3, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y5, w_text_box + 1, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y5, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y6, w_text_box + 1, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y6, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y7, w_text_box + 1, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y7, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y8, w_text_box + 1, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y8, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y9, w_text_box + 1, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y9, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y10, w_text_box + 1, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y10, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y11, w_text_box + 1, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y11, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y12, w_text_box + 1, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y12, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button + 1, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button + 1, y6, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y6, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button + 1, y7, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y7, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button + 1, y8, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y8, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button + 1, y9, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y9, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button + 1, y10, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y10, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button + 1, y11, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y11, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button + 1, y12, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y12, w_input_toggle_button, h_text_box});
}

/*
 * Initialize all graphics objects unique to this module type, and add them to the array
 * of graphics objects.
 */
void Mixer::initialize_unique_graphics_objects()
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

    names = {name + " mixer signal and multiplier (text)"};
    locations = {graphics_object_locations[MIXER_SIGNALS_TEXT]};
    colors = std::vector<SDL_Color>(1, secondary_module_color);
    texts = {"SIGNAL & MULTIPLIER:"};
    fonts = std::vector<TTF_Font *>(1, FONT_REGULAR);

    tmp_graphics_objects = initialize_text_objects(names, locations, colors, texts,
                                                   fonts);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(),
                            tmp_graphics_objects.end());

    names = {name + " waveform visualizer (waveform)"};
    locations = {graphics_object_locations[MIXER_OUTPUT_WAVEFORM]};
    colors = {primary_module_color};
    background_colors = {secondary_module_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&out};

    tmp_graphics_objects = initialize_waveform_objects(names, locations, colors,
                                                       background_colors, range_lows, range_highs, buffers);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(),
                            tmp_graphics_objects.end());

    names = {name + " signal 1 (input text box)",
             name + " signal 1 multiplier (input text box)",
             name + " signal 2 (input text box)",
             name + " signal 2 multiplier (input text box)",
             name + " signal 3 (input text box)",
             name + " signal 3 multiplier (input text box)",
             name + " signal 4 (input text box)",
             name + " signal 4 multiplier (input text box)",
             name + " signal 5 (input text box)",
             name + " signal 5 multiplier (input text box)",
             name + " signal 6 (input text box)",
             name + " signal 6 multiplier (input text box)",
             name + " signal 7 (input text box)",
             name + " signal 7 multiplier (input text box)",
             name + " signal 8 (input text box)",
             name + " signal 8 multiplier (input text box)"
            };
    locations = {graphics_object_locations[MIXER_SIGNAL_1_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_1_MULTIPLIER_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_2_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_2_MULTIPLIER_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_3_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_3_MULTIPLIER_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_4_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_4_MULTIPLIER_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_5_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_5_MULTIPLIER_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_6_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_6_MULTIPLIER_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_7_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_7_MULTIPLIER_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_8_INPUT_TEXT_BOX],
                 graphics_object_locations[MIXER_SIGNAL_8_MULTIPLIER_INPUT_TEXT_BOX]
                };
    colors = std::vector<SDL_Color>(16, secondary_module_color);
    text_colors = std::vector<SDL_Color>(16, primary_module_color);
    prompt_texts = {"input", "# or input",
                    "input", "# or input",
                    "input", "# or input",
                    "input", "# or input",
                    "input", "# or input",
                    "input", "# or input",
                    "input", "# or input",
                    "input", "# or input"
                   };
    fonts = std::vector<TTF_Font *>(16, FONT_REGULAR);
    parents = std::vector<Module *>(16, this);
    input_nums = {MIXER_SIGNAL_1, MIXER_SIGNAL_1_MULTIPLIER,
                  MIXER_SIGNAL_2, MIXER_SIGNAL_2_MULTIPLIER,
                  MIXER_SIGNAL_3, MIXER_SIGNAL_3_MULTIPLIER,
                  MIXER_SIGNAL_4, MIXER_SIGNAL_4_MULTIPLIER,
                  MIXER_SIGNAL_5, MIXER_SIGNAL_5_MULTIPLIER,
                  MIXER_SIGNAL_6, MIXER_SIGNAL_6_MULTIPLIER,
                  MIXER_SIGNAL_7, MIXER_SIGNAL_7_MULTIPLIER,
                  MIXER_SIGNAL_8, MIXER_SIGNAL_8_MULTIPLIER
                 };
    input_toggle_buttons = std::vector<Input_Toggle_Button *>(16, NULL);

    initialize_input_text_box_objects(names, locations, colors, text_colors,
                                      prompt_texts, fonts, parents, input_nums, input_toggle_buttons);

    names = {name + " signal 1 (input toggle button)",
             name + " signal 1 multiplier (input toggle button)",
             name + " signal 2 (input toggle button)",
             name + " signal 2 multiplier (input toggle button)",
             name + " signal 3 (input toggle button)",
             name + " signal 3 multiplier (input toggle button)",
             name + " signal 4 (input toggle button)",
             name + " signal 4 multiplier (input toggle button)",
             name + " signal 5 (input toggle button)",
             name + " signal 5 multiplier (input toggle button)",
             name + " signal 6 (input toggle button)",
             name + " signal 6 multiplier (input toggle button)",
             name + " signal 7 (input toggle button)",
             name + " signal 7 multiplier (input toggle button)",
             name + " signal 8 (input toggle button)",
             name + " signal 8 multiplier (input toggle button)"
            };
    locations = {graphics_object_locations[MIXER_SIGNAL_1_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_1_MULTIPLIER_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_2_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_2_MULTIPLIER_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_3_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_3_MULTIPLIER_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_4_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_4_MULTIPLIER_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_5_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_5_MULTIPLIER_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_6_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_6_MULTIPLIER_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_7_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_7_MULTIPLIER_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_8_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[MIXER_SIGNAL_8_MULTIPLIER_INPUT_TOGGLE_BUTTON]
                };
    colors = std::vector<SDL_Color>(16, RED);
    color_offs = std::vector<SDL_Color>(16, secondary_module_color);
    text_color_ons = std::vector<SDL_Color>(16, WHITE);
    text_color_offs = std::vector<SDL_Color>(16, primary_module_color);
    fonts = std::vector<TTF_Font *>(16, FONT_REGULAR);
    texts = std::vector<std::string>(16, "I");
    text_offs = texts;
    bs = std::vector<bool>(16, false);
    parents = std::vector<Module *>(16, this);
    input_nums = {MIXER_SIGNAL_1, MIXER_SIGNAL_1_MULTIPLIER,
                  MIXER_SIGNAL_2, MIXER_SIGNAL_2_MULTIPLIER,
                  MIXER_SIGNAL_3, MIXER_SIGNAL_3_MULTIPLIER,
                  MIXER_SIGNAL_4, MIXER_SIGNAL_4_MULTIPLIER,
                  MIXER_SIGNAL_5, MIXER_SIGNAL_5_MULTIPLIER,
                  MIXER_SIGNAL_6, MIXER_SIGNAL_6_MULTIPLIER,
                  MIXER_SIGNAL_7, MIXER_SIGNAL_7_MULTIPLIER,
                  MIXER_SIGNAL_8, MIXER_SIGNAL_8_MULTIPLIER
                 };

    input_text_boxes = {(Input_Text_Box *) graphics_objects[MIXER_SIGNAL_1_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_1_MULTIPLIER_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_2_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_2_MULTIPLIER_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_3_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_3_MULTIPLIER_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_4_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_4_MULTIPLIER_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_5_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_5_MULTIPLIER_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_6_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_6_MULTIPLIER_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_7_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_7_MULTIPLIER_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_8_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[MIXER_SIGNAL_8_MULTIPLIER_INPUT_TEXT_BOX]
                       };


    initialize_input_toggle_button_objects(names, locations, colors, color_offs,
                                           text_color_ons, text_color_offs,
                                           fonts, texts, text_offs, bs, parents,
                                           input_nums, input_text_boxes);

    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_1_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[MIXER_SIGNAL_1_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_1_MULTIPLIER_INPUT_TEXT_BOX])->input_toggle_button
        = (Input_Toggle_Button *)
          graphics_objects[MIXER_SIGNAL_1_MULTIPLIER_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_2_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[MIXER_SIGNAL_2_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_2_MULTIPLIER_INPUT_TEXT_BOX])->input_toggle_button
        = (Input_Toggle_Button *)
          graphics_objects[MIXER_SIGNAL_2_MULTIPLIER_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_3_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[MIXER_SIGNAL_3_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_3_MULTIPLIER_INPUT_TEXT_BOX])->input_toggle_button
        = (Input_Toggle_Button *)
          graphics_objects[MIXER_SIGNAL_3_MULTIPLIER_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_4_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[MIXER_SIGNAL_4_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_4_MULTIPLIER_INPUT_TEXT_BOX])->input_toggle_button
        = (Input_Toggle_Button *)
          graphics_objects[MIXER_SIGNAL_4_MULTIPLIER_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_5_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[MIXER_SIGNAL_5_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_5_MULTIPLIER_INPUT_TEXT_BOX])->input_toggle_button
        = (Input_Toggle_Button *)
          graphics_objects[MIXER_SIGNAL_5_MULTIPLIER_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_6_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[MIXER_SIGNAL_6_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_6_MULTIPLIER_INPUT_TEXT_BOX])->input_toggle_button
        = (Input_Toggle_Button *)
          graphics_objects[MIXER_SIGNAL_6_MULTIPLIER_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_7_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[MIXER_SIGNAL_7_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_7_MULTIPLIER_INPUT_TEXT_BOX])->input_toggle_button
        = (Input_Toggle_Button *)
          graphics_objects[MIXER_SIGNAL_7_MULTIPLIER_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_8_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[MIXER_SIGNAL_8_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[MIXER_SIGNAL_8_MULTIPLIER_INPUT_TEXT_BOX])->input_toggle_button
        = (Input_Toggle_Button *)
          graphics_objects[MIXER_SIGNAL_8_MULTIPLIER_INPUT_TOGGLE_BUTTON];
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
    if(button == graphics_objects[MODULE_REMOVE_MODULE_BUTTON])
    {
        delete this;
    }
}

/*
 * Mixer has no toggle buttons. This is a dummy function.
 */
void Mixer::toggle_button_function(Toggle_Button *toggle_button)
{

}

