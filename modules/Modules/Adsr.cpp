/*
 * Matthew Diamond 2015
 * Member functions for the ADSR class.
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
#include "Modules/Adsr.hpp"

// Included graphics classes
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Toggle_Button.hpp"
#include "Graphics_Objects/Waveform.hpp"

/*************************
 * ADSR MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Adsr::Adsr() :
    Module(ADSR),
    current_amplitude(0), adsr_stage(ADSR_A_STAGE)
{
    inputs[ADSR_A].val = 500;
    inputs[ADSR_D].val = 500;
    inputs[ADSR_S].val = 1;
    inputs[ADSR_R].val = 500;
}

/*
 * Destructor.
 */
Adsr::~Adsr()
{}

/*
 * Process all dependencies, then fill the output buffer with a waveform given
 * the data contained within this class and the audio device information.
 */
void Adsr::process()
{
    // Process any dependencies
    process_dependencies();

    // Calculate an amplitude for each sample
    for(unsigned short i = 0; i < output.size(); i ++)
    {
        // Update parameters
        update_input_vals(i);

        if(!inputs[ADSR_NOTE].live)
        {
            inputs[ADSR_NOTE].val = 0;
        }

        // Set the current output sample to the current amplitude
        output[i] = current_amplitude;

        // Determine whether or not a note on value is detected
        bool note_on = inputs[ADSR_NOTE].val == 1;

        // Do something different based on the current envelope stage
        switch(adsr_stage)
        {
        // During the attack stage, note on will result in incrementing
        // towards full amplitude and switching to the decay stage when full
        // amplitude is reached, note off will result in skipping ahead to
        // the release stage
        case ADSR_A_STAGE:
            if(note_on)
            {
                double increment = 1 / ((inputs[ADSR_A].val / 1000)
                                        * SAMPLE_RATE);
                current_amplitude += increment;
                if(current_amplitude >= 1)
                {
                    current_amplitude = 1;
                    adsr_stage = ADSR_D_STAGE;
                }
            }
            else
            {
                adsr_stage = ADSR_R_STAGE;
            }
            break;
        // During the decay stage, note on will result in decrementing
        // towards sustain amplitude and switching to the sustain stage once
        // sustain amplitude is reached, note off will result in skipping
        // ahead to the release stage
        case ADSR_D_STAGE:
            if(note_on)
            {
                double decrement = (1 - inputs[ADSR_S].val)
                                   / ((inputs[ADSR_D].val / 1000)
                                      * SAMPLE_RATE);
                current_amplitude -= decrement;
                if(current_amplitude <= inputs[ADSR_S].val)
                {
                    current_amplitude = inputs[ADSR_S].val;
                    adsr_stage = ADSR_S_STAGE;
                }
            }
            else
            {
                adsr_stage = ADSR_R_STAGE;
            }
            break;
        // During the sustain stage, note on does nothing, note off will
        // result in skipping ahead to the release stage
        case ADSR_S_STAGE:
            if(!note_on)
            {
                adsr_stage = ADSR_R_STAGE;
            }
            break;
        // During the release stage, note on will result in switching back
        // to the attack stage, note off will result in decrementing towards
        // 0 amplitude, and switching to the idle stage once 0 amplitude is
        // reached
        case ADSR_R_STAGE:
            if(note_on)
            {
                adsr_stage = ADSR_A_STAGE;
            }
            else
            {
                double decrement = inputs[ADSR_S].val
                                   / ((inputs[ADSR_R].val / 1000)
                                      * SAMPLE_RATE);
                current_amplitude -= decrement;
                if(current_amplitude <= 0)
                {
                    adsr_stage = ADSR_IDLE_STAGE;
                    current_amplitude = 0;
                }
            }
            break;
        // During the idle stage, note on will result in switching to the
        // attack stage, note off does nothing
        case ADSR_IDLE_STAGE:
            if(note_on)
            {
                adsr_stage = ADSR_A_STAGE;
            }
            break;
        }
    }

    processed = true;
}

/*
 * Calculate the locations of graphics objects unique to this module type.
 */
