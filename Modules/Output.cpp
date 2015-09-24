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
#include "../Graphics_Objects/Toggle_Button.hpp"
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

    audio_on = false;
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

Graphics_Object *Output::calculate_on_off_button()
{
    SDL_Rect _on_off = {upper_left.x + MODULE_BORDER_WIDTH + 2,
                          upper_left.y + MODULE_BORDER_WIDTH + 18,
                          7, 15};
    string object_name = "on/off button";
    string text_on = "1";
    string text_off = "0";
    Toggle_Button *on_off = new Toggle_Button(&object_name, &_on_off, &WHITE, &BLACK, &text_on, &text_off, &audio_on);
    return on_off;
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Output::calculate_unique_graphics_objects()
{
    graphics_objects.push_back(calculate_on_off_button());
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
