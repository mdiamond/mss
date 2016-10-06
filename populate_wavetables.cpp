/*
 * Matthew Diamond 2015
 * Functions for populating the wavetables.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <vector>

// Included SDL components
#include "SDL.h"
#include "SDL_ttf.h"

// Included files
#include "main.hpp"

// Included classes
#include "Modules/Oscillator.hpp"

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * Populate the sine wave wavetable with a 1 second long, 1 Hz waveform.
 */
void populate_sin()
{
    for(unsigned int i = 0; i < WAVETABLES[SIN].size(); i ++)
        WAVETABLES[SIN][i] = sin(((float) i / SAMPLE_RATE) * (2 * M_PI));
}

/*
 * Populate the triangle wave wavetable with a 1 second long, 1 Hz waveform.
 */
void populate_tri()
{
    for(unsigned int i = 0; i < WAVETABLES[TRI].size(); i ++)
    {
        float one_fourth = (WAVETABLES[TRI].size() / 4);
        float half = (WAVETABLES[TRI].size() / 2);
        float three_fourths = one_fourth * 3;

        if(i < one_fourth)
            WAVETABLES[TRI][i] = i / one_fourth;
        else if(i < half)
            WAVETABLES[TRI][i] = (1 - ((i - one_fourth) / (one_fourth)));
        else if(i < three_fourths)
            WAVETABLES[TRI][i] = ((i - half) / one_fourth) * -1;
        else
            WAVETABLES[TRI][i] = ((1 - ((i - three_fourths) / (one_fourth))) * -1);
    }
}

/*
 * Populate the saw wave wavetable with a 1 second long, 1 Hz waveform.
 */
void populate_saw()
{
    for(unsigned int i = 0; i < WAVETABLES[SAW].size(); i ++)
    {
        float half = (WAVETABLES[SAW].size() / 2);

        if(i < half)
            WAVETABLES[SAW][i] = -1 * (1 - (i / half));
        else
            WAVETABLES[SAW][i] = -1 * (0 - ((i - half) / half));
    }
}

/*
 * Populate the square wave wavetable with a 1 second long, 1 Hz waveform.
 */
void populate_sqr()
{
    for(unsigned int i = 0; i < WAVETABLES[SQR].size(); i ++)
    {
        float half = (WAVETABLES[SQR].size() / 2);

        if(i < half)
            WAVETABLES[SQR][i] = 1;
        else
            WAVETABLES[SQR][i] = -1;
    }
}

/***********************
 * POPULATE WAVETABLES *
 ***********************/

/*
 * Populate all wavetables with 1 second long, 1 Hz waveforms.
 */
void populate_wavetables()
{
    populate_sin();
    populate_tri();
    populate_saw();
    populate_sqr();

    std::cout << "Wavetables populated." << std::endl;
}
