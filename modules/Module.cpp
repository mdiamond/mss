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

/*******************************
 * MODULE NAME PER MODULE TYPE *
 *******************************/

const std::map<Module::ModuleType, std::string> Module::module_names =
{
    {ADSR, "adsr"},
    {DELAY, "delay"},
    {FILTER, "filter"},
    {INPUT, "input"},
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

const std::map<Module::ModuleType,
               std::vector<std::string> > Module::parameter_names =
{
    {
        ADSR,
        {
            "note on/off",
            "attack",
            "decay",
            "sustain",
            "release"
        }
    },
    {
        DELAY,
        {
            "signal",
            "max delay time",
            "delay time",
            "feedback amount",
            "wet/dry amount"
        }
    },
    {
        FILTER,
        {
            "signal",
            "frequency cutoff",
            "q"
        }
    },
    {
        INPUT,
        {
        }
    },
    {
        MIXER,
        {
            "signal 1",
            "signal 1 multiplier",
            "signal 2",
            "signal 2 multiplier",
            "signal 3",
            "signal 3 multiplier",
            "signal 4",
            "signal 4 multiplier",
            "signal 5",
            "signal 5 multiplier"
        }
    },
    {
        MULTIPLIER,
        {
            "signal",
            "signal multiplier",
            "dry/wet amount"
        }
    },
    {
        NOISE,
        {
            "range low",
            "range high"
        }
    },
    {
        OSCILLATOR,
        {
            "frequency",
            "phase offset",
            "pulse width",
            "range low",
            "range high"
        }
    },
    {
        OUTPUT,
        {
            "input left",
            "input right"
        }
    },
    {
        SAH,
        {
            "signal",
            "hold time"
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
        module_names.at(_module_type) + " "
        + std::to_string(find_available_module_number(_module_type)),
        MODULE, NULL, find_module_location(find_available_module_slot()),
        BLACK),
    module_type(_module_type), number(find_available_module_slot()),
    processed(false),
    inputs(std::vector<Parameter>(parameter_names.at(_module_type).size())),
    out(std::vector<float>(BUFFER_SIZE))
{
    if(COLORBLIND_ON)
    {
        primary_module_color = WHITE;
        secondary_module_color = BLACK;
    }
    else
    {
        // Set this module's color randomly, but with enough contrast
        std::vector<SDL_Color> colors = generate_module_colors();
        primary_module_color = colors[0];
        secondary_module_color = colors[1];
    }

    std::cout << "Module \"" << name << "\" created" << std::endl;
}

/*
 * Destructor.
 */
Module::~Module()
{
    // Lock audio to avoid interfering with processing
    SDL_LockAudio();

    // Remove this module from the vector of modules
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != NULL && MODULES[i] == this)
        {
            MODULES[i] = NULL;
        }
    }

    // Cancel any inputs that this module is outputting to
    for(auto it1 = MODULES.begin(); it1 != MODULES.end(); it1 ++)
    {
        if(*it1 != NULL)
        {
            for(auto it2 = (*it1)->inputs.begin(); it2 != (*it1)->inputs.end(); it2 ++)
            {
                if((*it2).from == this)
                {
                    std::cout << it2 - (*it1)->inputs.begin() << std::endl;
                    (*it1)->cancel_input(it2 - (*it1)->inputs.begin());
                }
            }
        }
    }

    // Erase this module from the vector of modules
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != NULL && MODULES[i] == this)
        {
            MODULES.erase(MODULES.begin() + i);
        }
    }

    // Delete all graphics objects
    for(auto it = graphics_objects.begin();
        it != graphics_objects.end(); it ++)
    {
        delete it->second;
    }

    // Mark modules changed so that they will be re-rendered
    MODULES_CHANGED = true;

    std::cout << "Module \"" << name << "\" removed" << std::endl;

    // Unlock audio
    SDL_UnlockAudio();
}

/*
 * Get the contents of the text box that the user interacted with (either a
 * float or a string of text), and attempt to set the input value to the float
 * or set the input to receive values from the module name that may have been
 * entered.
 */
