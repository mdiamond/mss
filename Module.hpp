/*
 * Matthew Diamond 2015
 * The header for a base class to represent a generic synthesizer module.
 * Other classes will extend this class and represent different
 * types of modules.
 * The following classes are derived from the Module class:
 *   - ADSR
 *   - Delay
 *   - Filter
 *   - Mixer
 *   - Multiplier
 *   - Noise
 *   - Oscillator
 *   - Output
 */

#ifndef SYNTH_MODULE_HPP
#define SYNTH_MODULE_HPP

/************
 * INCLUDES *
 ************/

// Included libraries
#include <map>

// Included SDL components
#include "SDL.h"
#include "SDL_ttf.h"

// Included classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Waveform.hpp"

class Module: public Graphics_Object
{
public:
    // Module type enum
    enum ModuleType
    {
        ADSR = 0,
        DELAY,
        FILTER,
        MIXER,
        MULTIPLIER,
        NOISE,
        OSCILLATOR,
        OUTPUT,
        SAH
    };

    // Module graphics objects enum, these are common to all modules
    enum ModuleGraphicsObjects
    {
        MODULE_BACKGROUND_RECT = 0,
        MODULE_NAME_TEXT,
        MODULE_REMOVE_MODULE_BUTTON
    };

    // A struct to represent an input parameter for a module
    struct Input
    {
        // Module that is producing the data to be received
        Module *from;
        // Output buffer of the above module
        std::vector<float> *input;
        // Instantaneous value of the input
        float val;
        // Instantaneous value of the input as a string
        std::string val_str;
        // Whether or not this input is currently live
        bool live;
    };

    // Maps of useful information about modules, defined in
    // Module.cpp
    //   Map of the names per module type
    static const std::map<ModuleType, std::string> names;
    //   Map of the parameters per module type
    static const std::map<ModuleType, std::vector<std::string> > parameters;

    // Module information
    ModuleType module_type;
    SDL_Color primary_module_color;
    SDL_Color secondary_module_color;
    int number;
    bool processed;
    SDL_Point upper_left;
    bool graphics_objects_initialized;
    // A vector containing any graphics objects
    // necessary for rendering this module, and
    // their current locations
    std::vector<SDL_Rect> graphics_object_locations;
    std::vector<Graphics_Object *> graphics_objects;
    // A vector of inputs, accessed for any processing operations
    // that depend on the output of other modules
    std::vector<Input> inputs;
    // Output buffer
    std::vector<float> output;

    // Constructor and destructor
    Module(ModuleType);
    virtual ~Module();

    // Virtual member functions
    //   Process audio for the output buffer
    virtual void process() = 0;
    //   Update parameters at the k rate
    virtual void update_control_values() = 0;
    //   Calculate the locations of graphics objects unique to this module type
    virtual void calculate_unique_graphics_object_locations() = 0;
    //   Initialize the graphics objects unique to this module type
    virtual void initialize_unique_graphics_objects() = 0;
    //   Output the module's unique information as text
    virtual std::string get_unique_text_representation() = 0;
    //   Handle a button press
    virtual void button_function(Button *) = 0;
    //   Handle a toggle button press
    virtual void toggle_button_function(Toggle_Button *) = 0;

    // Member functions
    //   Process all modules that this module depends upon
    void process_dependencies();
    //   Grab samples from index i in all input buffers, store them as
    //   individual floats
    void update_input_vals(int);
    //   Calculate the locations of all graphics objects in this module
    void calculate_graphics_object_locations();
    //   Initialize input text box graphics objects
    void initialize_input_text_box_objects(std::vector<std::string>,
                                           std::vector<SDL_Rect>,
                                           std::vector<SDL_Color *>,
                                           std::vector<SDL_Color *>,
                                           std::vector<std::string>,
                                           std::vector<TTF_Font *>,
                                           std::vector<Module *>,
                                           std::vector<int>,
                                           std::vector<Input_Toggle_Button *>);
    //   Initialize input toggle button graphics objects
    //   (must always be used immediately after, and with the same size arrays for input
    //    as initialize_input_text_box_objects() because each input toggle button depends
    //    on and must be able to reference a particular input text object)
    void initialize_input_toggle_button_objects(std::vector<std::string>,
                                                std::vector<SDL_Rect>,
                                                std::vector<SDL_Color *>,
                                                std::vector<SDL_Color *>,
                                                std::vector<SDL_Color *>,
                                                std::vector<SDL_Color *>,
                                                std::vector<TTF_Font *>,
                                                std::vector<std::string>,
                                                std::vector<std::string>,
                                                std::vector<bool>,
                                                std::vector<Module *>,
                                                std::vector<int>,
                                                std::vector<Input_Text_Box *>);
    //   Initialize all graphics objects in this module
    void initialize_graphics_objects();
    //   Update the locations of all graphics objects in this module
    void update_graphics_object_locations();
    //   Set a parameter to a certain value
    void set(float, int);
    //   Set a parameter to be updated by another module's output
    void set(Module *, int);
    //   Cancel input for a certain parameter
    void cancel_input(int);
    //   Return module name
    std::string get_name();
    //   Return module short name
    std::string get_short_name();
    //   Output the module as text
    std::string get_text_representation();
    //   Make this module's input stuff adopt the colors of whatever is
    //   outputting to it
    void adopt_input_colors();
    //   Handle a background rectangle click (this module has been selected
    //   as a source for some other module's input)
    void module_selected();
    //   Render this module to the window
    void render();
    //   Handle a click
    void clicked();
};

#endif
