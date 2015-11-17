/*
 * Matthew Diamond 2015
 * Member functions for the mixer class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "../function_forwarder.hpp"
#include "../image_processing.hpp"
#include "../main.hpp"
#include "../signal_processing.hpp"

// Included classes
#include "../Module.hpp"
#include "Mixer.hpp"
#include "../Graphics_Objects/Input_Text_Box.hpp"
#include "../Graphics_Objects/Input_Toggle_Button.hpp"
#include "../Graphics_Objects/Text.hpp"
#include "../Graphics_Objects/Toggle_Button.hpp"
#include "../Graphics_Objects/Waveform.hpp"

using namespace std;

/**************************
 * MIXER MEMBER FUNCTIONS *
 **************************/

/*
 * Constructor.
 */
Mixer::Mixer() :
    Module(MIXER)
{
    auto_attenuate = true;

    // All multiplier floats should start at 1
    for(unsigned int i = 0; i < input_floats.size(); i ++)
        if(i % 2 == 1)
            input_floats[i] = 1;
}

/*
 * Destructor.
 */
Mixer::~Mixer()
{

}

/*
 * Process all dependencies, then sum and attenuate all signal inputs
 */
void Mixer::process()
{
    // Process any dependencies
    process_dependencies();

    short num_channels = 0;

    // Reset the output buffer
    for(int i = 0; i < BUFFER_SIZE; i ++)
        output[i] = 0;

    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        num_channels = 0;

        if(dependencies[MIXER_SIGNAL_1_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_1_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_1_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_2_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_2_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_2_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_3_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_3_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_3_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_4_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_4_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_4_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_5_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_5_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_5_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_6_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_6_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_6_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_7_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_7_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_7_MULTIPLIER]))[i];
        if(dependencies[MIXER_SIGNAL_8_MULTIPLIER] != NULL)
            input_floats[MIXER_SIGNAL_8_MULTIPLIER] = (*(inputs[MIXER_SIGNAL_8_MULTIPLIER]))[i];

        if(dependencies[MIXER_SIGNAL_1] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_1]))[i] * input_floats[MIXER_SIGNAL_1_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_2] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_2]))[i] * input_floats[MIXER_SIGNAL_2_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_3] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_3]))[i] * input_floats[MIXER_SIGNAL_3_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_4] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_4]))[i] * input_floats[MIXER_SIGNAL_4_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_5] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_5]))[i] * input_floats[MIXER_SIGNAL_5_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_6] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_6]))[i] * input_floats[MIXER_SIGNAL_6_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_7] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_7]))[i] * input_floats[MIXER_SIGNAL_7_MULTIPLIER];
            num_channels ++;
        }
        if(dependencies[MIXER_SIGNAL_8] != NULL)
        {
            output[i] += (*(inputs[MIXER_SIGNAL_8]))[i] * input_floats[MIXER_SIGNAL_8_MULTIPLIER];
            num_channels ++;
        }


        if(auto_attenuate)
            if(num_channels != 0)
                output[i] /= num_channels;
    }

    processed = true;
}

