/*
 * Matthew Diamond 2015
 * Functions for saving a patch.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "main.hpp"

// Included classes
#include "Module.hpp"
#include "Modules/Mixer.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"
#include "Modules/Multiplier.hpp"

/**************
 * SAVE PATCH *
 **************/

void save_patch()
{
    std::string patch;

    for(unsigned int i = 0; i < MODULES.size(); i ++)
        patch += MODULES[i]->text_representation();

    std::cout << patch << std::endl;
}
