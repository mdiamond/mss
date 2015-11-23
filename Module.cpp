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
            name = "mixer " + std::to_string(number);
            num_inputs = 16;
            break;
        case MULTIPLIER:
            name = "multiplier " + std::to_string(number);
            num_inputs = 3;
            break;
        case OSCILLATOR:
            name = "oscillator " + std::to_string(number);
            num_inputs = 5;
            break;
        case OUTPUT:
            name = "output";
            num_inputs = 2;
            break;
    }

    // Initialize the dependencies vector
    dependencies = std::vector<Module *>(num_inputs, NULL);

    // Initialize the module parameters as floats, strings, and
    // input buffers, and booleans to represent whether or not
    // the input for a certain parameter is live
    parameter_names = std::vector<std::string>(num_inputs, "");
    input_floats = std::vector<float>(num_inputs, 0);
    input_strs = std::vector<std::string>(num_inputs, "");
    inputs = std::vector<std::vector<float> *>(num_inputs, NULL);
    inputs_live = std::vector<bool>(num_inputs, false);

    // Initialize the output buffer
    output = std::vector<float>(BUFFER_SIZE, 0);

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
    // Lock audio to avoid interfering with processing
    SDL_LockAudio();

    // Cancel any inputs that this module is outputting to
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        for(unsigned int j = 0; j < MODULES[i]->dependencies.size(); j ++)
            if(MODULES[i]->dependencies[j] == this)
                MODULES[i]->cancel_input(j);

    // Erase this module from the list of modules
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(MODULES[i] == this)
            MODULES.erase(MODULES.begin() + i);

    // Delete all graphics objects
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
        delete graphics_objects[i];

    // Recalculate module numbers and names, update the name texts
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        MODULES[i]->number = i;

        switch(MODULES[i]->type)
        {
            case MIXER:
                MODULES[i]->name = "mixer " + std::to_string(MODULES[i]->number);
                break;
            case MULTIPLIER:
                MODULES[i]->name = "multiplier " + std::to_string(MODULES[i]->number);
                break;
            case OSCILLATOR:
                MODULES[i]->name = "oscillator " + std::to_string(MODULES[i]->number);
                break;
            case OUTPUT:
                MODULES[i]->name = "output";
                break;
        }
    }

    // Make sure that all inputs that were using this module as a source have
    // their associated input text boxes reset, and their input toggle buttons
    // turned off
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        int dependency_num = 0;
        Input_Text_Box *input_text_box;
        Input_Toggle_Button *input_toggle_button;

        for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
        {
            if(MODULES[i]->graphics_objects[j]->type == INPUT_TOGGLE_BUTTON)
            {
                input_toggle_button = ((Input_Toggle_Button *) MODULES[i]->graphics_objects[j]);
                if(input_toggle_button->b)
                {
                    input_text_box = input_toggle_button->input_text_box;
                    if(input_text_box->text.text == get_short_name())
                    {
                        if(input_text_box->prompt_text.text == "input")
                            input_text_box->update_current_text("");
                        else
                            input_text_box->update_current_text(std::to_string(MODULES[i]->input_floats[dependency_num]));
                        input_toggle_button->b = false;
                    }
                }
                dependency_num ++;
            }
        }
    }

    // Make sure that the text boxes in all modules accurately represent their inputs,
    // then update the module name text object
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        int dependency_num = 0;
        std::string dependency_short_name;
        Input_Text_Box *input_text_box;
        Text *text;

        for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
        {
            if(MODULES[i]->graphics_objects[j]->type == INPUT_TEXT_BOX
               && MODULES[i]->inputs_live[dependency_num])
            {
                input_text_box = (Input_Text_Box *) MODULES[i]->graphics_objects[j];
                dependency_short_name = MODULES[i]->dependencies[dependency_num]->get_short_name();
                input_text_box->update_current_text(dependency_short_name);
                dependency_num ++;
            }
            else if(MODULES[i]->graphics_objects[j]->type == INPUT_TEXT_BOX
                    && !MODULES[i]->inputs_live[dependency_num])
                dependency_num ++;
        }

        text = (Text *) MODULES[i]->graphics_objects[MODULE_NAME_TEXT];
        text->update_text(MODULES[i]->name);
    }

    // Mark modules changed so that they will be re-rendered
    MODULES_CHANGED = true;

    std::cout << name << " removed" << std::endl;

    // Unlock audio
    SDL_UnlockAudio();
}

/*
 * This function calls upon this module's dependencies
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
    graphics_object_locations.push_back({upper_left.x + MODULE_WIDTH - 10 - MODULE_BORDER_WIDTH,
                                         upper_left.y + MODULE_BORDER_WIDTH, 10, 15});

    calculate_unique_graphics_object_locations();
}

/*
 * Initialize a batch of input text box objects given arrays of contructor inputs.
 * Return a vector of the contructed graphics objects.
 */
void Module::initialize_input_text_box_objects(std::vector<std::string> names, std::vector<SDL_Rect> locations, std::vector<SDL_Color *> colors,
                                               std::vector<SDL_Color *> text_colors, std::vector<std::string> prompt_texts, std::vector<TTF_Font *> fonts,
                                               std::vector<Module *> parents, std::vector<int> input_nums)
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
void Module::initialize_input_toggle_button_objects(std::vector<std::string> names, std::vector<SDL_Rect> locations, std::vector<SDL_Color *> colors,
                                                    std::vector<SDL_Color *> color_offs, std::vector<SDL_Color *> text_color_ons,
                                                    std::vector<SDL_Color *> text_color_offs, std::vector<TTF_Font *> fonts,
                                                    std::vector<std::string> text_ons, std::vector<std::string> text_offs,
                                                    std::vector<bool> bs, std::vector<Module *> parents, std::vector<int> input_nums)
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
    std::string object_name;
    Button *button;
    Rect *rect;
    Text *text;

    // Calculate the locations of all graphics objects
    calculate_graphics_object_locations();

    // graphics_objects[0] is the outermost rectangle used to represent the module
    rect = new Rect(name + " border (rect)", graphics_object_locations[MODULE_BORDER_RECT], &WHITE, this);
    graphics_objects.push_back(rect);

    // graphics_objects[1] is the slightly smaller rectangle within the outermost
    // rectangle
    rect = new Rect(name + " background (rect)", graphics_object_locations[MODULE_BACKGROUND_RECT], &color, this);
    graphics_objects.push_back(rect);

    // graphics_objects[2] is the objects name
    text = new Text(name + " module name (text)", graphics_object_locations[MODULE_NAME_TEXT], &text_color, name, FONT_BOLD);
    graphics_objects.push_back(text);

    // graphics_objects[3] is the remove module button
    button = new Button(name + " remove module (button)", graphics_object_locations[MODULE_REMOVE_MODULE_BUTTON],
                        &color, &text_color, "X", this);
    graphics_objects.push_back(button);

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

    std::cout << name << " " << parameter_names[input_num]
         << " changed to " << val << std::endl;
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

    std::cout << name << " " << parameter_names[input_num]
         << " is now coming from " << src->name << std::endl;
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

    std::cout << name << " " << parameter_names[input_num]
         << " input cancelled" << std::endl;
}

/*
 * Return this module's name.
 */
std::string Module::get_name()
{
    return name;
}

/*
 * Return this module's short name.
 * For example, if this module's name is "oscillator 1", its short
 * name will be "osc 1". A short name is always 5 characters long.
 */
std::string Module::get_short_name()
{
    return name.substr(0, 3) + " " + name.substr(name.find(" ") + 1);
}
