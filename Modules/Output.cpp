/*
 * Matthew Diamond 2015
 * Member functions for the Output class.
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

// Included files
#include "../image_processing.hpp"
#include "../main.hpp"

// Included classes
#include "../Module.hpp"
#include "Output.hpp"

using namespace std;

/***************************
 * OUTPUT MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Output::Output(int _number)
{
    name = "output";
    type = OUTPUT;
    number = _number;

    this->audio.input_l = new vector<float>(BUFFER_SIZE, 0);
    this->audio.input_r = new vector<float>(BUFFER_SIZE, 0);
}

/*
 * Dummy function.
 */
Output::~Output()
{

}

/*
 * This function simply calls upon dependencies for
 * processing. The output module depends on all other
 * modules.
 */
void Output::process()
{
    process_depends();
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Output::calculate_unique_graphics_objects()
{

}

/*
 * Copy all data from the audio data struct to the
 * graphics data struct to make it available for
 * rendering.
 */
void Output::copy_graphics_data()
{
    this->graphics.input_l = new vector<float>(*(this->audio.input_l));
    this->graphics.input_r = new vector<float>(*(this->audio.input_r));
}
