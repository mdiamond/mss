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
#include "../function_forwarder.hpp"
#include "../image_processing.hpp"
#include "../main.hpp"
#include "../signal_processing.hpp"

// Included classes
#include "../Graphics_Objects/Input_Text_Box.hpp"
#include "../Graphics_Objects/Input_Toggle_Button.hpp"
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
Output::Output() :
    Module(OUTPUT)
{

}

/*
 * Destructor.
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
    process_dependencies();
}

void Output::update_unique_graphics_objects()
{

}

void Output::update_unique_control_values()
{

}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Output::calculate_unique_graphics_objects()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        x_input_toggle_button, w_input_toggle_button,
        x_button,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10;
    SDL_Rect location;
    Text *text;
    Input_Text_Box *input_text_box;
    Toggle_Button *toggle_button;
    Input_Toggle_Button *input_toggle_button;
    Waveform *waveform;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    w_text_box = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4) - 10;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box;
    w_input_toggle_button = 10;
    x_button = upper_left.x + MODULE_BORDER_WIDTH + 70;
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
    if(graphics_objects.size() == 3)
    {
        // graphics_objects[3] is the display text "AUDIO ON":
        location = {x_text, y3, 8, 15};
        text = new Text("on/off (text)", location, text_color, "AUDIO ON:", FONT_REGULAR);
        graphics_objects.push_back(text);

        // graphics_objects[4] is the toggle button to turn audio on or off
        location = {x_button, y4, 25, 15};
        toggle_button = new Toggle_Button("on/off button (toggle_button)", location, GREEN,
                                                  BLACK, BLACK, WHITE, FONT_BOLD, "ON", "OFF",
                                                  AUDIO_ON, this);
        graphics_objects.push_back(toggle_button);

        // graphics_objects[5] is the waveform visualizer for the left speaker
        location = {x_text_box, y5, w_waveform, h_waveform};
        waveform = new Waveform("waveform visualizer l (waveform)", location, WHITE, -1, 1, inputs[OUTPUT_INPUT_L]);
        graphics_objects.push_back(waveform);

        // graphics_objects[6] is the display text "LEFT INPUT:"
        location = {x_text, y6, 0, 0};
        text = new Text("output input left (text)", location, text_color, "LEFT SIGNAL:", FONT_REGULAR);
        graphics_objects.push_back(text);

        // graphics_objects[7] is the text box for entering and displaying input right
        location = {x_text_box, y7, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("output input left (input text box)", location, text_color,
                                "", "input", FONT_SMALL, this, OUTPUT_INPUT_L);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[8] is the toggle button for selecting or disabling the left input
        location = {x_input_toggle_button, y7, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("output left input (input toggle button)", location, WHITE,
                                                BLACK, RED, WHITE, FONT_SMALL, "I", "I",
                                                inputs_live[OUTPUT_INPUT_L], this, OUTPUT_INPUT_L);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[9] is the waveform visualizer for the right speaker
        location = {x_text_box, y8, w_waveform, h_waveform};
        waveform = new Waveform("waveform visualizer r (waveform)", location, WHITE, -1, 1, inputs[OUTPUT_INPUT_R]);
        graphics_objects.push_back(waveform);

        // graphics_objects[10] is the display text "PHASE OFFSET:"
        location = {x_text, y9, 0, 0};
        text = new Text("output input right (text)", location, text_color, "RIGHT SIGNAL:", FONT_REGULAR);
        graphics_objects.push_back(text);

        // graphics_objects[11] is the text box for entering and displaying input left
        location = {x_text_box, y10, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("output input right (input text box)", location, text_color,
                                "", "input", FONT_SMALL, this, OUTPUT_INPUT_R);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[12] is the toggle button for selecting or disabling frequency input
        location = {x_input_toggle_button, y10, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("output right input (input toggle button)", location, WHITE,
                                                BLACK, RED, WHITE, FONT_SMALL, "I", "I",
                                                inputs_live[OUTPUT_INPUT_R], this, OUTPUT_INPUT_R);
        graphics_objects.push_back(input_toggle_button);
    }

    // Otherwise, simply update the locations of all of the graphics objects
    else
    {
        location = {x_text, y3, 8, 15};
        graphics_objects[OUTPUT_AUDIO_TOGGLE_TEXT]->update_location(location);

        location = {x_button, y4, 25, 15};
        graphics_objects[OUTPUT_AUDIO_TOGGLE_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box, y5, w_waveform, h_waveform};
        graphics_objects[OUTPUT_INPUT_L_WAVEFORM]->update_location(location);

        location = {x_text, y6, 0, 0};
        graphics_objects[OUTPUT_INPUT_L_TEXT]->update_location(location);

        location = {x_text_box, y7, w_text_box, h_text_box};
        graphics_objects[OUTPUT_INPUT_L_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y7, w_input_toggle_button, h_text_box};
        graphics_objects[OUTPUT_INPUT_L_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box, y8, w_waveform, h_waveform};
        graphics_objects[OUTPUT_INPUT_R_WAVEFORM]->update_location(location);

        location = {x_text, y9, 0, 0};
        graphics_objects[OUTPUT_INPUT_R_TEXT]->update_location(location);

        location = {x_text_box, y10, w_text_box, h_text_box};
        graphics_objects[OUTPUT_INPUT_R_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y10, w_input_toggle_button, h_text_box};
        graphics_objects[OUTPUT_INPUT_R_TOGGLE_BUTTON]->update_location(location);
    }
}

void Output::toggle_audio_on()
{
    AUDIO_ON = !AUDIO_ON;

    if(AUDIO_ON)
        SDL_PauseAudio(0);
    else
        SDL_PauseAudio(1);

    cout << "Audio toggled" << endl;
}
