/*
 * Matthew Diamond 2015
 * The header for a base class to represent a generic synthesizer module.
 * Other classes will extend this class and represent different
 * types of modules. This file defines the class and includes any files or
 * libraries necessary for Module.cpp.
 * The following classes are derived from the Module class:
 *   - Output
 *   - Oscillator
 */

#ifndef synth_module_h
#define synth_module_h

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

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
        SDL_Point upper_left;
        SDL_Rect border;
        SDL_Rect inner_border;
        SDL_Color color;
        // A vector containing pointers to any module that must
        // be processed before this module
        std::vector<Module *> depends;
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
        //   render() is called each time a new frame is needed for
        //   the window to display, it should render the module in
        //   the window
        virtual void render() = 0;
        // Member functions
        void process_depends();
        void calculate_upper_left(int);
        void render_border();
        void render_inner_border();
};

#endif
