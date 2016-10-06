/*
 * Matthew Diamond 2015
 * Member functions for the Output class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL.h"
#include "SDL_ttf.h"

// Included files
#include "../function_forwarder.hpp"
#include "../graphics_object_utils.hpp"
#include "../image_processing.hpp"
#include "../main.hpp"
#include "../signal_processing.hpp"

// Included classes
#include "../Graphics_Objects/Input_Text_Box.hpp"
#include "../Graphics_Objects/Input_Toggle_Button.hpp"
#include "../Graphics_Objects/Toggle_Button.hpp"
#include "../Graphics_Objects/Waveform.hpp"
#include "../Module.hpp"
#include "Output.hpp"

/***************************
 * OUTPUT MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Output::Output() :
    Module(OUTPUT)
{

}

/*
 * Destructor.
 */
Output::~Output()
{

}

/*
 * This function simply calls upon dependencies for
 * processing. The output module depends on all other
 * modules.
 */
void Output::process()
{
    process_dependencies();
}

/*
 * Update parameters at the k rate.
 */
void Output::update_control_values()
{

}

/*
 * Calculate the locations of graphics objects unique to this module type.
 */
void Output::calculate_unique_graphics_object_locations()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        x_input_toggle_button, w_input_toggle_button,
        x_button,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10;

    x_text = upper_left.x + 2;
    x_text_box = upper_left.x;
    w_text_box = MODULE_WIDTH - 11;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box + 1;
    w_input_toggle_button = 10;
    x_button = upper_left.x + 70;
    w_waveform = MODULE_WIDTH;
    h_waveform = 57;
    y3 = upper_left.y + 20;
    y4 = upper_left.y + 20;
    y5 = upper_left.y + 40;
    y6 = upper_left.y + 97;
    y7 = upper_left.y + 112;
    y8 = upper_left.y + 133;
    y9 = upper_left.y + 189;
    y10 = upper_left.y + 204;

    graphics_object_locations.push_back({x_text, y3, 0, 0});
    graphics_object_locations.push_back({x_text, y6, 0, 0});
    graphics_object_locations.push_back({x_text, y9, 0, 0});
    graphics_object_locations.push_back({x_text_box, y5, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y8, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y7, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y10, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y7, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y10, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_button, y4, 25, 15});
}

/*
 * Initialize all graphics objects unique to this module type, and add them to the array
 * of graphics objects.
 */