void Module::handle_text_box_event(Text_Box *text_box)
{
    int input_num = text_box_to_input_num[text_box];

    // If the text box contains a float value, use that to set the value for
    // the input/parameter associated with the text box
    if(text_box->is_float)
    {
        set(input_num, text_box->as_float);
    }
    // Otherwise, assume that the text box contains the name of another module
    // to output to the input associated with the text box
    else
    {
        Module *src = find_module(&text_box->text.text);
        // If the module is not found, inform the user, take no action
        if(src == nullptr)
        {
            std::cout << RED_STDOUT << "Input could not be set, no such module"
                      << DEFAULT_STDOUT << std::endl;
        }
        // If the user is attempting to receive input from the output module,
        // inform the user that it is not possible, take no action
        else if(src == MODULES[0])
        {
            std::cout << RED_STDOUT
                      << "The output module does not output any signals "
                      "accessible within the context of this software"
                      << DEFAULT_STDOUT << std::endl;
        }
        // If the user is attempting to have a module output to itself, inform
        // the user that it is not possible, take no action
        else if(src == this)
        {
            std::cout << RED_STDOUT << "No module may output to itself"
                      << DEFAULT_STDOUT << std::endl;
        }
        // Otherwise, a module has been found, and it should be able to input
        // to this module properly, so set the input associated with input_num
        // to receive input from the module, update the associated toggle
        // button, and update the text box text to show the short module name
        else
        {
            Toggle_Button *toggle_button = inputs[input_num].toggle_button;

            set(input_num, src);
            if(toggle_button != nullptr)
            {
                toggle_button->b = true;
            }
            text_box->update_current_text(
                text_box->text.text.substr(0, 3) + " "
                + text_box->text.text.substr(text_box->text.text.find(" ") + 1));
        }
    }
}

/*
 * If the toggle button that the user interacted with is associated with a
 * currently live input, cancel input. Otherwise, enable module selection mode.
 */
void Module::handle_toggle_button_event(Toggle_Button *toggle_button)
{
    // Determine the input number and text box associated with this toggle
    // button
    int input_num = toggle_button_to_input_num[toggle_button];
    Text_Box *text_box = inputs[input_num].text_box;

    // Input is live, cancel input, reset associated text box and toggle button
    if(inputs[input_num].live)
    {
        cancel_input(input_num);
        text_box->update_current_text(text_box->prompt_text.text);
        if(!(text_box->prompt_text.text == "input"))
        {
            std::string input_val_str = std::to_string(inputs[input_num].val);
            text_box->update_current_text(input_val_str);
        }
        toggle_button->b = false;
    }
    // Input is not live, enable module selection mode, keep track of which
    // toggle button was clicked, and the module it is associated with
    else
    {
        SELECTING_SRC = !SELECTING_SRC;
        CURRENT_TOGGLE_BUTTON = toggle_button;
        SELECTING_FOR_MODULE = this;
    }
}

/*
 * This function allows the module to do something in response to user
 * interaction with a contained graphics object. Any class that inherits from
 * the module class may implement its own version, but generally, even if this
 * function is overwritten, it may need to be called to handle certain actions
 * before checking for module specific actions. Return true if any action was
 * taken based on the user interaction, false otherwise.
 */
bool Module::handle_event(Graphics_Object *g)
{
    // If g is null, take no action, return false
    if(g == nullptr)
    {
        return false;
    }
    // If g is the background rect and select source mode is not active,
    // take no action, return false
    else if(g == graphics_objects["background rect"]
            && !SELECTING_SRC)
    {
        return false;
    }
    // If the background rect is clicked and select source mode is active,
    // attempt to set this module as the source for an input to another module,
    // return true if successful, false otherwise
    else if(g == graphics_objects["background rect"])
    {
        return module_selected();
    }
    // If this is not the output module and the remove module button is
    // pressed, delete this module, return true
    else if(module_type != OUTPUT
            && g == graphics_objects["remove module button"])
    {
        delete this;
        return true;
    }
    // If a text box is interacted with, and we aren't in select source mode,
    // go get its contents and figure out what to do with them
    else if(g->graphics_object_type == TEXT_BOX && !SELECTING_SRC)
    {
        handle_text_box_event((Text_Box *) g);
        return true;
    }
    // If a toggle button is interacted with, and we aren't in select source
    // mode, either enable select source mode or cancel input, depending on the
    // current state of the input
    else if(g->graphics_object_type == TOGGLE_BUTTON && !SELECTING_SRC)
    {
        handle_toggle_button_event((Toggle_Button *) g);
        return true;
    }

    // If none of the above, return false
    return false;
}

