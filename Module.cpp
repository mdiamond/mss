/*
 * Matthew Diamond 2015
 * Member functions for the Module class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "function_forwarder.hpp"
#include "image_processing.hpp"
#include "main.hpp"

// Included classes
#include "Module.hpp"
#include "Modules/Mixer.hpp"
#include "Modules/Output.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/VCA.hpp"

using namespace std;

/***************************
 * MODULE MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Module::Module(int _type) :
    type(_type), number(MODULES.size()), processed(false)
{
    int num_inputs;

    switch(_type)
    {
        case OUTPUT:
            name = "output";
            num_inputs = 2;
            break;
        case MIXER:
            name = "mixer " + to_string(number);
            num_inputs = 16;
            break;
        case OSCILLATOR:
            name = "oscillator " + to_string(number);
            num_inputs = 5;
            break;
        case VCA:
            name = "vca " + to_string(number);
            num_inputs = 3;
            break;
    }

    dependencies = vector<Module *>(num_inputs, NULL);

    parameter_names = vector<string>(num_inputs, "");
    input_floats = vector<float>(num_inputs, 0);
    input_strs = vector<string>(num_inputs, "");
    inputs = vector<vector<float> *>(num_inputs, NULL);
    inputs_live = vector<bool>(num_inputs, false);

    output = vector<float>(BUFFER_SIZE, 0);

    color.r = rand() % 128;
    text_color.r = (rand() % 128) + 128;
    color.g = rand() % 128;
    text_color.g = (rand() % 128) + 128;
    color.b = rand() % 128;
    text_color.b = (rand() % 128) + 128;

    if(text_color.r - color.r < 40)
        text_color.r += 20;
        color.r -= 20;
    if(text_color.g - color.g < 40)
        text_color.g += 20;
        color.g -= 20;
    if(text_color.b - color.b < 40)
        text_color.b += 20;
        color.b -= 20;

    color.a = 255;
    text_color.a = 255;

    switch(_type)
    {
        case OUTPUT:
            parameter_names[OUTPUT_INPUT_L] = "LEFT SIGNAL";
            parameter_names[OUTPUT_INPUT_R] = "RIGHT SIGNAL";
            color.r = 48;
            color.g = 219;
            color.b = 68;
            text_color.r = 219;
            text_color.g = 48;
            text_color.b = 199;
            break;
        case MIXER:
            parameter_names[MIXER_SIGNAL_1] = "SIGNAL 1";
            parameter_names[MIXER_SIGNAL_1_MULTIPLIER] = "SIGNAL 1 MULTIPLIER";
            parameter_names[MIXER_SIGNAL_2] = "SIGNAL 2";
            parameter_names[MIXER_SIGNAL_2_MULTIPLIER] = "SIGNAL 2 MULTIPLIER";
            parameter_names[MIXER_SIGNAL_3] = "SIGNAL 3";
            parameter_names[MIXER_SIGNAL_3_MULTIPLIER] = "SIGNAL 3 MULTIPLIER";
            parameter_names[MIXER_SIGNAL_4] = "SIGNAL 4";
            parameter_names[MIXER_SIGNAL_4_MULTIPLIER] = "SIGNAL 4 MULTIPLIER";
            parameter_names[MIXER_SIGNAL_5] = "SIGNAL 5";
            parameter_names[MIXER_SIGNAL_5_MULTIPLIER] = "SIGNAL 5 MULTIPLIER";
            parameter_names[MIXER_SIGNAL_6] = "SIGNAL 6";
            parameter_names[MIXER_SIGNAL_6_MULTIPLIER] = "SIGNAL 6 MULTIPLIER";
            parameter_names[MIXER_SIGNAL_7] = "SIGNAL 7";
            parameter_names[MIXER_SIGNAL_7_MULTIPLIER] = "SIGNAL 7 MULTIPLIER";
            parameter_names[MIXER_SIGNAL_8] = "SIGNAL 8";
            parameter_names[MIXER_SIGNAL_8_MULTIPLIER] = "SIGNAL 8 MULTIPLIER";
            color.r = 219;
            color.g = 116;
            color.b = 48;
            text_color.r = 48;
            text_color.g = 151;
            text_color.b = 219;
            break;
        case OSCILLATOR:
            parameter_names[OSCILLATOR_FREQUENCY] = "FREQUENCY";
            parameter_names[OSCILLATOR_PHASE_OFFSET] = "PHASE OFFSET";
            parameter_names[OSCILLATOR_PULSE_WIDTH] = "PULSE WIDTH";
            parameter_names[OSCILLATOR_RANGE_LOW] = "RANGE LOW";
            parameter_names[OSCILLATOR_RANGE_HIGH] = "RANGE HIGH";
            color.r = 48;
            color.g = 59;
            color.b = 219;
            text_color.r = 219;
            text_color.g = 208;
            text_color.b = 48;
            break;
        case VCA:
            parameter_names[VCA_SIGNAL] = "SIGNAL";
            parameter_names[VCA_CV] = "CV";
            parameter_names[VCA_CV_AMOUNT] = "CV AMOUNT";
            color.r = 48;
            color.g = 219;
            color.b = 199;
            text_color.r = 219;
            text_color.g = 48;
            text_color.b = 68;
            break;
    }
}

/*
 * Destructor.
 */
