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
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Waveform.hpp"

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
    color.a = 255;
    text_color.a = 255;

    switch(_type)
    {
        case OUTPUT:
            parameter_names[OUTPUT_INPUT_L] = "LEFT SIGNAL";
            parameter_names[OUTPUT_INPUT_R] = "RIGHT SIGNAL";
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
            break;
        case OSCILLATOR:
            parameter_names[OSCILLATOR_FREQUENCY] = "FREQUENCY";
            parameter_names[OSCILLATOR_PHASE_OFFSET] = "PHASE OFFSET";
            parameter_names[OSCILLATOR_PULSE_WIDTH] = "PULSE WIDTH";
            parameter_names[OSCILLATOR_RANGE_LOW] = "RANGE LOW";
            parameter_names[OSCILLATOR_RANGE_HIGH] = "RANGE HIGH";
            break;
        case VCA:
            parameter_names[VCA_SIGNAL] = "SIGNAL";
            parameter_names[VCA_CV] = "CV";
            parameter_names[VCA_CV_AMOUNT] = "CV AMOUNT";
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

void Module::calculate_upper_left()
{
    int x, y;
    x = ((number % (MODULES_PER_ROW * MODULES_PER_COLUMN)) % MODULES_PER_ROW);
    y = ((number % (MODULES_PER_ROW * MODULES_PER_COLUMN)) / MODULES_PER_ROW);
    upper_left.x = ((x * MODULE_WIDTH) + (x * MODULE_SPACING));
    upper_left.y = ((y * MODULE_HEIGHT) + (y * MODULE_SPACING));
}

/*
 * Call upon each module to update its control values.
 */
void Module::update_control_values()
{
    update_unique_control_values();
}

void Module::update_graphics_objects()
{
    update_unique_graphics_objects();
}

/*
 * Calculate the locations of all graphics objects, then
 * call upon the module to caluclate the locations of
 * any graphics objects that are unique to the module type.
 */
void Module::calculate_graphics_objects()
{
    SDL_Rect location;
    string object_name;
    Rect *rect;
    Text *text;

    // Calculate the modules top left pixel location in the window
    calculate_upper_left();

    // If the graphics objects have not yet been initialized
    if(graphics_objects.size() == 0)
    {
        // graphics_object[0] is the outermost rectangle used to represent the module
        location = {upper_left.x, upper_left.y, MODULE_WIDTH, MODULE_HEIGHT};
        rect = new Rect("border (rect)", location, WHITE, this);
        graphics_objects.push_back(rect);

        // graphics_object[1] is the slightly smaller rectangle within the outermost
        // rectangle
        location = {upper_left.x + MODULE_BORDER_WIDTH,
                    upper_left.y + MODULE_BORDER_WIDTH,
                    MODULE_WIDTH - (2 * MODULE_BORDER_WIDTH),
                    MODULE_HEIGHT - (2 * MODULE_BORDER_WIDTH)};
        rect = new Rect("inner_border (rect)", location, color, this);
        graphics_objects.push_back(rect);

        // graphics_object[2] is the objects name
        location = {upper_left.x + MODULE_BORDER_WIDTH + 2,
                    upper_left.y + MODULE_BORDER_WIDTH + 5, 0, 0};
        text = new Text("module name (text)", location, text_color, name, FONT_BOLD);
        graphics_objects.push_back(text);
    }

    // If they have already been initialized, just update their locations
    else
    {
        location = {upper_left.x, upper_left.y, MODULE_WIDTH, MODULE_HEIGHT};
        graphics_objects[MODULE_BORDER_RECT]->update_location(location);

        location = {upper_left.x + MODULE_BORDER_WIDTH,
                    upper_left.y + MODULE_BORDER_WIDTH,
                    MODULE_WIDTH - (2 * MODULE_BORDER_WIDTH),
                    MODULE_HEIGHT - (2 * MODULE_BORDER_WIDTH)};
        graphics_objects[MODULE_INNER_BORDER_RECT]->update_location(location);

        location = {upper_left.x + MODULE_BORDER_WIDTH + 2,
            upper_left.y + MODULE_BORDER_WIDTH + 5, 0, 0};
        graphics_objects[MODULE_NAME_TEXT]->update_location(location);
    }

    calculate_unique_graphics_objects();
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