/*
 * This function determines the locations of this module's graphics objects
 * based on how many inputs are detected for this module type. This is the
 * defualt implementation, but derived module classes may override it to have
 * more customized interfaces.
 */
void Module::calculate_unique_graphics_object_locations()
{
    SDL_Rect location;

    // Waveform viewer location
    location = {upper_left.x, upper_left.y + 15, MODULE_WIDTH, 34};
    graphics_object_locations["waveform"] = location;

    location.y += 37;

    // Text, input text box, and input toggle button locations
    for(unsigned int i = 0; i < inputs.size(); i++)
    {
        location = {upper_left.x + 2, location.y, 0, 0};
        graphics_object_locations[parameter_names.at(module_type).at(i) + " text"] =
            location;
        location = {upper_left.x, location.y + 10, MODULE_WIDTH - 8, 9};
        graphics_object_locations[parameter_names.at(module_type).at(i) + " text box"] =
            location;
        location = {location.x + location.w + 1, location.y, 7, 9};
        graphics_object_locations[parameter_names.at(module_type).at(i) + " toggle button"] =
            location;
        location.y += 10;
    }
}

/*
 * This function initializes this modules graphics objects based on the
 * locations determined in calculate_unique_graphics_locations(). This is the
 * default implementation, but derived module classes may override it to have
 * more customized interfaces.
 */
void Module::initialize_unique_graphics_objects()
{
    std::string parameter_name;

    // Initialize waveform viewer
    graphics_objects["waveform"] =
        new Waveform(name + " waveform",
                     graphics_object_locations["waveform"],
                     primary_module_color, secondary_module_color, &out);

    // Initialize text, text box, and toggle button for this parameter/input
    for(unsigned int i = 0; i < parameter_names.at(module_type).size(); i ++)
    {
        parameter_name = parameter_names.at(module_type).at(i);
        Text *text = new Text(
            name + " " + parameter_name + " text",
            graphics_object_locations[parameter_name + " text"],
            secondary_module_color, parameter_name + ":");

        Text_Box *text_box = new Text_Box(
            name + " " + parameter_name + " text box",
            graphics_object_locations[parameter_name + " text box"],
            secondary_module_color, primary_module_color, "# or input", this);

        Toggle_Button *toggle_button = new Toggle_Button(
            name + " " + parameter_name + " toggle button",
            graphics_object_locations[parameter_name + " toggle button"],
            secondary_module_color, primary_module_color,
            primary_module_color, secondary_module_color, "I", "I", false,
            this);

        graphics_objects[parameter_name + " text"] = text;
        graphics_objects[parameter_name + " text box"] = text_box;
        graphics_objects[parameter_name + " toggle button"] = toggle_button;

        text_box_to_input_num[text_box] = i;
        toggle_button_to_input_num[toggle_button] = i;
        inputs[i].text_box = text_box;
        inputs[i].toggle_button = toggle_button;
    }
}

/*
 * This function calls upon this module's dependencies
 * to process samples.
 */
