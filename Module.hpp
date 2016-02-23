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

#ifndef synth_module_h
#define synth_module_h

/************
 * INCLUDES *
 ************/

// Included libraries
#include <map>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Input_Text_Box.hpp"
#include "Graphics_Objects/Input_Toggle_Button.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Text.hpp"
#include "Graphics_Objects/Waveform.hpp"

/*************************
 * TYPES OF MODULES ENUM *
 *************************/

enum Modules
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

/********************************
 * MODULE GRAPHICS OBJECTS ENUM *
 ********************************/

enum Module_Graphics_Objects
{
    MODULE_BACKGROUND_RECT = 0,
    MODULE_NAME_TEXT,
    MODULE_REMOVE_MODULE_BUTTON
};

/***************************
 * MODULE CLASS DEFINITION *
 ***************************/

class Module
{
    public:
        // Module information
        std::string name;
        int type;
        int number;
        bool processed;
        SDL_Point upper_left;
        SDL_Color color;
        SDL_Color text_color;
        bool graphics_objects_initialized;
        // A vector containing pointers to any module that must
        // be processed before this module
        std::vector<Module *> dependencies;
        // A vector containing any graphics objects
        // necessary for rendering this module, and
        // their current locations
        std::vector<SDL_Rect> graphics_object_locations;
        std::vector<Graphics_Object *> graphics_objects;
        // Vectors containing representations of the inputs as
        // floats, strings and input buffers, as well as booleans
        // representing whether or not that input is a live signal
        std::vector<float> input_floats;
        std::vector<std::string> input_strs;
        std::vector<std::vector<float> *> inputs;
        std::vector<bool> inputs_live;
        // Output buffer
        std::vector<float> output;

        // Constructor and destructor
        Module(int);
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

        // Member functions
        //   Process all modules that this module depends upon
        void process_dependencies();
        //   Calculate the locations of all graphics objects in this module
        void calculate_graphics_object_locations();
        //   Initialize input text box graphics objects
        void initialize_input_text_box_objects(std::vector<std::string>, std::vector<SDL_Rect>, std::vector<SDL_Color *>,
                                               std::vector<SDL_Color *>,std::vector<std::string>, std::vector<TTF_Font *>,
                                               std::vector<Module *>, std::vector<int>);
        //   Initialize input toggle button graphics objects
        //   (must always be used immediately after, and with the same size arrays for input
        //    as initialize_input_text_box_objects() because each input toggle button depends
        //    on and must be able to reference a particular input text object)
        void initialize_input_toggle_button_objects(std::vector<std::string>, std::vector<SDL_Rect>, std::vector<SDL_Color *>,
                                                    std::vector<SDL_Color *>, std::vector<SDL_Color *>, std::vector<SDL_Color *>,
                                                    std::vector<TTF_Font *>,
                                                    std::vector<std::string>, std::vector<std::string>,
                                                    std::vector<bool>, std::vector<Module *>, std::vector<int>);
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
        //   Make this module's input stuff adopt the colors of whatever is outputting to it
        void adopt_input_colors();
};

#endif
