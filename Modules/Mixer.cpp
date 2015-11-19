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
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "../function_forwarder.hpp"
#include "../image_processing.hpp"
#include "../main.hpp"
#include "../signal_processing.hpp"

// Included classes
#include "../Module.hpp"
#include "Mixer.hpp"
#include "../Graphics_Objects/Input_Text_Box.hpp"
#include "../Graphics_Objects/Input_Toggle_Button.hpp"
#include "../Graphics_Objects/Text.hpp"
#include "../Graphics_Objects/Toggle_Button.hpp"
#include "../Graphics_Objects/Waveform.hpp"

using namespace std;

/**************************
 * MIXER MEMBER FUNCTIONS *
 **************************/

/*
 * Constructor.
 */
Mixer::Mixer() :
    Module(MIXER)
{
    auto_attenuate = true;

    // All multiplier floats should start at 1
    for(unsigned int i = 0; i < input_floats.size(); i ++)
        if(i % 2 == 1)
            input_floats[i] = 1;
}

/*
 * Destructor.
 */
Mixer::~Mixer()
{

}

/*
 * Process all dependencies, then sum and attenuate all signal inputs
 */
void Mixer::process()
{
    // Process any dependencies
    process_dependencies();

    short num_channels = 0;

    // Reset the output buffer
    for(int i = 0; i < BUFFER_SIZE; i ++)
        output[i] = 0;

    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        num_channels = 0;

        if(dependencies[MIXER_SIGNAL_1_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_1_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_1_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_2_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_2_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_2_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_3_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_3_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_3_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_4_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_4_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_4_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_5_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_5_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_5_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_6_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_6_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_6_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_7_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_7_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_7_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_8_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_8_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_8_MULTIPLIER]))[i];

        if(dependencies[MIXER_SIGNAL_1] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_1]))[i] * input_floats[MIXER_SIGNAL_1_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_2] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_2]))[i] * input_floats[MIXER_SIGNAL_2_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_3] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_3]))[i] * input_floats[MIXER_SIGNAL_3_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_4] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_4]))[i] * input_floats[MIXER_SIGNAL_4_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_5] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_5]))[i] * input_floats[MIXER_SIGNAL_5_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_6] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_6]))[i] * input_floats[MIXER_SIGNAL_6_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_7] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_7]))[i] * input_floats[MIXER_SIGNAL_7_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_8] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_8]))[i] * input_floats[MIXER_SIGNAL_8_MULTIPLIER];
            num_channels ++;
        }


        if(auto_attenuate)
            if(num_channels != 0)
                output[i] /= num_channels;
    }

    processed = true;
}

void Mixer::update_control_values()
{
    
}