void Module::process_dependencies()
{
    for(unsigned int i = 0; i < inputs.size(); i ++)
    {
        if(inputs[i].live && !inputs[i].from->processed)
        {
            inputs[i].from->process();
        }
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
    {
        if(inputs[j].live)
        {
            inputs[j].val = inputs[j].in->at(i);
        }
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

    graphics_object_locations["background rect"] =
        {upper_left.x, upper_left.y, MODULE_WIDTH, MODULE_HEIGHT};
    graphics_object_locations["module selection rect"] =
        graphics_object_locations["background rect"];
    graphics_object_locations["name text"] =
        {upper_left.x + 2, upper_left.y + 3, 0, 0};
    graphics_object_locations["remove module button"] =
        {upper_left.x + MODULE_WIDTH - 7, upper_left.y, 7, 9};

    calculate_unique_graphics_object_locations();
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

    // the background rectangle
    rect = new Rect(name + " background rect",
                    graphics_object_locations["background rect"],
                    primary_module_color,
                    this);
    graphics_objects["background rect"] = rect;

    // the module selection rectangle
    rect = new Rect(name + " module selection rect",
                    graphics_object_locations["module selection rect"],
                    BLACK,
                    this);
    graphics_objects["module selection rect"] = rect;
    rect->color.a = 127;

    // the name of the object
    text = new Text(name + " module name text",
                    graphics_object_locations["name text"],
                    secondary_module_color,
                    name);
    graphics_objects["name text"] = text;

    // the remove module button
    button = new Button(name + " remove module button",
                        graphics_object_locations["remove module button"],
                        secondary_module_color,
                        primary_module_color,
                        "X",
                        this);
    graphics_objects["remove module button"] = button;

    // Initialize all graphics objects specific to this module type
    initialize_unique_graphics_objects();

    // Mark this function as complete so that running it again can easily be avoided
    graphics_objects_initialized = true;
}

/*
 * Update the locations of all graphics objects.
 */
void Module::update_graphics_object_locations()
{
    // Calculate graphics object locations
    calculate_graphics_object_locations();

    // Update graphics object locations
    for(auto it = graphics_objects.begin(); it != graphics_objects.end(); it ++)
    {
        it->second->update_location(graphics_object_locations[it->first]);
    }
}

/*
 * Set the parameter specified by input num to the value
 * specified by val.
 */
void Module::set(int input_num, float val)
{
    // Set the input and dependency to NULL,
    // the float to val, and the live boolean to false
    inputs[input_num].in = NULL;
    inputs[input_num].val = val;
    inputs[input_num].live = false;
    inputs[input_num].from = NULL;

    // Reset the input toggle button associated with this text box, if
    // applicable (some inputs do not allow live value updating)
    if(inputs[input_num].toggle_button != nullptr)
    {
        inputs[input_num].toggle_button->b = false;
    }

    adopt_input_colors();

    std::cout << name << " " << parameter_names.at(module_type).at(input_num)
              << " changed to " << val << std::endl;
}

/*
 * Set the parameter specified to be updated by the output of
 * the module specified.
 */
void Module::set(int input_num, Module *src)
{
    // Set the input to the output of src, the dependency to src,
    // the live boolean to true
    inputs[input_num].in = &src->out;
    inputs[input_num].live = true;
    inputs[input_num].from = src;

    // If this is the output module, update the waveforms to display
    // the proper audio buffers
    if(module_type == OUTPUT)
    {
        Waveform *waveform;
        if(input_num == Output::OUTPUT_INPUT_L)
        {
            waveform = (Waveform *) graphics_objects["waveform left"];
        }
        else
        {
            waveform = (Waveform *) graphics_objects["waveform right"];
        }
        waveform->buffer = &src->out;
    }

    // Set the colors of the text box to be the colors of the source module
    adopt_input_colors();

    // Ensure that the input toggle button associated with this input is turned
    // on

    std::cout << name << " " << parameter_names.at(module_type).at(input_num)
              << " is now coming from " << src->name << std::endl;
}

/*
 * Cancel input from another module, reverting to a constant value for the
 * parameter specified by input num. The value will stay at whatever it was
 * most recently.
 */
void Module::cancel_input(int input_num)
{
    Text_Box *text_box =
        ((Text_Box *) graphics_objects[parameter_names.at(module_type).at(input_num) + " text box"]);
    Toggle_Button *toggle_button =
        ((Toggle_Button *) graphics_objects[parameter_names.at(module_type).at(input_num) + " toggle button"]);

    // Set the input and dependency to NULL,
    // and the live boolean to false
    inputs[input_num].in = nullptr;
    inputs[input_num].live = false;
    inputs[input_num].from = nullptr;

    // Reset the input text box and input toggle button associated with this
    // input
    if(text_box->prompt_text.text == "input")
    {
        text_box->update_current_text("");
    }
    else
    {
        text_box->update_current_text(std::to_string(inputs[input_num].val));
    }
    toggle_button->b = false;

    // If this is the output module, update the waveforms to display
    // an empty audio buffer
    if(module_type == OUTPUT)
    {
        Waveform *waveform;
        if(input_num == Output::OUTPUT_INPUT_L)
        {
            waveform = (Waveform *) graphics_objects["waveform left"];
        }
        else
        {
            waveform = (Waveform *) graphics_objects["waveform right"];
        }
        waveform->buffer = nullptr;
    }

    adopt_input_colors();

    std::cout << name << " " << parameter_names.at(module_type).at(input_num)
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
 * Return this modules short name. For example, if this module's name is
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
 * their input modules colors if applicable. If not, set them to this module's
 * colors.
 */
void Module::adopt_input_colors()
{
    unsigned int dependency_num;

    for(auto it = graphics_objects.begin(); it != graphics_objects.end();
        it ++)
    {
        if(it->second->graphics_object_type == TEXT_BOX)
        {
            dependency_num = text_box_to_input_num[((Text_Box *) it->second)];
            if(inputs[dependency_num].live)
            {
                ((Text_Box *) it->second)->set_colors(
                    inputs[dependency_num].from->primary_module_color,
                    inputs[dependency_num].from->secondary_module_color);
            }
            else
            {
                ((Text_Box *) it->second)->set_colors(
                    secondary_module_color, primary_module_color);
            }
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
 * The output module cannot be the source for any inputs. Return true if action
 * was taken, false otherwise.
 */
bool Module::module_selected()
{
    if(module_type == OUTPUT)
    {
        std::cout << RED_STDOUT
                  << "The output module cannot be the source for any inputs"
                  << DEFAULT_STDOUT << std::endl;
        return false;
    }
    else if(SELECTING_FOR_MODULE == this)
    {
        std::cout << RED_STDOUT << "No module may output to itself"
                  << DEFAULT_STDOUT << std::endl;
        return false;
    }
    else
    {
        int input_num = SELECTING_FOR_MODULE->toggle_button_to_input_num[CURRENT_TOGGLE_BUTTON];

        SELECTING_FOR_MODULE->set(input_num, this);
        CURRENT_TOGGLE_BUTTON->b = true;
        SELECTING_SRC = false;
        SELECTING_FOR_MODULE->inputs[input_num].text_box->update_current_text(
            get_short_name());
        // If it's the output module, set the waveform's buffer
        if(SELECTING_FOR_MODULE->module_type == OUTPUT)
        {
            // If it's the left input, update the left waveform
            if(CURRENT_TOGGLE_BUTTON
               == MODULES[0]->graphics_objects["input left toggle button"])
            {
                ((Waveform *)
                 MODULES[0]->graphics_objects["waveform left"])->buffer =
                     &this->out;
            }
            // Else, update the right waveform
            else if(CURRENT_TOGGLE_BUTTON
                    == MODULES[0]->graphics_objects["input right toggle button"])
            {
                ((Waveform *)
                 MODULES[0]->graphics_objects["waveform right"])->buffer =
                     &this->out;
            }
        }
        CURRENT_TOGGLE_BUTTON = nullptr;
        SELECTING_FOR_MODULE = nullptr;
        return true;
    }
}

/*
 * Render the graphics objects that represent this module. Always ensure that
 * the background rectangle renders first, and then does not render again while
 * iterating through the vector of graphics objects. Also ensure that if
 * selecting source mode is active and we are not currently hovering over this
 * module, we render the module selection rectangle to partially hide the
 * module, and that it does not render when iterating through the vector of
 * graphics objects.
 */
void Module::render()
{
    graphics_objects["background rect"]->render();
    for(auto it = graphics_objects.begin(); it != graphics_objects.end();
        it ++)
    {
        if(it->first != "background rect"
           && it->first != "module selection rect")
        {
            it->second->render();
        }
    }
    if(SELECTING_SRC
       && !graphics_objects["module selection rect"]->mouse_over())
    {
        graphics_objects["module selection rect"]->render();
    }
}

/*
 * Pass along a click to the specific graphics object that has been clicked.
 * Graphics objects should only respond to clicks if selecting source mode is
 * inactive, or if it is the background rectangle (clicks of the background
 * rectangle are how source selection happens).
 */
bool Module::clicked()
{
    for(auto it = graphics_objects.begin(); it != graphics_objects.end();
        it ++)
    {
        if(it->second->mouse_over())
        {
            // Only respond to a click if it's not selecting source mode, or if
            // a background rectangle is being clicked
            if(!SELECTING_SRC || it->second->graphics_object_type == RECT)
            {
                if(it->second->clicked())
                {
                    return true;
                }
            }
        }
    }
    return false;
}

