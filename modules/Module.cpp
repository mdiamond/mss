/*
 * Matthew Diamond 2015
 * Member functions for the Module class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Included files
#include "function_forwarder.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "module_utils.hpp"

// Included modules classes
#include "Module.hpp"
#include "Modules/Adsr.hpp"
#include "Modules/Delay.hpp"
#include "Modules/Filter.hpp"
#include "Modules/Mixer.hpp"
#include "Modules/Multiplier.hpp"
#include "Modules/Noise.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"
#include "Modules/Sah.hpp"

// Included graphics classes
#include "Graphics_Object.hpp"

/*******************************
 * MODULE NAME PER MODULE TYPE *
 *******************************/

const std::map<Module::ModuleType, std::string> Module::names =
{
    {ADSR, "adsr"},
    {DELAY, "delay"},
    {FILTER, "filter"},
    {MIXER, "mixer"},
    {MULTIPLIER, "multiplier"},
    {NOISE, "noise"},
    {OSCILLATOR, "oscillator"},
    {OUTPUT, "output"},
    {SAH, "sah"}
};

/******************************************
 * MODULE PARAMETER NAMES PER MODULE TYPE *
 ******************************************/

const std::map<Module::ModuleType, std::vector<std::string> > Module::parameters
=
{
    {
        ADSR,
        {
            "NOTE ON/OFF",
            "ATTACK",
            "DECAY",
            "SUSTAIN",
            "RELEASE"
        }
    },
    {
        DELAY,
        {
            "SIGNAL",
            "MAX DELAY TIME",
            "DELAY TIME",
            "FEEDBACK AMOUNT",
            "WET/DRY AMOUNT"
        }
    },
    {
        FILTER,
        {
            "SIGNAL",
            "FREQUENCY CUTOFF",
            "FILTER QUALITY"
        }
    },
    {
        MIXER,
        {
            "SIGNAL 1",
            "SIGNAL 1 MULTIPLIER",
            "SIGNAL 2",
            "SIGNAL 2 MULTIPLIER",
            "SIGNAL 3",
            "SIGNAL 3 MULTIPLIER",
            "SIGNAL 4",
            "SIGNAL 4 MULTIPLIER",
            "SIGNAL 5",
            "SIGNAL 5 MULTIPLIER",
            "SIGNAL 6",
            "SIGNAL 6 MULTIPLIER",
            "SIGNAL 7",
            "SIGNAL 7 MULTIPLIER",
            "SIGNAL 8",
            "SIGNAL 8 MULTIPLIER"
        }
    },
    {
        MULTIPLIER,
        {
            "SIGNAL",
            "MULTIPLIER",
            "DRY/WET"
        }
    },
    {
        NOISE,
        {
            "RANGE LOW",
            "RANGE HIGH"
        }
    },
    {
        OSCILLATOR,
        {
            "FREQUENCY",
            "PHASE OFFSET",
            "PULSE WIDTH",
            "RANGE LOW",
            "RANGE HIGH"
        }
    },
    {
        OUTPUT,
        {
            "LEFT SIGNAL",
            "RIGHT SIGNAL"
        }
    },
    {
        SAH,
        {
            "SIGNAL",
            "HOLD TIME"
        }
    }
};

/***************************
 * MODULE MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Module::Module(ModuleType _module_type) :
    Graphics_Object(
        names.at(_module_type) + " "
        + std::to_string(find_available_module_number(_module_type)),
        MODULE, NULL, find_module_location(find_available_module_slot()), NULL),
    module_type(_module_type), number(find_available_module_slot()),
    processed(false),
    inputs(std::vector<Input>(parameters.at(_module_type).size())),
    output(std::vector<float>(BUFFER_SIZE, 0))
{
    // Set this module's color randomly, but with enough contrast
    std::vector<SDL_Color> colors = generate_module_colors();
    primary_module_color = colors[0];
    secondary_module_color = colors[1];

    std::cout << "Module \"" << name << "\" created" << std::endl;
}

/*
 * Destructor.
 */
