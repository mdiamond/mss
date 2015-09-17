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
    OUTPUT,
    OSCILLATOR,
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
        SDL_Point upper_left;
        SDL_Color color;
        SDL_Color text_color;
        // A vector containing pointers to any module that must
        // be processed before this module
        std::vector<Module *> depends;
        // A vector containing any graphics objects
        // necessary for rendering this module
        std::vector<Graphics_Object *> graphics_objects;
        // Constructor and destructor
        Module();
        virtual ~Module();
        // Virtual member functions
        //   process() is called during each callback function to
        //   populate the modules output buffer and make it available
        //   to other modules
        virtual void process() = 0;
        //   copy_graphics_data() is called to copy any data in the
        //   module into a nearly duplicate struct so that audio
        //   can be unlocked and rendering can occur while audio is
        //   processing
        virtual void copy_graphics_data() = 0;
        //   calculate_unique_graphics_objects() is called to calculate
        //   the locations of any graphics objects that are unique
        //   to this module type
        virtual void calculate_unique_graphics_objects() = 0;
        // Member functions
        void process_depends();
        Graphics_Object *calculate_border();
        Graphics_Object *calculate_inner_border();
        Graphics_Object *calculate_name();
        void calculate_graphics_objects();
};

#endif