Module::~Module()
{
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
        delete graphics_objects[i];
}

/*
 * This function calls upon the modules dependencies
 * to process samples.
 */
void Module::process_dependencies()
{
    for(unsigned int i = 0; i < dependencies.size(); i ++)
        if(dependencies[i] != NULL && !dependencies[i]->processed)
            dependencies[i]->process();
}

void Module::calculate_graphics_object_locations()
{
    graphics_object_locations.clear();

    int x, y;
    x = ((number % (MODULES_PER_ROW * MODULES_PER_COLUMN)) % MODULES_PER_ROW);
    y = ((number % (MODULES_PER_ROW * MODULES_PER_COLUMN)) / MODULES_PER_ROW);
    upper_left.x = ((x * MODULE_WIDTH) + (x * MODULE_SPACING));
    upper_left.y = ((y * MODULE_HEIGHT) + (y * MODULE_SPACING));

    graphics_object_locations.push_back({upper_left.x, upper_left.y, MODULE_WIDTH, MODULE_HEIGHT});
    graphics_object_locations.push_back({upper_left.x + MODULE_BORDER_WIDTH, upper_left.y + MODULE_BORDER_WIDTH,
                                        MODULE_WIDTH - (2 * MODULE_BORDER_WIDTH), MODULE_HEIGHT - (2 * MODULE_BORDER_WIDTH)});
    graphics_object_locations.push_back({upper_left.x + MODULE_BORDER_WIDTH + 2, upper_left.y + MODULE_BORDER_WIDTH + 5, 0, 0});

    calculate_unique_graphics_object_locations();
}

void Module::create_text_objects(vector<string> names, vector<SDL_Rect> locations,
                         vector<SDL_Color> colors, vector<string> texts,
                         vector<TTF_Font *> fonts)
{
    Text *text = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        text = new Text(names[i], locations[i], colors[i], texts[i], fonts[i]);
        graphics_objects.push_back(text);
    }
}

void Module::create_toggle_button_objects(vector<string> names, vector<SDL_Rect> locations, vector<SDL_Color> colors,
                                  vector<SDL_Color> color_offs, vector<SDL_Color> text_color_ons, vector<SDL_Color> text_color_offs,
                                  vector<TTF_Font *> fonts,
                                  vector<string> text_ons, vector<string> text_offs,
                                  vector<bool> bs, vector<Module *> parents)
{
    Toggle_Button *toggle_button = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        toggle_button = new Toggle_Button(names[i], locations[i], colors[i], color_offs[i], text_color_ons[i],
                                          text_color_offs[i], fonts[i], text_ons[i], text_offs[i], bs[i], parents[i]);
        graphics_objects.push_back(toggle_button);
    }
}

void Module::create_waveform_objects(vector<string> names, vector<SDL_Rect> locations,
                             vector<SDL_Color> colors, vector<SDL_Color> background_colors,
                             vector<float> range_lows, vector<float> range_highs,
                             vector<vector<float> *> buffers)
{
    Waveform *waveform = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        waveform = new Waveform(names[i], locations[i], colors[i], background_colors[i], range_lows[i],
                                range_highs[i], buffers[i]);
        graphics_objects.push_back(waveform);
    }
}

void Module::create_input_text_box_objects(vector<string> names, vector<SDL_Rect> locations, vector<SDL_Color> colors,
                                           vector<SDL_Color> text_colors, vector<string> prompt_texts, vector<TTF_Font *> fonts,
                                           vector<Module *> parents, vector<int> input_nums)
{
    Input_Text_Box *input_text_box = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        input_text_box = new Input_Text_Box(names[i], locations[i], colors[i], text_colors[i],
                                            prompt_texts[i], fonts[i], parents[i], input_nums[i]);
        graphics_objects.push_back(input_text_box);
    }
}

