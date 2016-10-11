/*
 * Matthew Diamond 2015
 * Functions for saving a patch.
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
#include "main.hpp"

// Included modules classes
#include "Module.hpp"
#include "Modules/Mixer.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"
#include "Modules/Multiplier.hpp"

/**************
 * SAVE PATCH *
 **************/

void save_patch(std::string filename)
{
    std::ofstream outfile;

    outfile.open(filename);

    if(outfile.is_open())
    {
        outfile.close();

        /* std::cout << "Patch " << filename << " saved" << std::endl; */
        std::cout << "Patch saving not implemented" << std::endl;
    }
    else
        std::cout << RED_STDOUT << "Patch could not be saved!" << DEFAULT_STDOUT << std::endl;
}