Module::~Module()
{
    // Lock audio to avoid interfering with processing
    SDL_LockAudio();

    // Remove this module from the list of modules
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(MODULES[i] != NULL && MODULES[i] == this)
        {
            MODULES[i] = NULL;
        }

    // Cancel any inputs that this module is outputting to
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(MODULES[i] != NULL)
            for(unsigned int j = 0; j < MODULES[i]->inputs.size(); j ++)
                if(MODULES[i]->inputs[j].from == this)
                {
                    MODULES[i]->cancel_input(j);
                }

    // Erase this module from the list of modules
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(MODULES[i] != NULL && MODULES[i] == this)
        {
            MODULES.erase(MODULES.begin() + i);
        }

    // Delete all graphics objects
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
    {
        delete graphics_objects[i];
    }

    // Make sure that all inputs that were using this module as a source have
    // their associated input text boxes reset, and their input toggle buttons
    // turned off
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != NULL)
        {
            int dependency_num = 0;
            Input_Text_Box *input_text_box;
            Input_Toggle_Button *input_toggle_button;

            for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
            {
                if(MODULES[i]->graphics_objects[j]->graphics_object_type
                   == INPUT_TOGGLE_BUTTON)
                {
                    input_toggle_button =
                        ((Input_Toggle_Button *) MODULES[i]->graphics_objects[j]);
                    if(input_toggle_button->b)
                    {
                        input_text_box = input_toggle_button->input_text_box;
                        if(input_text_box->text.text == get_short_name())
                        {
                            if(input_text_box->prompt_text.text == "input")
                            {
                                input_text_box->update_current_text("");
                            }
                            else
                            {
                                input_text_box->update_current_text(
                                    std::to_string(MODULES[i]->inputs[dependency_num].val));
                            }
                            input_toggle_button->b = false;
                        }
                    }
                    dependency_num ++;
                }
            }
        }
    }

    // Make sure that the text boxes in all modules accurately represent their inputs,
    // then update the module name text object
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != NULL)
        {
            int dependency_num = 0;
            std::string dependency_short_name;
            Input_Text_Box *input_text_box;
            Text *text;

            for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
            {
                if(MODULES[i]->graphics_objects[j]->graphics_object_type == INPUT_TEXT_BOX
                   && MODULES[i]->inputs[dependency_num].live)
                {
                    input_text_box = (Input_Text_Box *) MODULES[i]->graphics_objects[j];
                    dependency_short_name =
                        MODULES[i]->inputs[dependency_num].from->get_short_name();
                    input_text_box->update_current_text(dependency_short_name);
                    dependency_num ++;
                }
                else if(MODULES[i]->graphics_objects[j]->graphics_object_type == INPUT_TEXT_BOX
                        && !MODULES[i]->inputs[dependency_num].live)
                {
                    dependency_num ++;
                }
            }

            text = (Text *) MODULES[i]->graphics_objects[MODULE_NAME_TEXT];
            text->update_text(MODULES[i]->name);
        }
    }

    // Mark modules changed so that they will be re-rendered
    MODULES_CHANGED = true;

    std::cout << "Module \"" << name << "\" removed" << std::endl;

    // Unlock audio
    SDL_UnlockAudio();
}

/*
 * This function calls upon this module's dependencies
 * to process samples.
 */
void Module::process_dependencies()
{
    for(unsigned int i = 0; i < inputs.size(); i ++)
        if(inputs[i].live && !inputs[i].from->processed)
        {
            inputs[i].from->process();
        }
}

/*
 * This function stores the sample at index i from each of the input buffers as
 * floats in their input struct's respective value variable.
 * This is essentially just updating the locally stored values for each parameter.
 */
void Module::update_input_vals(int i)
{
    for(unsigned int j = 0; j < inputs.size(); j ++)
        if(inputs[j].live)
        {
            inputs[j].val = inputs[j].input->at(i);
        }
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

    graphics_object_locations.push_back({upper_left.x, upper_left.y,
                                         MODULE_WIDTH, MODULE_HEIGHT
                                        });
    graphics_object_locations.push_back({upper_left.x + 2, upper_left.y + 3, 0,
                                         0
                                        });
    graphics_object_locations.push_back({upper_left.x + MODULE_WIDTH - 9,
                                         upper_left.y, 9, 15
                                        });

    calculate_unique_graphics_object_locations();
}

/*
 * Initialize a batch of input text box objects given arrays of contructor inputs.
 * Return a vector of the contructed graphics objects.
 */
