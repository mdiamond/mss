/*
 * Matthew Diamond 2015
 * This file contains functions for populating the wavetables.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "main.hpp"

using namespace std;

/********************
 * HELPER FUNCTIONS *
 ********************/

void populate_sin()
{
    for(unsigned int i = 0; i < SIN_WAVE->size(); i ++)
        (*SIN_WAVE)[i] = sin(((float) i / SAMPLE_RATE) * (2 * M_PI));
}

void populate_tri()
{
    for(unsigned int i = 0; i < TRI_WAVE->size(); i ++)
    {
        float one_fourth = (TRI_WAVE->size() / 4);
        float half = (TRI_WAVE->size() / 2);
        float three_fourths = one_fourth * 3;

        if(i < one_fourth)
            (*TRI_WAVE)[i] = i / one_fourth;
        else if(i < half)
            (*TRI_WAVE)[i] = (1 - ((i - one_fourth) / (one_fourth)));
        else if(i < three_fourths)
            (*TRI_WAVE)[i] = ((i - half) / one_fourth) * -1;
        else
            (*TRI_WAVE)[i] = ((1 - ((i - three_fourths) / (one_fourth))) * -1);
    }
}

/***********************
 * POPULATE WAVETABLES *
 ***********************/

void populate_wavetables()
{
    populate_sin();
    populate_tri();

    cout << "Wavetables populated." << endl;
}
