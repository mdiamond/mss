/*
 * Matthew Diamond 2015
 * Member functions for the Output class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "image_processing.hpp"
#include "main.hpp"

// Included classes
#include "Module.hpp"
#include "Output.hpp"

using namespace std;

/***************************
 * OUTPUT MEMBER FUNCTIONS *
 ***************************/

/*
 * Constructor.
 */
Output::Output()
{
    name = "output";
    type = OUTPUT;

    color.r = rand() % 256;
    text_color.r = 256 - color.r;
    color.g = rand() % 256;
    text_color.g = 256 - color.g;
    color.b = rand() % 256;
    text_color.b = 256 - color.b;
    color.a = 255;
    text_color.a = 255;

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
 * Copy all data from the audio data struct to the
 * graphics data struct to make it available for
 * rendering.
 */
void Output::copy_graphics_data()
{
    this->graphics.input_l = new vector<float>(*(this->audio.input_l));
    this->graphics.input_r = new vector<float>(*(this->audio.input_r));
}

/*
 * This function uses the renderer to create a representation
 * of this module in the window.
 */
void Output::render()
{
    render_name(&text_color);
}

void Output::render_text()
{

}