void Mixer::update_unique_graphics_objects()
{
    // if(dependencies[MIXER_SIGNAL_1_MULTIPLIER] != NULL)
    //     ((Text_Box *) graphics_objects[MIXER_SIGNAL_1_MULTIPLIER_TEXT_BOX])->update_current_text(to_string(input_floats[MIXER_SIGNAL_1_MULTIPLIER]));
    // if(dependencies[MIXER_SIGNAL_2_MULTIPLIER] != NULL)
    //     ((Text_Box *) graphics_objects[MIXER_SIGNAL_2_MULTIPLIER_TEXT_BOX])->update_current_text(to_string(input_floats[MIXER_SIGNAL_2_MULTIPLIER]));
    // if(dependencies[MIXER_SIGNAL_3_MULTIPLIER] != NULL)
    //     ((Text_Box *) graphics_objects[MIXER_SIGNAL_3_MULTIPLIER_TEXT_BOX])->update_current_text(to_string(input_floats[MIXER_SIGNAL_3_MULTIPLIER]));
    // if(dependencies[MIXER_SIGNAL_4_MULTIPLIER] != NULL)
    //     ((Text_Box *) graphics_objects[MIXER_SIGNAL_4_MULTIPLIER_TEXT_BOX])->update_current_text(to_string(input_floats[MIXER_SIGNAL_4_MULTIPLIER]));
    // if(dependencies[MIXER_SIGNAL_5_MULTIPLIER] != NULL)
    //     ((Text_Box *) graphics_objects[MIXER_SIGNAL_5_MULTIPLIER_TEXT_BOX])->update_current_text(to_string(input_floats[MIXER_SIGNAL_5_MULTIPLIER]));
    // if(dependencies[MIXER_SIGNAL_6_MULTIPLIER] != NULL)
    //     ((Text_Box *) graphics_objects[MIXER_SIGNAL_6_MULTIPLIER_TEXT_BOX])->update_current_text(to_string(input_floats[MIXER_SIGNAL_6_MULTIPLIER]));
    // if(dependencies[MIXER_SIGNAL_7_MULTIPLIER] != NULL)
    //     ((Text_Box *) graphics_objects[MIXER_SIGNAL_7_MULTIPLIER_TEXT_BOX])->update_current_text(to_string(input_floats[MIXER_SIGNAL_7_MULTIPLIER]));
    // if(dependencies[MIXER_SIGNAL_8_MULTIPLIER] != NULL)
    //     ((Text_Box *) graphics_objects[MIXER_SIGNAL_8_MULTIPLIER_TEXT_BOX])->update_current_text(to_string(input_floats[MIXER_SIGNAL_8_MULTIPLIER]));
}

