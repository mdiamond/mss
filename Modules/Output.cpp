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
#include "../signal_processing.hpp"

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

    audio.input_l = new vector<float>(BUFFER_SIZE, 0);
    audio.input_r = new vector<float>(BUFFER_SIZE, 0);

    graphics.input_l = new vector<float>(BUFFER_SIZE, 0);
    graphics.input_r = new vector<float>(BUFFER_SIZE, 0);
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

void Output::calculate_text_objects()
{
    SDL_Rect location = {upper_left.x + MODULE_BORDER_WIDTH + 2,
                          upper_left.y + MODULE_BORDER_WIDTH + 23,
                          8, 15};
    string object_name = "on_off (text)";
    string text = "AUDIO ON:";
    Text *on_off_text = new Text(&object_name, &location, &text_color, NULL, &text, FONT_REGULAR);
    graphics_objects.push_back(on_off_text);
}

Graphics_Object *Output::calculate_on_off_button()
{
    SDL_Rect location = {upper_left.x + MODULE_BORDER_WIDTH + 70,
                          upper_left.y + MODULE_BORDER_WIDTH + 23,
                          8, 15};
    string object_name = "on/off button (toggle_button)";
    string text_on = "1";
    string text_off = "0";
    Toggle_Button *on_off = new Toggle_Button(&object_name, &location, &WHITE,
                                              &BLACK, &text_on, &text_off,
                                              &AUDIO_ON, this);
    return on_off;
}

Graphics_Object *Output::calculate_input_1eft()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 2;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 85;
    SDL_Rect location = {x, y, ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4), 15};
    string object_name = "output input left (text_box)";
    string prompt = "module name";
    string text = "";
    Text_Box *input_left = new Text_Box(&object_name, &location, &text_color,
                                       NULL,
                                       &text,
                                       &prompt,
                                       FONT_REGULAR,
                                       this);
    return input_left;
}

Graphics_Object *Output::calculate_input_right()
{
    int x = upper_left.x + MODULE_BORDER_WIDTH + 2;
    int y = upper_left.y + MODULE_BORDER_WIDTH + 120;
    SDL_Rect location = {x, y, ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4), 15};
    string object_name = "output input right (text_box)";
    string prompt = "module name";
    string text = "";
    Text_Box *input_right = new Text_Box(&object_name, &location, &text_color,
                                       NULL,
                                       &text,
                                       &prompt,
                                       FONT_REGULAR,
                                       this);
    return input_right;
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Output::calculate_unique_graphics_objects()
{
    calculate_text_objects();
    graphics_objects.push_back(calculate_on_off_button());
    graphics_objects.push_back(calculate_input_1eft());
    graphics_objects.push_back(calculate_input_right());
}

/*
 * Copy all data from the audio data struct to the
 * graphics data struct to make it available for
 * rendering.
 */
void Output::copy_graphics_data()
{
  copy_signal(audio.input_l, graphics.input_l);
  copy_signal(audio.input_r, graphics.input_r);
}

void Output::toggle_audio_on()
{
    SDL_LockAudio();
    AUDIO_ON = !AUDIO_ON;
    if(AUDIO_ON)
        SDL_PauseAudio(0);
    else
        SDL_PauseAudio(1);
    SDL_UnlockAudio();
}
