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
#include "../Graphics_Objects/Waveform.hpp"
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

    Graphics_Object *dummy = NULL;
    graphics_objects = new vector<Graphics_Object *>(11, dummy);
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
    int x_text,
        x_button,
        x_text_box, w_text_box, h_text_box,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10;
    SDL_Rect location;
    string object_name, contents, prompt, text_off;
    Text *text;
    Text_Box *text_box;
    Waveform *waveform;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 5;
    x_button = upper_left.x + MODULE_BORDER_WIDTH + 70;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    w_text_box = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_text_box = 15;
    w_waveform = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_waveform = 55;
    y3 = upper_left.y + MODULE_BORDER_WIDTH + 20;
    y4 = upper_left.y + MODULE_BORDER_WIDTH + 20;
    y5 = upper_left.y + MODULE_BORDER_WIDTH + 40;   
    y6 = upper_left.y + MODULE_BORDER_WIDTH + 97;
    y7 = upper_left.y + MODULE_BORDER_WIDTH + 112;
    y8 = upper_left.y + MODULE_BORDER_WIDTH + 133;
    y9 = upper_left.y + MODULE_BORDER_WIDTH + 189;
    y10 = upper_left.y + MODULE_BORDER_WIDTH + 204;

    // If the 4th graphics object is null, that means the graphics objects have not
    // been calculated before, and we must make them from scratch
    if((*graphics_objects)[3] == NULL)
    {
        // graphics_objects[3] is the display text "AUDIO ON":
        location = {x_text, y3, 8, 15};
        object_name = "on_off (text)";
        contents = "AUDIO ON:";
        text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
        (*graphics_objects)[3] = text;

        // graphics_objects[4] is the toggle button to turn audio on or off
        location = {x_button, y4, 8, 15};
        object_name = "on/off button (toggle_button)";
        contents = "1";
        text_off = "0";
        Toggle_Button *toggle_button = new Toggle_Button(&object_name, &location, &WHITE,
                                                  &BLACK, &contents, &text_off,
                                                  &AUDIO_ON, this);
        (*graphics_objects)[4] = toggle_button;

        // graphics_objects[5] is the waveform visualizer for the left speaker
        location = {x_text_box, y5, w_waveform, h_waveform};
        object_name = "waveform visualizer l (waveform)";
        waveform = new Waveform(&object_name, &location, &WHITE, graphics.input_l);
        (*graphics_objects)[5] = waveform;

        // graphics_objects[6] is the display text "LEFT INPUT:"
        location = {x_text, y6, 0, 0};
        object_name = "output input left (text)";
        contents = "INPUT LEFT:";
        text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
        (*graphics_objects)[6] = text;

        // graphics_objects[7] is the text box for entering and displaying input right
        location = {x_text_box, y7, w_text_box, h_text_box};
        object_name = "output input left (text_box)";
        contents = "";
        prompt = "input";
        text_box = new Text_Box(&object_name, &location, &text_color, NULL,
                                &contents, &prompt, FONT_REGULAR, this);
        (*graphics_objects)[7] = text_box;

        // graphics_objects[8] is the waveform visualizer for the right speaker
        location = {x_text_box, y8, w_waveform, h_waveform};
        object_name = "waveform visualizer r (waveform)";
        waveform = new Waveform(&object_name, &location, &WHITE, graphics.input_r);
        (*graphics_objects)[8] = waveform;

        // graphics_objects[9] is the display text "PHASE OFFSET:"
        location = {x_text, y9, 0, 0};
        object_name = "output input right (text)";
        contents = "INPUT RIGHT:";
        text = new Text(&object_name, &location, &text_color, NULL, &contents, FONT_REGULAR);
        (*graphics_objects)[9] = text;

        // graphics_objects[10] is the text box for entering and displaying input left
        location = {x_text_box, y10, w_text_box, h_text_box};
        object_name = "output input right (text_box)";
        contents = "";
        prompt = "input";
        text_box = new Text_Box(&object_name, &location, &text_color, NULL,
                                &contents, &prompt, FONT_REGULAR, this);
        (*graphics_objects)[10] = text_box;
    }
    // Otherwise, simply update the locations of all of the graphics objects
    else
    {
        location = {x_text, y3, 8, 15};
        (*graphics_objects)[3]->location = location;

        location = {x_button, y4, 8, 15};
        (*graphics_objects)[4]->location = location;

        location = {x_text_box, y5, w_waveform, h_waveform};
        (*graphics_objects)[5]->location = location;

        location = {x_text, y6, 0, 0};
        (*graphics_objects)[6]->location = location;

        location = {x_text_box, y7, w_text_box, h_text_box};
        (*graphics_objects)[7]->location = location;

        location = {x_text_box, y8, w_waveform, h_waveform};
        (*graphics_objects)[8]->location = location;

        location = {x_text, y9, 0, 0};
        (*graphics_objects)[9]->location = location;

        location = {x_text_box, y10, w_text_box, h_text_box};
        (*graphics_objects)[10]->location = location;
    }
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
