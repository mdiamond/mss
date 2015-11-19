/*
 * Matthew Diamond 2015
 * The header for a base class to represent a generic synthesizer module.
 * Other classes will extend this class and represent different
 * types of modules. This file defines the class.
 * The following classes are derived from the Module class:
 *   - Output
 *   - Oscillator
 */

#ifndef synth_module_h
#define synth_module_h

/************
 * INCLUDES *
 ************/

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included classes
#include "Graphics_Object.hpp"
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
    OUTPUT = 0,
    MIXER,
    OSCILLATOR,
    VCA
};

/********************************
 * MODULE GRAPHICS OBJECTS ENUM *
 ********************************/

enum Module_Graphics_Objects
{
    MODULE_BORDER_RECT = 0,
    MODULE_INNER_BORDER_RECT,
    MODULE_NAME_TEXT,
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
        std::vector<std::string> parameter_names;
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
        virtual void process() = 0;
        virtual void update_control_values() = 0;
        virtual void calculate_unique_graphics_object_locations() = 0;
        virtual void initialize_unique_graphics_objects() = 0;

        // Member functions
        void process_dependencies();
        void calculate_graphics_object_locations();
        void initialize_input_text_box_objects(std::vector<std::string>, std::vector<SDL_Rect>, std::vector<SDL_Color>,
                                               std::vector<SDL_Color>,std::vector<std::string>, std::vector<TTF_Font *>,
                                               std::vector<Module *>, std::vector<int>);
        void initialize_input_toggle_button_objects(std::vector<std::string>, std::vector<SDL_Rect>, std::vector<SDL_Color>,
                                                    std::vector<SDL_Color>, std::vector<SDL_Color>, std::vector<SDL_Color>,
                                                    std::vector<TTF_Font *>,
                                                    std::vector<std::string>, std::vector<std::string>,
                                                    std::vector<bool>, std::vector<Module *>, std::vector<int>);
        void initialize_graphics_objects();
        void update_graphics_object_locations();
        void set(float, int);
        void set(Module *, int);
        void cancel_input(int);
};

#endif