void Module::initialize_input_text_box_objects(
    std::vector<std::string> names,
    std::vector<SDL_Rect> locations,
    std::vector<SDL_Color *> colors,
    std::vector<SDL_Color *> text_colors,
    std::vector<std::string> prompt_texts,
    std::vector<TTF_Font *> fonts,
    std::vector<Module *> parents,
    std::vector<int> input_nums,
    std::vector<Input_Toggle_Button *> input_toggle_buttons)
{
    Input_Text_Box *input_text_box = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        input_text_box = new Input_Text_Box(names[i], locations[i], colors[i],
                                            text_colors[i],
                                            prompt_texts[i], fonts[i], parents[i], input_nums[i],
                                            input_toggle_buttons[i]);
        graphics_objects.push_back(input_text_box);
    }
}

/*
 * Initialize a batch of input toggle button objects given arrays of contructor inputs.
 * Return a vector of the contructed graphics objects.
 */
void Module::initialize_input_toggle_button_objects(
    std::vector<std::string> names,
    std::vector<SDL_Rect> locations,
    std::vector<SDL_Color *> colors,
    std::vector<SDL_Color *> color_offs,
    std::vector<SDL_Color *> text_color_ons,
    std::vector<SDL_Color *> text_color_offs,
    std::vector<TTF_Font *> fonts,
    std::vector<std::string> text_ons,
    std::vector<std::string> text_offs,
    std::vector<bool> bs,
    std::vector<Module *> parents,
    std::vector<int> input_nums,
    std::vector<Input_Text_Box *> input_text_boxes)
{
    Input_Toggle_Button *input_toggle_button = NULL;

    for(unsigned int i = 0; i < names.size(); i ++)
    {
        input_toggle_button = new Input_Toggle_Button(
            names[i], locations[i], colors[i],
            color_offs[i], text_color_ons[i],
            text_color_offs[i], fonts[i], text_ons[i],
            text_offs[i], bs[i], parents[i],
            input_nums[i], input_text_boxes[i]);
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

    // graphics_objects[0] is the slightly smaller rectangle within the outermost
    // rectangle
    rect = new Rect(name + " background (rect)",
                    graphics_object_locations[MODULE_BACKGROUND_RECT],
                    &primary_module_color,
                    this);
    graphics_objects.push_back(rect);

    // graphics_objects[1] is the objects name
    text = new Text(name + " module name (text)",
                    graphics_object_locations[MODULE_NAME_TEXT],
                    &secondary_module_color,
                    name,
                    FONT_BOLD);
    graphics_objects.push_back(text);

    // graphics_objects[2] is the remove module button
    button = new Button(name + " remove module (button)",
                        graphics_object_locations[MODULE_REMOVE_MODULE_BUTTON],
                        &secondary_module_color,
                        &primary_module_color,
                        "X",
                        this);
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
    {
        graphics_objects[i]->update_location(graphics_object_locations[i]);
    }
}

/*
 * Set the parameter specified by input num to the value
 * specified by val.
 */
void Module::set(float val, int input_num)
{
    // Set the input and dependency to NULL,
    // the float to val, and the live boolean to false
    inputs[input_num].input = NULL;
    inputs[input_num].val = val;
    inputs[input_num].live = false;
    inputs[input_num].from = NULL;

    adopt_input_colors();

    std::cout << name << " " << parameters.at(module_type).at(input_num)
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
    inputs[input_num].input = &src->output;
    inputs[input_num].live = true;
    inputs[input_num].from = src;
    SELECTING_SRC = false;

    // If this is the output module, update the waveforms to display
    // the proper audio buffers
    if(module_type == OUTPUT)
    {
        Waveform *waveform;
        if(input_num == Output::OUTPUT_INPUT_L)
        {
            waveform = (Waveform *) graphics_objects[Output::OUTPUT_INPUT_L_WAVEFORM];
        }
        else
        {
            waveform = (Waveform *) graphics_objects[Output::OUTPUT_INPUT_R_WAVEFORM];
        }
        waveform->buffer = &src->output;
    }

    // Set the colors of the text box to be the colors of the source module
    adopt_input_colors();

    // Ensure that the input toggle button associated with this input is turned
    // on

    std::cout << name << " " << parameters.at(module_type).at(input_num)
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
    inputs[input_num].input = NULL;
    inputs[input_num].live = false;
    inputs[input_num].from = NULL;

    // If this is the output module, update the waveforms to display
    // an empty audio buffer
    if(module_type == OUTPUT)
    {
        Waveform *waveform;
        if(input_num == Output::OUTPUT_INPUT_L)
        {
            waveform = (Waveform *) graphics_objects[Output::OUTPUT_INPUT_L_WAVEFORM];
        }
        else
        {
            waveform = (Waveform *) graphics_objects[Output::OUTPUT_INPUT_R_WAVEFORM];
        }
        waveform->buffer = NULL;
    }

    adopt_input_colors();

    std::cout << name << " " << parameters.at(module_type).at(input_num)
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
 * Return this module's short name. For example, if this module's name is
 * "oscillator 1", its short name will be "osc 1". A short name is always 5
 * characters long.
 */
std::string Module::get_short_name()
{
    return name.substr(0, 3) + " " + name.substr(name.find(" ") + 1);
}

/*
 * Return a text representation of this module. It should contain any
 * information necessary to reconstruct the module.
 */
std::string Module::get_text_representation()
{
    std::string result;

    result += std::to_string(module_type) + " (" + name + ")" + "\n";
    for(unsigned int i = 0; i < inputs.size(); i ++)
    {
        result += std::to_string(inputs[i].val) + "\n";
    }
    for(unsigned int i = 0; i < inputs.size(); i ++)
        if(inputs[i].from == NULL)
        {
            result += "NULL\n";
        }
        else
        {
            result += inputs[i].from->name + "\n";
        }

    result += this->get_unique_text_representation();

    result += "DONE\n";

    return result;
}

/*
 * Set the colors of all input text boxes and input toggle buttons to match
 * their input modules' colors if applicable. If not, set them to this module's
 * colors.
 */
void Module::adopt_input_colors()
{
    unsigned int dependency_num = 0;

    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
    {
        if(graphics_objects[i]->graphics_object_type == INPUT_TEXT_BOX)
        {
            if(inputs[dependency_num].live)
                ((Input_Text_Box *) graphics_objects[i])->set_colors(
                    &inputs[dependency_num].from->primary_module_color,
                    &inputs[dependency_num].from->secondary_module_color);
            else
            {
                ((Input_Text_Box *) graphics_objects[i])->set_colors(
                    &secondary_module_color,
                    &primary_module_color);
            }
            dependency_num ++;
        }
    }
}

/*
 * If this module's background rectangle receives a click, it means it has been
 * selected as a source for some input on some other module. This means that
 * the following things need to happen:
 *  - set the the input on the other module to take the output from this module
 *  - turn on the input toggle button associated with the input on the other
 *    module
 *  - exit module selection mode
 *  - set the name of this module as the text in the input text box associated
 *    with the input it is now the source for
 *  - set the current input toggle button back to none
 * However, if the type of this module is the output module, don't do anything!
 * The output module cannot be the source for any inputs.
 */
void Module::module_selected()
{
    if(module_type == OUTPUT)
    {
        std::cout << RED_STDOUT
                  << "The output module cannot be the source for any inputs"
                  << DEFAULT_STDOUT << std::endl;
    }
    else
    {
        CURRENT_INPUT_TOGGLE_BUTTON->parent->set(this,
                                                 CURRENT_INPUT_TOGGLE_BUTTON->input_num);
        CURRENT_INPUT_TOGGLE_BUTTON->b = true;
        SELECTING_SRC = false;
        CURRENT_INPUT_TOGGLE_BUTTON->input_text_box->update_current_text(
            get_short_name());
        // If it's the output module, set the waveform's buffer
        if(CURRENT_INPUT_TOGGLE_BUTTON->parent->module_type == OUTPUT)
        {
            // If it's the left input, update the left waveform
            if(CURRENT_INPUT_TOGGLE_BUTTON
               == MODULES[0]->graphics_objects[Output::OUTPUT_INPUT_L_INPUT_TOGGLE_BUTTON])
            {
                ((Waveform *)
                 MODULES[0]->graphics_objects[Output::OUTPUT_INPUT_L_WAVEFORM])->buffer =
                     &this->output;
            }
            // Else, update the right waveform
            else if(CURRENT_INPUT_TOGGLE_BUTTON
                    == MODULES[0]->graphics_objects[Output::OUTPUT_INPUT_R_INPUT_TOGGLE_BUTTON])
            {
                ((Waveform *)
                 MODULES[0]->graphics_objects[Output::OUTPUT_INPUT_R_WAVEFORM])->buffer =
                     &this->output;
            }
        }
        CURRENT_INPUT_TOGGLE_BUTTON = NULL;
    }
}

void Module::render()
{
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
    {
        graphics_objects[i]->render();
    }
}

void Module::clicked()
{
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
    {
        if(graphics_objects[i]->was_clicked())
        {
            graphics_objects[i]->clicked();
        }
    }
}

