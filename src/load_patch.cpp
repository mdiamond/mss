/*
 * Matthew Diamond 2015
 * Functions for loading a patch.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL.h"
#include "SDL_ttf.h"

// Included files
#include "Function_Forwarder.hpp"
#include "main.hpp"
#include "module_utils.hpp"

// Included modules classes
#include "Modules/Adsr.hpp"
#include "Modules/Delay.hpp"
#include "Modules/Filter.hpp"
#include "Modules/Mixer.hpp"
#include "Modules/Multiplier.hpp"
#include "Modules/Noise.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"

/**************
 * LOAD PATCH *
 **************/

void load_patch(std::string filename)
{
    std::ifstream infile;

    infile.open(filename);

    if(infile.is_open())
    {
        infile.close();

        MODULES_CHANGED = true;

        /* std::cout << "Patch " << filename << " loaded" << std::endl; */
        std::cout << "Patch loading not implemented" << std::endl;
    }
    else
    {
        std::cout << RED_STDOUT << "Patch could not be loaded!"
                  << DEFAULT_STDOUT << std::endl;
    }
}

