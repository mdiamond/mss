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
Mixer::Mixer(string _name, int _number)
{
    name = _name;
    type = MIXER;
    number = _number;

    dependencies = new vector<Module *>(16, NULL);
    output = new vector<float>(BUFFER_SIZE, 0);

    auto_attenuate = true;

    signal_1_float = 0;
    signal_1_multiplier_float = 1;
    signal_1_str = "0";
    signal_1_multiplier_str = "1";
    signal_1_input = NULL;
    signal_1_multiplier_input = NULL;

    signal_2_float = 0;
    signal_2_multiplier_float = 1;
    signal_2_str = "0";
    signal_2_multiplier_str = "1";
    signal_2_input = NULL;
    signal_2_multiplier_input = NULL;

    signal_3_float = 0;
    signal_3_multiplier_float = 1;
    signal_3_str = "0";
    signal_3_multiplier_str = "1";
    signal_3_input = NULL;
    signal_3_multiplier_input = NULL;

    signal_4_float = 0;
    signal_4_multiplier_float = 1;
    signal_4_str = "0";
    signal_4_multiplier_str = "1";
    signal_4_input = NULL;
    signal_4_multiplier_input = NULL;

    signal_5_float = 0;
    signal_5_multiplier_float = 1;
    signal_5_str = "0";
    signal_5_multiplier_str = "1";
    signal_5_input = NULL;
    signal_5_multiplier_input = NULL;

    signal_6_float = 0;
    signal_6_multiplier_float = 1;
    signal_6_str = "0";
    signal_6_multiplier_str = "1";
    signal_6_input = NULL;
    signal_6_multiplier_input = NULL;

    signal_7_float = 0;
    signal_7_multiplier_float = 1;
    signal_7_str = "0";
    signal_7_multiplier_str = "1";
    signal_7_input = NULL;
    signal_7_multiplier_input = NULL;

    signal_8_float = 0;
    signal_8_multiplier_float = 1;
    signal_8_str = "0";
    signal_8_multiplier_str = "1";
    signal_8_input = NULL;
    signal_8_multiplier_input = NULL;
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
        (*output)[i] = 0;

    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        num_channels = 0;

        if((*dependencies)[MIXER_SIGNAL_1_MULTIPLIER_DEPENDENCY] != NULL)
            signal_1_multiplier_float = (*signal_1_multiplier_input)[i];
        if((*dependencies)[MIXER_SIGNAL_2_MULTIPLIER_DEPENDENCY] != NULL)
            signal_2_multiplier_float = (*signal_2_multiplier_input)[i];
        if((*dependencies)[MIXER_SIGNAL_3_MULTIPLIER_DEPENDENCY] != NULL)
            signal_3_multiplier_float = (*signal_3_multiplier_input)[i];
        if((*dependencies)[MIXER_SIGNAL_4_MULTIPLIER_DEPENDENCY] != NULL)
            signal_4_multiplier_float = (*signal_4_multiplier_input)[i];
        if((*dependencies)[MIXER_SIGNAL_5_MULTIPLIER_DEPENDENCY] != NULL)
            signal_5_multiplier_float = (*signal_5_multiplier_input)[i];
        if((*dependencies)[MIXER_SIGNAL_6_MULTIPLIER_DEPENDENCY] != NULL)
            signal_6_multiplier_float = (*signal_6_multiplier_input)[i];
        if((*dependencies)[MIXER_SIGNAL_7_MULTIPLIER_DEPENDENCY] != NULL)
            signal_7_multiplier_float = (*signal_7_multiplier_input)[i];
        if((*dependencies)[MIXER_SIGNAL_8_MULTIPLIER_DEPENDENCY] != NULL)
            signal_8_multiplier_float = (*signal_8_multiplier_input)[i];

        if((*dependencies)[MIXER_SIGNAL_1_DEPENDENCY] != NULL)
        {
            (*output)[i] += (*signal_1_input)[i] * signal_1_multiplier_float;
            num_channels ++;
        }
        if((*dependencies)[MIXER_SIGNAL_2_DEPENDENCY] != NULL)
        {
            (*output)[i] += (*signal_2_input)[i] * signal_2_multiplier_float;
            num_channels ++;
        }
        if((*dependencies)[MIXER_SIGNAL_3_DEPENDENCY] != NULL)
        {
            (*output)[i] += (*signal_3_input)[i] * signal_3_multiplier_float;
            num_channels ++;
        }
        if((*dependencies)[MIXER_SIGNAL_4_DEPENDENCY] != NULL)
        {
            (*output)[i] += (*signal_4_input)[i] * signal_4_multiplier_float;
            num_channels ++;
        }
        if((*dependencies)[MIXER_SIGNAL_5_DEPENDENCY] != NULL)
        {
            (*output)[i] += (*signal_5_input)[i] * signal_5_multiplier_float;
            num_channels ++;
        }
        if((*dependencies)[MIXER_SIGNAL_6_DEPENDENCY] != NULL)
        {
            (*output)[i] += (*signal_6_input)[i] * signal_6_multiplier_float;
            num_channels ++;
        }
        if((*dependencies)[MIXER_SIGNAL_7_DEPENDENCY] != NULL)
        {
            (*output)[i] += (*signal_7_input)[i] * signal_7_multiplier_float;
            num_channels ++;
        }
        if((*dependencies)[MIXER_SIGNAL_8_DEPENDENCY] != NULL)
        {
            (*output)[i] += (*signal_8_input)[i] * signal_8_multiplier_float;
            num_channels ++;
        }

        if(auto_attenuate)
            (*output)[i] /= num_channels;
    }

    processed = true;
}