void Adsr::calculate_unique_graphics_object_locations()
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
                                         upper_left.y, 9, 15
                                        });
    graphics_object_locations.push_back({x_text, y4, 0, 0});
    graphics_object_locations.push_back({x_text, y6, 0, 0});
    graphics_object_locations.push_back({x_text, y8, 0, 0});
    graphics_object_locations.push_back({x_text_box, y3, w_waveform,
                                         h_waveform
                                        });
    graphics_object_locations.push_back({x_text_box, y5, w_text_box,
                                         h_text_box
                                        });
    graphics_object_locations.push_back({x_text_box, y7, w_signals,
                                         h_text_box
                                        });
    graphics_object_locations.push_back({x_signal_cv, y7, w_signals - 1,
                                         h_text_box
                                        });
    graphics_object_locations.push_back({x_text_box, y9, w_signals,
                                         h_text_box
                                        });
    graphics_object_locations.push_back({x_signal_cv, y9, w_signals - 1,
                                         h_text_box
                                        });
    graphics_object_locations.push_back({x_input_toggle_button, y5,
                                         w_input_toggle_button, h_text_box
                                        });
    graphics_object_locations.push_back({x_signal_input_toggle_button, y7,
                                         w_input_toggle_button, h_text_box
                                        });
    graphics_object_locations.push_back({x_input_toggle_button, y7,
                                         w_input_toggle_button, h_text_box
                                        });
    graphics_object_locations.push_back({x_signal_input_toggle_button, y9,
                                         w_input_toggle_button, h_text_box
                                        });
    graphics_object_locations.push_back({x_input_toggle_button, y9,
                                         w_input_toggle_button, h_text_box
                                        });
}

/*
 * Initialize all graphics objects unique to this module type, and add them to
 * the array of graphics objects.
 */