void Module::create_input_toggle_button_objects(vector<string> names, vector<SDL_Rect> locations, vector<SDL_Color> colors,
                                        vector<SDL_Color> color_offs, vector<SDL_Color> text_color_ons, vector<SDL_Color> text_color_offs,
                                        vector<TTF_Font *> fonts,
                                        vector<string> text_ons, vector<string> text_offs,
                                        vector<bool> bs, vector<Module *> parents, vector<int> input_nums)
{
    Input_Toggle_Button *input_toggle_button = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        input_toggle_button = new Input_Toggle_Button(names[i], locations[i], colors[i], color_offs[i], text_color_ons[i],
                                            text_color_offs[i], fonts[i], text_ons[i], text_offs[i], bs[i], parents[i],
                                            input_nums[i],
                                            (Input_Text_Box *) graphics_objects[graphics_objects.size() - parameter_names.size()]);
        graphics_objects.push_back(input_toggle_button);
    }
}

/*
 * Calculate the locations of all graphics objects, then
 * call upon the module to caluclate the locations of
 * any graphics objects that are unique to the module type.
 */
void Module::initialize_graphics_objects()
{
    string object_name;
    Rect *rect;
    Text *text;

    // Calculate the modules top left pixel location in the window
    calculate_graphics_object_locations();

    // graphics_object[0] is the outermost rectangle used to represent the module
    rect = new Rect("border (rect)", graphics_object_locations[MODULE_BORDER_RECT], WHITE, this);
    graphics_objects.push_back(rect);

    // graphics_object[1] is the slightly smaller rectangle within the outermost
    // rectangle
    rect = new Rect("inner_border (rect)", graphics_object_locations[MODULE_INNER_BORDER_RECT], color, this);
    graphics_objects.push_back(rect);

    // graphics_object[2] is the objects name
    text = new Text("module name (text)", graphics_object_locations[MODULE_NAME_TEXT], text_color, name, FONT_BOLD);
    graphics_objects.push_back(text);

    initialize_unique_graphics_objects();

    // for(vector<Graphics_Object *>::iterator graphics_object = graphics_objects.begin();
    //     graphics_object < graphics_objects.end(); graphics_object ++)
    //     cout << (*graphics_object)->name << endl;

    graphics_objects_initialized = true;
}

void Module::update_graphics_object_locations()
{
    SDL_Point old_upper_left = upper_left;

    calculate_graphics_object_locations();

    if(old_upper_left.x != upper_left.x ||
       old_upper_left.y != upper_left.y)
    {
        for(unsigned int i = 0; i < graphics_objects.size(); i ++)
        {
            graphics_objects[i]->update_location(graphics_object_locations[i]);
            cout << i << ": " << graphics_objects[i]->name << ": " << graphics_object_locations[i].x << ", " << graphics_object_locations[i].y << endl;
        }
    }
}

void Module::set(float val, int input_num)
{
    inputs[input_num] = NULL;
    dependencies[input_num] = NULL;
    input_floats[input_num] = val;
    inputs_live[input_num] = false;

    cout << name << " " << parameter_names[input_num]
         << " changed to " << val << endl;
}

void Module::set(Module *src, int input_num)
{
    inputs[input_num] = &src->output;
    dependencies[input_num] = src;
    inputs_live[input_num] = true;
    SELECTING_SRC = false;
    reset_alphas();

    if(type == OUTPUT)
    {
        Waveform *waveform;
        if(input_num == OUTPUT_INPUT_L)
            waveform = (Waveform *) graphics_objects[OUTPUT_INPUT_L_WAVEFORM];
        else
            waveform = (Waveform *) graphics_objects[OUTPUT_INPUT_R_WAVEFORM];
        waveform->buffer = &src->output;
    }


    cout << name << " " << parameter_names[input_num]
         << " is now coming from " << src->name << endl;
}

void Module::cancel_input(int input_num)
{
    inputs[input_num] = NULL;
    dependencies[input_num] = NULL;
    inputs_live[input_num] = false;

    if(type == OUTPUT)
    {
        Waveform *waveform;
        if(input_num == OUTPUT_INPUT_L)
            waveform = (Waveform *) graphics_objects[OUTPUT_INPUT_L_WAVEFORM];
        else
            waveform = (Waveform *) graphics_objects[OUTPUT_INPUT_R_WAVEFORM];
        waveform->buffer = NULL;
    }

    cout << name << " " << parameter_names[input_num]
         << " input cancelled" << endl;
}
