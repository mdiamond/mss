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
#include "Modules/Multiplier.hpp"

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
    // The number of inputs this module has
    int num_inputs;

    // Set the number of inputs depending on the module type
    switch(_type)
    {
        case MIXER:
            name = "mixer " + to_string(number);
            num_inputs = 16;
            break;
        case MULTIPLIER:
            name = "multiplier " + to_string(number);
            num_inputs = 3;
            break;
        case OSCILLATOR:
            name = "oscillator " + to_string(number);
            num_inputs = 5;
            break;
        case OUTPUT:
            name = "output";
            num_inputs = 2;
            break;
    }

    // Initialize the dependencies vector
    dependencies = vector<Module *>(num_inputs, NULL);

    // Initialize the module parameters as floats, strings, and
    // input buffers, and booleans to represent whether or not
    // the input for a certain parameter is live
    parameter_names = vector<string>(num_inputs, "");
    input_floats = vector<float>(num_inputs, 0);
    input_strs = vector<string>(num_inputs, "");
    inputs = vector<vector<float> *>(num_inputs, NULL);
    inputs_live = vector<bool>(num_inputs, false);

    // Initialize the output buffer
    output = vector<float>(BUFFER_SIZE, 0);

    // Set this module's color randomly, but with enough contrast
    color.r = rand() % 128;
    color.g = rand() % 128;
    color.b = rand() % 128;
    text_color.r = 255 - color.g;
    text_color.g = 255 - color.b;
    text_color.b = 255 - color.r;

    color.a = 255;
    text_color.a = 255;

    // Set parameter names and module colors based on the module type
    switch(_type)
    {
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
            // color.r = 219;
            // color.g = 116;
            // color.b = 48;
            // text_color.r = 48;
            // text_color.g = 151;
            // text_color.b = 219;
            break;
        case MULTIPLIER:
            parameter_names[MULTIPLIER_SIGNAL] = "SIGNAL";
            parameter_names[MULTIPLIER_CV] = "CV";
            parameter_names[MULTIPLIER_CV_AMOUNT] = "CV AMOUNT";
            // color.r = 48;
            // color.g = 219;
            // color.b = 199;
            // text_color.r = 219;
            // text_color.g = 48;
            // text_color.b = 68;
            break;
        case OSCILLATOR:
            parameter_names[OSCILLATOR_FREQUENCY] = "FREQUENCY";
            parameter_names[OSCILLATOR_PHASE_OFFSET] = "PHASE OFFSET";
            parameter_names[OSCILLATOR_PULSE_WIDTH] = "PULSE WIDTH";
            parameter_names[OSCILLATOR_RANGE_LOW] = "RANGE LOW";
            parameter_names[OSCILLATOR_RANGE_HIGH] = "RANGE HIGH";
            // color.r = 48;
            // color.g = 59;
            // color.b = 219;
            // text_color.r = 219;
            // text_color.g = 208;
            // text_color.b = 48;
            break;
        case OUTPUT:
            parameter_names[OUTPUT_INPUT_L] = "LEFT SIGNAL";
            parameter_names[OUTPUT_INPUT_R] = "RIGHT SIGNAL";
            // color.r = 48;
            // color.g = 219;
            // color.b = 68;
            // text_color.r = 219;
            // text_color.g = 48;
            // text_color.b = 199;
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

/*
 * Use the upper left pixel of the module to calculate the locations of all
 * graphics objects, including those unique to this module type via
 * calculate_unique_graphics_objects()
 */
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

/*
 * Initialize a batch of input text box objects given arrays of contructor inputs.
 * Return a vector of the contructed graphics objects.
 */
void Module::initialize_input_text_box_objects(vector<string> names, vector<SDL_Rect> locations, vector<SDL_Color> colors,
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

/*
 * Initialize a batch of input toggle button objects given arrays of contructor inputs.
 * Return a vector of the contructed graphics objects.
 */
void Module::initialize_input_toggle_button_objects(vector<string> names, vector<SDL_Rect> locations, vector<SDL_Color> colors,
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
 * Calculate the locations of all graphics objects in this module
 * via calculate_graphics_object_locations(), then initialize the
 * graphics objects required for all module types, and finally
 * initialize the graphics objects specific to this module type via
 * initialize_inique_graphics_objects().
 */
void Module::initialize_graphics_objects()
{
    string object_name;
    Rect *rect;
    Text *text;

    // Calculate the locations of all graphics objects
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

    // Initialize all graphics objects specific to this module type
    initialize_unique_graphics_objects();

    // Mark this function as complete so that running it again can easily be avoided
    graphics_objects_initialized = true;
}

/*
 * Update the locations of all graphics object.
 */
void Module::update_graphics_object_locations()
{
    // Calculate graphics object locations
    calculate_graphics_object_locations();

    // Update graphics object locations
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
        graphics_objects[i]->update_location(graphics_object_locations[i]);
}

/*
 * Set the parameter specified by input num to the value
 * specified by val.
 */
void Module::set(float val, int input_num)
{
    // Set the input and dependency to NULL,
    // the float to val, and the live boolean to false
    inputs[input_num] = NULL;
    dependencies[input_num] = NULL;
    input_floats[input_num] = val;
    inputs_live[input_num] = false;

    cout << name << " " << parameter_names[input_num]
         << " changed to " << val << endl;
}

/*
 * Set the parameter specified to be updated by the output of
 * the module specified.
 */
void Module::set(Module *src, int input_num)
{
    // Set the input to the output of src, the dependency to src,
    // the live boolean to true, and the SELECTING_SRC program state variable
    // to false
    inputs[input_num] = &src->output;
    dependencies[input_num] = src;
    inputs_live[input_num] = true;
    SELECTING_SRC = false;

    // Reset the transparencies for all graphics objects
    reset_alphas();

    // If this is the output module, update the waveforms to display
    // the proper audio buffers
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

/*
 * Cancel input from another module, reverting to a constant value for the
 * parameter specified by input num. The value will stay at whatever it was
 * most recently.
 */
void Module::cancel_input(int input_num)
{
    // Set the input and dependency to NULL,
    // and the live boolean to false
    inputs[input_num] = NULL;
    dependencies[input_num] = NULL;
    inputs_live[input_num] = false;

    // If this is the output module, update the waveforms to display
    // an empty audio buffer
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