void Output::initialize_unique_graphics_objects()
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

    names = {name + " on/off (text)", name + " input left (text)", name + " input right (text)"};
    locations = {graphics_object_locations[OUTPUT_AUDIO_TOGGLE_TEXT],
                 graphics_object_locations[OUTPUT_INPUT_L_TEXT],
                 graphics_object_locations[OUTPUT_INPUT_R_TEXT]};
    colors = std::vector<SDL_Color *>(3, &secondary_module_color);
    texts = {"AUDIO ON:", "LEFT SIGNAL:", "RIGHT SIGNAL:"};
    fonts = std::vector<TTF_Font *>(3, FONT_REGULAR);

    tmp_graphics_objects = initialize_text_objects(names, locations, colors, texts, fonts);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {name + " waveform visualizer l (waveform)", name + " waveform visualizer r (waveform)"};
    locations = {graphics_object_locations[OUTPUT_INPUT_L_WAVEFORM],
                 graphics_object_locations[OUTPUT_INPUT_R_WAVEFORM]};
    colors = std::vector<SDL_Color *>(2, &primary_module_color);
    background_colors = std::vector<SDL_Color *>(2, &secondary_module_color);
    range_lows = std::vector<float>(2, -1);
    range_highs = std::vector<float>(2, 1);
    buffers = std::vector<std::vector<float> *>(2, NULL);

    tmp_graphics_objects = initialize_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {name + " input l (input text box)", name + " input r (input text box)"};
    locations = {graphics_object_locations[OUTPUT_INPUT_L_INPUT_TEXT_BOX],
                 graphics_object_locations[OUTPUT_INPUT_R_INPUT_TEXT_BOX]};
    colors = std::vector<SDL_Color *>(2, &secondary_module_color);
    text_colors = std::vector<SDL_Color *>(2, &primary_module_color);
    prompt_texts = std::vector<std::string>(2, "input");
    fonts = std::vector<TTF_Font *>(2, FONT_SMALL);
    parents = std::vector<Module *>(2, this);
    input_nums = {OUTPUT_INPUT_L, OUTPUT_INPUT_R};
    input_toggle_buttons = std::vector<Input_Toggle_Button *>(2, NULL);

    initialize_input_text_box_objects(names, locations, colors, text_colors, prompt_texts, fonts, parents, input_nums, input_toggle_buttons);

    names = {name + " input l (input toggle button)", name + " input r (input toggle button)"};
    locations = {graphics_object_locations[OUTPUT_INPUT_L_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[OUTPUT_INPUT_R_INPUT_TOGGLE_BUTTON]};
    colors = std::vector<SDL_Color *>(2, &RED);
    color_offs = std::vector<SDL_Color *>(2, &secondary_module_color);
    text_color_ons = std::vector<SDL_Color *>(2, &WHITE);
    text_color_offs = std::vector<SDL_Color *>(2, &primary_module_color);
    fonts = std::vector<TTF_Font *>(2, FONT_SMALL);
    texts = std::vector<std::string>(2, "I");
    text_offs = texts;
    bs = std::vector<bool>(2, false);
    parents = std::vector<Module *>(2, this);
    input_nums = {OUTPUT_INPUT_L, OUTPUT_INPUT_R};

    input_text_boxes = {(Input_Text_Box *) graphics_objects[OUTPUT_INPUT_L_INPUT_TEXT_BOX],
                        (Input_Text_Box *) graphics_objects[OUTPUT_INPUT_R_INPUT_TEXT_BOX]};

    initialize_input_toggle_button_objects(names, locations, colors, color_offs,
                                           text_color_ons, text_color_offs,
                                           fonts, texts, text_offs, bs, parents,
                                           input_nums, input_text_boxes);

    names = {name + " on/off button (toggle_button)"};
    locations = {graphics_object_locations[OUTPUT_AUDIO_TOGGLE_TOGGLE_BUTTON]};
    colors = std::vector<SDL_Color *>(1, &RED);
    color_offs = std::vector<SDL_Color *>(1, &secondary_module_color);
    text_color_ons = std::vector<SDL_Color *>(1, &WHITE);
    text_color_offs = std::vector<SDL_Color *>(1, &primary_module_color);
    fonts = std::vector<TTF_Font *>(1, FONT_BOLD);
    texts = std::vector<std::string>(1, "ON");
    text_offs = std::vector<std::string>(1, "OFF");
    bs = {true};
    parents = std::vector<Module *>(1, this);

    tmp_graphics_objects = initialize_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                                        text_color_offs, fonts, texts, text_offs, bs, parents);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    ((Input_Text_Box *) graphics_objects[OUTPUT_INPUT_L_INPUT_TEXT_BOX])->input_toggle_button = (Input_Toggle_Button *) graphics_objects[OUTPUT_INPUT_L_INPUT_TOGGLE_BUTTON];
    ((Input_Text_Box *) graphics_objects[OUTPUT_INPUT_R_INPUT_TEXT_BOX])->input_toggle_button = (Input_Toggle_Button *) graphics_objects[OUTPUT_INPUT_R_INPUT_TOGGLE_BUTTON];
}

/*
 * Toggle audio processing by either starting or pausing SDL
 * audio, which will halt or start calling of the callback function.
 */
void Output::toggle_audio_on()
{
    AUDIO_ON = !AUDIO_ON;

    if(AUDIO_ON)
        SDL_PauseAudio(0);
    else
        SDL_PauseAudio(1);

    std::cout << "Audio toggled" << std::endl;
}

std::string Output::get_unique_text_representation()
{
    return "";
}

/*
 * Handle button presses. Output button presses are not used.
 */
void Output::button_function(Button *button)
{
    if(button == graphics_objects[MODULE_REMOVE_MODULE_BUTTON])
    {
        std::cout << RED_STDOUT << "The output module cannot be deleted!"
                  << DEFAULT_STDOUT << std::endl;
    }
}

/*
 * Handle toggle button presses. Output toggle button presses are used to toggle
 * audio processing on or off.
 */
void Output::toggle_button_function(Toggle_Button *toggle_button)
{
    if(toggle_button == graphics_objects[OUTPUT_AUDIO_TOGGLE_TOGGLE_BUTTON])
        toggle_audio_on();
}