void Mixer::calculate_unique_graphics_object_locations()
{
    int x_text, x_text_box, x_text_box_2, w_text_box, h_text_box,
        x_input_toggle_button, x_input_toggle_button_2, w_input_toggle_button,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10, y11, y12;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box_2 = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_text_box = (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3) - 11;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box + 1;
    x_input_toggle_button_2 = x_text_box_2 + w_text_box + 1;
    w_input_toggle_button = 10;
    w_waveform = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_waveform = 46;
    y3 = upper_left.y + MODULE_BORDER_WIDTH + 23;
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
    graphics_object_locations.push_back({x_text_box, y5, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y5, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y6, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y6, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y7, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y7, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y8, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y8, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y9, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y9, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y10, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y10, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y11, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y11, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y12, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box_2, y12, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y6, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y6, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y7, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y7, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y8, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y8, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y9, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y9, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y10, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y10, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y11, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y11, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y12, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button_2, y12, w_input_toggle_button, h_text_box});
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Mixer::initialize_unique_graphics_objects()
{
    vector<string> names, texts, prompt_texts, text_offs;
    vector<SDL_Rect> locations;
    vector<SDL_Color> colors, background_colors, color_offs, text_colors, text_color_ons, text_color_offs;
    vector<TTF_Font *> fonts;
    vector<float> range_lows, range_highs;
    vector<int> input_nums;
    vector<vector<float> *> buffers;
    vector<Module *> parents;
    vector<bool> bs;

    names = {"mixer signal 1 (text)"};
    locations = {graphics_object_locations[MIXER_SIGNALS_TEXT]};
    colors = vector<SDL_Color>(1, text_color);
    texts = {"SIGNAL & MULTIPLIER:"};
    fonts = vector<TTF_Font *>(1, FONT_REGULAR);

    create_text_objects(names, locations, colors, texts, fonts);

    names = {"waveform visualizer (waveform)"};
    locations = {graphics_object_locations[MIXER_OUTPUT_WAVEFORM]};
    colors = {color};
    background_colors = {text_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&output};

    create_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);

    names = {"mixer signal 1 (input text box)",
             "mixer signal 1 multiplier (input text box)",
             "mixer signal 2 (input text box)",
             "mixer signal 2 multiplier (input text box)",
             "mixer signal 3 (input text box)",
             "mixer signal 3 multiplier (input text box)",
             "mixer signal 4 (input text box)",
             "mixer signal 4 multiplier (input text box)",
             "mixer signal 5 (input text box)",
             "mixer signal 5 multiplier (input text box)",
             "mixer signal 6 (input text box)",
             "mixer signal 6 multiplier (input text box)",
             "mixer signal 7 (input text box)",
             "mixer signal 7 multiplier (input text box)",
             "mixer signal 8 (input text box)",
             "mixer signal 8 multiplier (input text box)"};
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
                 graphics_object_locations[MIXER_SIGNAL_8_MULTIPLIER_INPUT_TEXT_BOX]};
    colors = vector<SDL_Color>(16, text_color);
    text_colors = vector<SDL_Color>(16, color);
    prompt_texts = {"input", "# or input",
                    "input", "# or input",
                    "input", "# or input",
                    "input", "# or input",
                    "input", "# or input",
                    "input", "# or input",
                    "input", "# or input",
                    "input", "# or input"};
    fonts = vector<TTF_Font *>(16, FONT_SMALL);
    parents = vector<Module *>(16, this);
    input_nums = {MIXER_SIGNAL_1, MIXER_SIGNAL_1_MULTIPLIER,
                  MIXER_SIGNAL_2, MIXER_SIGNAL_2_MULTIPLIER,
                  MIXER_SIGNAL_3, MIXER_SIGNAL_3_MULTIPLIER,
                  MIXER_SIGNAL_4, MIXER_SIGNAL_4_MULTIPLIER,
                  MIXER_SIGNAL_5, MIXER_SIGNAL_5_MULTIPLIER,
                  MIXER_SIGNAL_6, MIXER_SIGNAL_6_MULTIPLIER,
                  MIXER_SIGNAL_7, MIXER_SIGNAL_7_MULTIPLIER,
                  MIXER_SIGNAL_8, MIXER_SIGNAL_8_MULTIPLIER};

    create_input_text_box_objects(names, locations, colors, text_colors, prompt_texts, fonts, parents, input_nums);

    names = {"mixer signal 1 (input toggle button)",
             "mixer signal 1 multiplier (input toggle button)",
             "mixer signal 2 (input toggle button)",
             "mixer signal 2 multiplier (input toggle button)",
             "mixer signal 3 (input toggle button)",
             "mixer signal 3 multiplier (input toggle button)",
             "mixer signal 4 (input toggle button)",
             "mixer signal 4 multiplier (input toggle button)",
             "mixer signal 5 (input toggle button)",
             "mixer signal 5 multiplier (input toggle button)",
             "mixer signal 6 (input toggle button)",
             "mixer signal 6 multiplier (input toggle button)",
             "mixer signal 7 (input toggle button)",
             "mixer signal 7 multiplier (input toggle button)",
             "mixer signal 8 (input toggle button)",
             "mixer signal 8 multiplier (input toggle button)"};
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
                 graphics_object_locations[MIXER_SIGNAL_8_MULTIPLIER_INPUT_TOGGLE_BUTTON]};
    colors = vector<SDL_Color>(16, RED);
    color_offs = vector<SDL_Color>(16, text_color);
    text_color_ons = vector<SDL_Color>(16, WHITE);
    text_color_offs = vector<SDL_Color>(16, color);
    fonts = vector<TTF_Font *>(16, FONT_SMALL);
    texts = vector<string>(16, "I");
    text_offs = texts;
    bs = vector<bool>(16, false);
    parents = vector<Module *>(16, this);
    input_nums = {MIXER_SIGNAL_1, MIXER_SIGNAL_1_MULTIPLIER,
                  MIXER_SIGNAL_2, MIXER_SIGNAL_2_MULTIPLIER,
                  MIXER_SIGNAL_3, MIXER_SIGNAL_3_MULTIPLIER,
                  MIXER_SIGNAL_4, MIXER_SIGNAL_4_MULTIPLIER,
                  MIXER_SIGNAL_5, MIXER_SIGNAL_5_MULTIPLIER,
                  MIXER_SIGNAL_6, MIXER_SIGNAL_6_MULTIPLIER,
                  MIXER_SIGNAL_7, MIXER_SIGNAL_7_MULTIPLIER,
                  MIXER_SIGNAL_8, MIXER_SIGNAL_8_MULTIPLIER};

    create_input_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                       text_color_offs, fonts, texts, text_offs, bs, parents, input_nums);
}