void Adsr::initialize_unique_graphics_objects()
{
    std::vector<std::string> names, texts, prompt_texts, text_offs;
    std::vector<SDL_Rect> locations;
    std::vector<SDL_Color *> colors, background_colors, color_offs, text_colors,
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
    button = new Button(name + " reset current amplitude (button)",
                        graphics_object_locations[ADSR_RESET_CURRENT_AMPLITUDE_BUTTON],
                        &secondary_module_color, &primary_module_color, "0",
                        this);
    graphics_objects.push_back(button);

    names = {name + " note on/off (text)", name + " attack and decay (text)",
             name + " sustain and release (text)"
            };
    locations = {graphics_object_locations[ADSR_NOTE_TEXT],
                 graphics_object_locations[ADSR_A_D_TEXT],
                 graphics_object_locations[ADSR_S_R_TEXT]
                };
    colors = std::vector<SDL_Color *>(3, &secondary_module_color);
    texts = {"NOTE ON/OFF:", "ATTACK & DECAY:", "SUSTAIN & RELEASE:"};
    fonts = std::vector<TTF_Font *>(3, FONT_REGULAR);

    tmp_graphics_objects = initialize_text_objects(names, locations, colors,
                                                   texts, fonts);
    graphics_objects.insert(graphics_objects.end(),
                            tmp_graphics_objects.begin(),
                            tmp_graphics_objects.end());

    names = {name + " waveform visualizer (waveform)"};
    locations = {graphics_object_locations[ADSR_OUTPUT_WAVEFORM]};
    colors = {&primary_module_color};
    background_colors = {&secondary_module_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&output};

    tmp_graphics_objects = initialize_waveform_objects(names, locations, colors,
                                                       background_colors,
                                                       range_lows, range_highs,
                                                       buffers);
    graphics_objects.insert(graphics_objects.end(),
                            tmp_graphics_objects.begin(),
                            tmp_graphics_objects.end());

    names = {name + " note on/off (input text box)",
             name + " attack (input text box)",
             name + " decay (input text box)",
             name + " sustain (input text box)",
             name + " release (input text box)"
            };
    locations = {graphics_object_locations[ADSR_NOTE_INPUT_TEXT_BOX],
                 graphics_object_locations[ADSR_A_INPUT_TEXT_BOX],
                 graphics_object_locations[ADSR_D_INPUT_TEXT_BOX],
                 graphics_object_locations[ADSR_S_INPUT_TEXT_BOX],
                 graphics_object_locations[ADSR_R_INPUT_TEXT_BOX]
                };
    colors = std::vector<SDL_Color *>(5, &secondary_module_color);
    text_colors = std::vector<SDL_Color *>(5, &primary_module_color);
    prompt_texts = std::vector<std::string>(5, "# or input");
    fonts = std::vector<TTF_Font *>(5, FONT_REGULAR);
    parents = std::vector<Module *>(5, this);
    input_nums = {ADSR_NOTE, ADSR_A, ADSR_D, ADSR_S, ADSR_R};
    input_toggle_buttons = std::vector<Input_Toggle_Button *>(5, NULL);

    initialize_input_text_box_objects(names, locations, colors, text_colors,
                                      prompt_texts, fonts, parents, input_nums,
                                      input_toggle_buttons);

    names = {name + " note on/off (input toggle button)",
             name + " attack (input toggle button)",
             name + " decay (input toggle button)",
             name + " sustain (input toggle button)",
             name + " release (input toggle button)"
            };
    locations = {graphics_object_locations[ADSR_NOTE_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[ADSR_A_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[ADSR_D_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[ADSR_S_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[ADSR_R_INPUT_TOGGLE_BUTTON]
                };
    colors = std::vector<SDL_Color *>(5, &RED);
    color_offs = std::vector<SDL_Color *>(5, &secondary_module_color);
    text_color_ons = std::vector<SDL_Color *>(5, &WHITE);
    text_color_offs = std::vector<SDL_Color *>(5, &primary_module_color);
    fonts = std::vector<TTF_Font *>(5, FONT_REGULAR);
    texts = std::vector<std::string>(5, "I");
    text_offs = texts;
    bs = std::vector<bool>(5, false);
    parents = std::vector<Module *>(5, this);
    input_nums = {ADSR_NOTE, ADSR_A, ADSR_D,
                  ADSR_S, ADSR_R
                 };

    input_text_boxes = {(Input_Text_Box *) graphics_objects[ADSR_NOTE_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[ADSR_A_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[ADSR_D_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[ADSR_S_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[ADSR_R_INPUT_TEXT_BOX]
                       };

    initialize_input_toggle_button_objects(names, locations, colors, color_offs,
                                           text_color_ons, text_color_offs,
                                           fonts, texts, text_offs, bs, parents,
                                           input_nums, input_text_boxes);

    ((Input_Text_Box *)
     graphics_objects[ADSR_NOTE_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[ADSR_NOTE_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[ADSR_A_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[ADSR_A_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[ADSR_D_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[ADSR_D_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[ADSR_S_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[ADSR_S_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *)
     graphics_objects[ADSR_R_INPUT_TEXT_BOX])->input_toggle_button =
         (Input_Toggle_Button *) graphics_objects[ADSR_R_INPUT_TOGGLE_BUTTON];
}

std::string Adsr::get_unique_text_representation()
{
    return std::to_string(current_amplitude) + "\n"
           + std::to_string(adsr_stage) + "\n";
}

/*
 * Reset this ADSR's amplitude
 */
void Adsr::reset_current_amplitude()
{
    current_amplitude = 0;
    adsr_stage = ADSR_A_STAGE;

    std::cout << name << " current amplitude reset" << std::endl;
}

/*
 * Handle button presses. Adsr button presses are used to remove the module and
 * reset envelope amplitude.
 */
void Adsr::button_function(Button *button)
{
    if(button == graphics_objects[MODULE_REMOVE_MODULE_BUTTON])
    {
        delete this;
    }
    else if(button == graphics_objects[ADSR_RESET_CURRENT_AMPLITUDE_BUTTON])
    {
        reset_current_amplitude();
    }
}

/*
 * Adsr has no toggle buttons. This is a dummy function.
 */
void Adsr::toggle_button_function(Toggle_Button *toggle_button)
{}