void Mixer::update_unique_graphics_objects()
{
    if((*dependencies)[MIXER_SIGNAL_1_MULTIPLIER_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_1_MULTIPLIER_TEXT_BOX])->text->text = to_string(signal_1_multiplier_float);
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_1_MULTIPLIER_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[MIXER_SIGNAL_2_MULTIPLIER_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_2_MULTIPLIER_TEXT_BOX])->text->text = to_string(signal_2_multiplier_float);
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_2_MULTIPLIER_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[MIXER_SIGNAL_3_MULTIPLIER_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_3_MULTIPLIER_TEXT_BOX])->text->text = to_string(signal_3_multiplier_float);
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_3_MULTIPLIER_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[MIXER_SIGNAL_4_MULTIPLIER_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_4_MULTIPLIER_TEXT_BOX])->text->text = to_string(signal_4_multiplier_float);
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_4_MULTIPLIER_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[MIXER_SIGNAL_5_MULTIPLIER_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_5_MULTIPLIER_TEXT_BOX])->text->text = to_string(signal_5_multiplier_float);
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_5_MULTIPLIER_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[MIXER_SIGNAL_6_MULTIPLIER_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_6_MULTIPLIER_TEXT_BOX])->text->text = to_string(signal_6_multiplier_float);
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_6_MULTIPLIER_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[MIXER_SIGNAL_7_MULTIPLIER_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_7_MULTIPLIER_TEXT_BOX])->text->text = to_string(signal_7_multiplier_float);
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_7_MULTIPLIER_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[MIXER_SIGNAL_8_MULTIPLIER_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_8_MULTIPLIER_TEXT_BOX])->text->text = to_string(signal_8_multiplier_float);
        ((Text_Box *) (*graphics_objects)[MIXER_SIGNAL_8_MULTIPLIER_TEXT_BOX])->text->updated = true;
    }
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
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10, y11, y12;
    SDL_Rect location;
    Text *text;
    Text_Box *text_box;
    Waveform *waveform;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box_2 = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_text_box = (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3);
    h_text_box = 15;
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
    if(graphics_objects->size() == 3)
    {
        // graphics_objects[3] is the waveform visualizer
        location = {x_text_box, y3, w_waveform, h_waveform};
        waveform = new Waveform("waveform visualizer (waveform)", &location, &WHITE, output);
        graphics_objects->push_back(waveform);

        // graphics_objects[4] is the display text "SIGNAL & MULTIPLIER:"
        location = {x_text, y4, 0, 0};
        text = new Text("mixer signal 1 (text)", &location, &text_color, "SIGNAL & MULTIPLIER:", FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[5] is the text box for entering and displaying signal 1
        location = {x_text_box, y5, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 1 (text_box)", &location, &text_color,
                                "", "input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[6] is the text box for entering and displaying the signal 1 channel multiplier
        location = {x_text_box_2, y5, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 1 multiplier (text_box)", &location, &text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[7] is the text box for entering and displaying signal 2
        location = {x_text_box, y6, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 2 (text_box)", &location, &text_color,
                                "", "input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[8] is the text box for entering and displaying the signal 2 channel multiplier
        location = {x_text_box_2, y6, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 2 multiplier (text_box)", &location, &text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[9] is the text box for entering and displaying signal 3
        location = {x_text_box, y7, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 3 (text_box)", &location, &text_color,
                                "", "input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[10] is the text box for entering and displaying the signal 3 channel multiplier
        location = {x_text_box_2, y7, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 3 multiplier (text_box)", &location, &text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[11] is the text box for entering and displaying signal 4
        location = {x_text_box, y8, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 4 (text_box)", &location, &text_color,
                                "", "input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[12] is the text box for entering and displaying the signal 4 channel multiplier
        location = {x_text_box_2, y8, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 4 multiplier (text_box)", &location, &text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[13] is the text box for entering and displaying signal 5
        location = {x_text_box, y9, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 5 (text_box)", &location, &text_color,
                                "", "input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[14] is the text box for entering and displaying the signal 5 channel multiplier
        location = {x_text_box_2, y9, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 5 multiplier (text_box)", &location, &text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[15] is the text box for entering and displaying signal 6
        location = {x_text_box, y10, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 6 (text_box)", &location, &text_color,
                                "", "input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[16] is the text box for entering and displaying the signal 6 channel multiplier
        location = {x_text_box_2, y10, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 6 multiplier (text_box)", &location, &text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[17] is the text box for entering and displaying signal 7
        location = {x_text_box, y11, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 7 (text_box)", &location, &text_color,
                                "", "input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[18] is the text box for entering and displaying the signal 7 channel multiplier
        location = {x_text_box_2, y11, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 7 multiplier (text_box)", &location, &text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[19] is the text box for entering and displaying signal 8
        location = {x_text_box, y12, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 8 (text_box)", &location, &text_color,
                                "", "input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[20] is the text box for entering and displaying the signal 8 channel multiplier
        location = {x_text_box_2, y12, w_text_box, h_text_box};
        text_box = new Text_Box("mixer signal 8 multiplier (text_box)", &location, &text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);
    }

    // Otherwise, simply update the locations of all of the graphics objects
    else
    {

        location = {x_text_box, y3, w_waveform, h_waveform};
        (*graphics_objects)[MIXER_OUTPUT_WAVEFORM]->update_location(&location);

        location = {x_text, y4, 0, 0};
        (*graphics_objects)[MIXER_SIGNAL_1_TEXT]->update_location(&location);

        location = {x_text_box, y5, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_1_TEXT_BOX]->update_location(&location);

        location = {x_text_box_2, y5, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_1_MULTIPLIER_TEXT_BOX]->update_location(&location);

        location = {x_text_box, y6, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_2_TEXT_BOX]->update_location(&location);

        location = {x_text_box_2, y6, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_2_MULTIPLIER_TEXT_BOX]->update_location(&location);

        location = {x_text_box, y7, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_3_TEXT_BOX]->update_location(&location);

        location = {x_text_box_2, y7, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_3_MULTIPLIER_TEXT_BOX]->update_location(&location);

        location = {x_text_box, y8, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_4_TEXT_BOX]->update_location(&location);

        location = {x_text_box_2, y8, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_4_MULTIPLIER_TEXT_BOX]->update_location(&location);

        location = {x_text_box, y9, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_5_TEXT_BOX]->update_location(&location);

        location = {x_text_box_2, y9, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_5_MULTIPLIER_TEXT_BOX]->update_location(&location);

        location = {x_text_box, y10, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_6_TEXT_BOX]->update_location(&location);

        location = {x_text_box_2, y10, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_6_MULTIPLIER_TEXT_BOX]->update_location(&location);

        location = {x_text_box, y11, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_7_TEXT_BOX]->update_location(&location);

        location = {x_text_box_2, y11, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_7_MULTIPLIER_TEXT_BOX]->update_location(&location);

        location = {x_text_box, y12, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_8_TEXT_BOX]->update_location(&location);

        location = {x_text_box_2, y12, w_text_box, h_text_box};
        (*graphics_objects)[MIXER_SIGNAL_8_MULTIPLIER_TEXT_BOX]->update_location(&location);

    }
}

void Mixer::set_signal(Module *src, short signal_num)
{
    switch(signal_num)
    {
        case 1:
            set(src, &signal_1_input, MIXER_SIGNAL_1_DEPENDENCY);
            break;
        case 2:
            set(src, &signal_2_input, MIXER_SIGNAL_2_DEPENDENCY);
            break;
        case 3:
            set(src, &signal_3_input, MIXER_SIGNAL_3_DEPENDENCY);
            break;
        case 4:
            set(src, &signal_4_input, MIXER_SIGNAL_4_DEPENDENCY);
            break;
        case 5:
            set(src, &signal_5_input, MIXER_SIGNAL_5_DEPENDENCY);
            break;
        case 6:
            set(src, &signal_6_input, MIXER_SIGNAL_6_DEPENDENCY);
            break;
        case 7:
            set(src, &signal_7_input, MIXER_SIGNAL_7_DEPENDENCY);
            break;
        case 8:
            set(src, &signal_8_input, MIXER_SIGNAL_8_DEPENDENCY);
            break;
    }

    cout << name << " signal " << signal_num << " is now coming from " << src->name << endl;
}

void Mixer::set_signal_multiplier(Module *src, short signal_num)
{
    switch(signal_num)
    {
        case 1:
            set(src, &signal_1_multiplier_input, MIXER_SIGNAL_1_MULTIPLIER_DEPENDENCY);
            break;
        case 2:
            set(src, &signal_3_multiplier_input, MIXER_SIGNAL_2_MULTIPLIER_DEPENDENCY);
            break;
        case 3:
            set(src, &signal_3_multiplier_input, MIXER_SIGNAL_3_MULTIPLIER_DEPENDENCY);
            break;
        case 4:
            set(src, &signal_4_multiplier_input, MIXER_SIGNAL_4_MULTIPLIER_DEPENDENCY);
            break;
        case 5:
            set(src, &signal_5_multiplier_input, MIXER_SIGNAL_5_MULTIPLIER_DEPENDENCY);
            break;
        case 6:
            set(src, &signal_6_multiplier_input, MIXER_SIGNAL_6_MULTIPLIER_DEPENDENCY);
            break;
        case 7:
            set(src, &signal_7_multiplier_input, MIXER_SIGNAL_7_MULTIPLIER_DEPENDENCY);
            break;
        case 8:
            set(src, &signal_8_multiplier_input, MIXER_SIGNAL_8_MULTIPLIER_DEPENDENCY);
            break;
    }

    cout << name << " signal " << signal_num << " multiplier is now coming from " << src->name << endl;
}

void Mixer::set_signal_multiplier(float val, short signal_num)
{
    switch(signal_num)
    {
        case 1:
            set(val, &signal_1_multiplier_float, MIXER_SIGNAL_1_MULTIPLIER_DEPENDENCY);
            break;
        case 2:
            set(val, &signal_3_multiplier_float, MIXER_SIGNAL_2_MULTIPLIER_DEPENDENCY);
            break;
        case 3:
            set(val, &signal_3_multiplier_float, MIXER_SIGNAL_3_MULTIPLIER_DEPENDENCY);
            break;
        case 4:
            set(val, &signal_4_multiplier_float, MIXER_SIGNAL_4_MULTIPLIER_DEPENDENCY);
            break;
        case 5:
            set(val, &signal_5_multiplier_float, MIXER_SIGNAL_5_MULTIPLIER_DEPENDENCY);
            break;
        case 6:
            set(val, &signal_6_multiplier_float, MIXER_SIGNAL_6_MULTIPLIER_DEPENDENCY);
            break;
        case 7:
            set(val, &signal_7_multiplier_float, MIXER_SIGNAL_7_MULTIPLIER_DEPENDENCY);
            break;
        case 8:
            set(val, &signal_8_multiplier_float, MIXER_SIGNAL_8_MULTIPLIER_DEPENDENCY);
            break;
    }

    cout << name << " signal " << signal_num << " multiplier is now " << val << endl;
}
