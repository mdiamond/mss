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

// Included classes
#include "Graphics_Object.hpp"

/*************************
 * TYPES OF MODULES ENUM *
 *************************/

enum Modules
{
    OUTPUT = 0,
    OSCILLATOR,
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
        unsigned short current_sample;
        SDL_Point upper_left;
        SDL_Color color;
        SDL_Color text_color;
        // A vector containing pointers to any module that must
        // be processed before this module
        std::vector<Module *> *depends;
        // A vector containing any graphics objects
        // necessary for rendering this module
        std::vector<Graphics_Object *> *graphics_objects;
        // Output buffer
        std::vector<float> *output;

        // Constructor and destructor
        Module();
        virtual ~Module();

        // Virtual member functions
        //   process() is called during each callback function to
        //   populate the modules output buffer and make it available
        //   to other modules
        virtual void process() = 0;        //   update_control_values() is called during each k rate callback
        //   to make sure that the modules all get their most up to date control
        //   values in order to synthesize sound properly
        virtual void update_unique_control_values() = 0;
        //   calculate_unique_graphics_objects() is called to calculate
        //   the locations of any graphics objects that are unique
        //   to this module type
        virtual void calculate_unique_graphics_objects() = 0;
        virtual void update_unique_graphics_objects() = 0;

        // Member functions
        void process_depends();
        void calculate_upper_left();
        void update_control_values();
        void calculate_graphics_objects();
        void update_graphics_objects();
};

#endif