void Mixer::update_unique_control_values()
{

}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Mixer::calculate_unique_graphics_objects()
{
    int x_text, x_text_box, x_text_box_2, w_text_box, h_text_box,
        x_input_toggle_button, x_input_toggle_button_2, w_input_toggle_button,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10, y11, y12;
    SDL_Rect location;
    Text *text;
    Input_Text_Box *input_text_box;
    Input_Toggle_Button *input_toggle_button;
    Waveform *waveform;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box_2 = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_text_box = (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3) - 10;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box;
    x_input_toggle_button_2 = x_text_box_2 + w_text_box;
    w_input_toggle_button = 10;
    w_waveform = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_waveform = 46;
    y3 = upper_left.y + MODULE_BORDER_WIDTH + 23;
    y4 = y3 + 47;
    y5 = y4 + 15;
    y6 = y5 + 17;
    y7 = y6 + 17;
    y8 = y7 + 17;
    y9 = y8 + 17;
    y10 = y9 + 17;
    y11 = y10 + 17;
    y12 = y11 + 17;


    // If the 4th graphics object is null, that means the graphics objects have not
    // been calculated before, and we must make them from scratch
    if(graphics_objects.size() == 3)
    {
        // graphics_objects[3] is the waveform visualizer
        location = {x_text_box, y3, w_waveform, h_waveform};
        waveform = new Waveform("waveform visualizer (waveform)", location, WHITE, -1, 1, &output);
        graphics_objects.push_back(waveform);

        // graphics_objects[4] is the display text "SIGNAL & MULTIPLIER:"
        location = {x_text, y4, 0, 0};
        text = new Text("mixer signal 1 (text)", location, text_color, "SIGNAL & MULTIPLIER:", FONT_REGULAR);
        graphics_objects.push_back(text);

        // graphics_objects[5] is the text box for entering and displaying signal 1
        location = {x_text_box, y5, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 1 (input text box)", location, text_color,
                                "", "input", FONT_SMALL, this, MIXER_SIGNAL_1);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[6] is the toggle button for selecting or disabling signal 1 input
        location = {x_input_toggle_button, y5, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 1 input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_1],
                                          this, MIXER_SIGNAL_1, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[7] is the text box for entering and displaying the signal 1 channel multiplier
        location = {x_text_box_2, y5, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 1 multiplier (input text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this, MIXER_SIGNAL_1_MULTIPLIER);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[8] is the toggle button for selecting or disabling signal 1 multiplier input
        location = {x_input_toggle_button_2, y5, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 1 multiplier input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_1_MULTIPLIER],
                                          this, MIXER_SIGNAL_1_MULTIPLIER, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[9] is the text box for entering and displaying signal 2
        location = {x_text_box, y6, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 2 (input text box)", location, text_color,
                                "", "input", FONT_SMALL, this, MIXER_SIGNAL_2);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[10] is the toggle button for selecting or disabling signal 2 input
        location = {x_input_toggle_button, y6, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 2 input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_2],
                                          this, MIXER_SIGNAL_2, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[11] is the text box for entering and displaying the signal 2 channel multiplier
        location = {x_text_box_2, y6, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 2 multiplier (input text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this, MIXER_SIGNAL_2_MULTIPLIER);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[12] is the toggle button for selecting or disabling signal 2 multiplier input
        location = {x_input_toggle_button_2, y6, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 2 multiplier input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_2_MULTIPLIER],
                                          this, MIXER_SIGNAL_2_MULTIPLIER, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[13] is the text box for entering and displaying signal 3
        location = {x_text_box, y7, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 3 (input text box)", location, text_color,
                                "", "input", FONT_SMALL, this, MIXER_SIGNAL_3);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[14] is the toggle button for selecting or disabling signal 3 input
        location = {x_input_toggle_button, y7, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 3 input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_3],
                                          this, MIXER_SIGNAL_3, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[15] is the text box for entering and displaying the signal 3 channel multiplier
        location = {x_text_box_2, y7, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 3 multiplier (input text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this, MIXER_SIGNAL_3_MULTIPLIER);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[16] is the toggle button for selecting or disabling signal 3 multiplier input
        location = {x_input_toggle_button_2, y7, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 3 multiplier input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_3_MULTIPLIER],
                                          this, MIXER_SIGNAL_3_MULTIPLIER, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[17] is the text box for entering and displaying signal 4
        location = {x_text_box, y8, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 4 (input text box)", location, text_color,
                                "", "input", FONT_SMALL, this, MIXER_SIGNAL_4);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[18] is the toggle button for selecting or disabling signal 4 input
        location = {x_input_toggle_button, y8, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 4 input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_4],
                                          this, MIXER_SIGNAL_4, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[19] is the text box for entering and displaying the signal 4 channel multiplier
        location = {x_text_box_2, y8, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 4 multiplier (input text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this, MIXER_SIGNAL_4_MULTIPLIER);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[20] is the toggle button for selecting or disabling signal 4 multiplier input
        location = {x_input_toggle_button_2, y8, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 4 multiplier input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_4_MULTIPLIER],
                                          this, MIXER_SIGNAL_4_MULTIPLIER, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[21] is the text box for entering and displaying signal 5
        location = {x_text_box, y9, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 5 (input text box)", location, text_color,
                                "", "input", FONT_SMALL, this, MIXER_SIGNAL_5);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[22] is the toggle button for selecting or disabling signal 5 input
        location = {x_input_toggle_button, y9, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 5 input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_5],
                                          this, MIXER_SIGNAL_5, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[23] is the text box for entering and displaying the signal 5 channel multiplier
        location = {x_text_box_2, y9, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 5 multiplier (input text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this, MIXER_SIGNAL_5_MULTIPLIER);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[24] is the toggle button for selecting or disabling signal 5 multiplier input
        location = {x_input_toggle_button_2, y9, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 5 multiplier input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_5_MULTIPLIER],
                                          this, MIXER_SIGNAL_5_MULTIPLIER, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[25] is the text box for entering and displaying signal 6
        location = {x_text_box, y10, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 6 (input text box)", location, text_color,
                                "", "input", FONT_SMALL, this, MIXER_SIGNAL_6);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[26] is the toggle button for selecting or disabling signal 6 input
        location = {x_input_toggle_button, y10, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 6 input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_6],
                                          this, MIXER_SIGNAL_6, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[27] is the text box for entering and displaying the signal 6 channel multiplier
        location = {x_text_box_2, y10, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 6 multiplier (input text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this, MIXER_SIGNAL_6_MULTIPLIER);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[28] is the toggle button for selecting or disabling signal 6 multiplier input
        location = {x_input_toggle_button_2, y10, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 6 multiplier input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_6_MULTIPLIER],
                                          this, MIXER_SIGNAL_6_MULTIPLIER, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[29] is the text box for entering and displaying signal 7
        location = {x_text_box, y11, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 7 (input text box)", location, text_color,
                                "", "input", FONT_SMALL, this, MIXER_SIGNAL_7);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[30] is the toggle button for selecting or disabling signal 7 input
        location = {x_input_toggle_button, y11, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 7 input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_7],
                                          this, MIXER_SIGNAL_7, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[31] is the text box for entering and displaying the signal 7 channel multiplier
        location = {x_text_box_2, y11, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 7 multiplier (input text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this, MIXER_SIGNAL_7_MULTIPLIER);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[32] is the toggle button for selecting or disabling signal 7 multiplier input
        location = {x_input_toggle_button_2, y11, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 7 multiplier input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_7_MULTIPLIER],
                                          this, MIXER_SIGNAL_7_MULTIPLIER, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[33] is the text box for entering and displaying signal 8
        location = {x_text_box, y12, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 8 (input text box)", location, text_color,
                                "", "input", FONT_SMALL, this, MIXER_SIGNAL_8);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[34] is the toggle button for selecting or disabling signal 8 input
        location = {x_input_toggle_button, y12, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 8 input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_8],
                                          this, MIXER_SIGNAL_8, input_text_box);
        graphics_objects.push_back(input_toggle_button);

        // graphics_objects[35] is the text box for entering and displaying the signal 8 channel multiplier
        location = {x_text_box_2, y12, w_text_box, h_text_box};
        input_text_box = new Input_Text_Box("mixer signal 8 multiplier (input text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this, MIXER_SIGNAL_8_MULTIPLIER);
        graphics_objects.push_back(input_text_box);

        // graphics_objects[36] is the toggle button for selecting or disabling signal 8 multiplier input
        location = {x_input_toggle_button_2, y12, w_input_toggle_button, h_text_box};
        input_toggle_button = new Input_Toggle_Button("mixer signal 8 multiplier input (input toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[MIXER_SIGNAL_8_MULTIPLIER],
                                          this, MIXER_SIGNAL_8_MULTIPLIER, input_text_box);
        graphics_objects.push_back(input_toggle_button);
    }

    // Otherwise, simply update the locations of all of the graphics objects
    else
    {

        location = {x_text_box, y3, w_waveform, h_waveform};
        graphics_objects[MIXER_OUTPUT_WAVEFORM]->update_location(location);

        location = {x_text, y4, 0, 0};
        graphics_objects[MIXER_SIGNAL_1_TEXT]->update_location(location);

        location = {x_text_box, y5, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_1_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y5, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_1_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box_2, y5, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_1_MULTIPLIER_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button_2, y5, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_1_MULTIPLIER_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box, y6, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_2_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y6, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_2_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box_2, y6, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_2_MULTIPLIER_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button_2, y6, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_2_MULTIPLIER_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box, y7, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_3_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y7, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_3_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box_2, y7, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_3_MULTIPLIER_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button_2, y7, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_3_MULTIPLIER_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box, y8, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_4_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y8, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_4_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box_2, y8, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_4_MULTIPLIER_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button_2, y8, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_4_MULTIPLIER_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box, y9, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_5_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y9, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_5_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box_2, y9, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_5_MULTIPLIER_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button_2, y9, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_5_MULTIPLIER_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box, y10, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_6_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y10, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_6_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box_2, y10, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_6_MULTIPLIER_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button_2, y10, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_6_MULTIPLIER_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box, y11, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_7_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y11, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_7_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box_2, y11, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_7_MULTIPLIER_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button_2, y11, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_7_MULTIPLIER_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box, y12, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_8_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y12, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_8_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box_2, y12, w_text_box, h_text_box};
        graphics_objects[MIXER_SIGNAL_8_MULTIPLIER_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button_2, y12, w_input_toggle_button, h_text_box};
        graphics_objects[MIXER_SIGNAL_8_MULTIPLIER_INPUT_TOGGLE_BUTTON]->update_location(location);

    }
}
