/*
 * Matthew Diamond 2015
 * Member functions for the VCA class.
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
#include "VCA.hpp"
#include "../Graphics_Objects/Text.hpp"
#include "../Graphics_Objects/Toggle_Button.hpp"
#include "../Graphics_Objects/Waveform.hpp"

using namespace std;

/*******************************
 * VCA MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor
 */
Vca::Vca(string _name, int _number)
{
    name = _name;
    type = VCA;
    number = _number;

    dependencies = new vector<Module *>(3, NULL);

    signal_float = 0;
    signal_str = "0";
    signal_input = NULL;

    cv_float = 1;
    cv_str = "1";
    cv_input = NULL;

    cv_amount_float = 1;
    cv_amount_str = "1";
    cv_amount_input = NULL;

    output = new vector<float>(BUFFER_SIZE, 0);
}

/*
 * Destructor.
 */
Vca::~Vca()
{

}

/*
 * Process all dependencies, then multiply the scale
 */
void Vca::process()
{
    // Process any dependencies
    process_dependencies();

    if(signal_input != NULL && cv_input != NULL)
    {
        for(unsigned short i = 0; i < BUFFER_SIZE; i ++)
        {
            if((*dependencies)[VCA_CV_SCALE_DEPENDENCY] != NULL)
                cv_amount_float = (*cv_amount_input)[i];

            (*output)[i] = ((*signal_input)[i] * (1 - cv_amount_float)) +
                           ((*signal_input)[i] * (*cv_input)[i] * cv_amount_float);
        }
    }

    processed = true;
}

void Vca::update_unique_graphics_objects()
{
    if((*dependencies)[VCA_CV_SCALE_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[VCA_CV_SCALE_TEXT_BOX])->text->text = to_string(cv_amount_float);
        ((Text_Box *) (*graphics_objects)[VCA_CV_SCALE_TEXT_BOX])->text->updated = true;
    }
}

void Vca::update_unique_control_values()
{

}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Vca::calculate_unique_graphics_objects()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        w_waveform, h_waveform,
        x_signal_cv, w_signals,
        y3, y4, y5, y6, y7;
    SDL_Rect location;
    Text *text;
    Text_Box *text_box;
    Waveform *waveform;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 5;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    w_text_box = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_text_box = 15;
    w_waveform = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_waveform = 135;
    y3 = upper_left.y + MODULE_BORDER_WIDTH + 23;
    y4 = y3 + 136;
    y5 = y4 + 15; 
    y6 = y5 + 15;
    y7 = y6 + 15;
    x_signal_cv = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_signals = (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3);


    // If the 4th graphics object is null, that means the graphics objects have not
    // been calculated before, and we must make them from scratch
    if(graphics_objects->size() == 3)
    {
        // graphics_objects[3] is the waveform visualizer
        location = {x_text_box, y3, w_waveform, h_waveform};
        waveform = new Waveform("waveform visualizer (waveform)", &location, &WHITE, output);
        graphics_objects->push_back(waveform);

        // graphics_objects[4] is the display text "SIGNAL & CV INPUT:"
        location = {x_text, y4, 0, 0};
        text = new Text("vca signal & cv input (text)", &location, &text_color, "SIGNAL & CV INPUT:", FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[5] is the text box for entering and displaying a signal
        location = {x_text_box, y5, w_signals, h_text_box};
        text_box = new Text_Box("vca signal (text_box)", &location, &text_color,
                                "", "input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[6] is the text box for entering and displaying control values
        location = {x_signal_cv, y5, w_signals, h_text_box};
        text_box = new Text_Box("vca cv (text_box)", &location, &text_color,
                                "", "input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[7] is the display text "CV SCALE FACTOR:"
        location = {x_text, y6, 0, 0};
        text = new Text("vca cv scale (text)", &location, &text_color, "CV AMOUNT:", FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[8] is the text box for entering and displaying control value scale factors
        location = {x_text_box, y7, w_text_box, h_text_box};
        text_box = new Text_Box("vca cv scale (text_box)", &location, &text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects->push_back(text_box);
    }

    // Otherwise, simply update the locations of all of the graphics objects
    else
    {
        location = {x_text_box, y3, w_waveform, h_waveform};
        (*graphics_objects)[VCA_OUTPUT_WAVEFORM]->update_location(&location);

        location = {x_text, y4, 0, 0};
        (*graphics_objects)[VCA_INPUT_TEXT]->update_location(&location);

        location = {x_text_box, y5, w_signals, h_text_box};
        (*graphics_objects)[VCA_SIGNAL_TEXT_BOX]->update_location(&location);

        location = {x_signal_cv, y5, w_signals, h_text_box};
        (*graphics_objects)[VCA_CV_TEXT_BOX]->update_location(&location);

        location = {x_text, y6, 0, 0};
        (*graphics_objects)[VCA_CV_SCALE_TEXT]->update_location(&location);

        location = {x_text_box, y7, w_text_box, h_text_box};
        (*graphics_objects)[VCA_CV_SCALE_TEXT_BOX]->update_location(&location);
    }
}

void Vca::set_signal(Module *src)
{
    set(src, &signal_input, VCA_SIGNAL_DEPENDENCY);
    cout << name << " signal is now coming from " << src->name << endl;
}

void Vca::set_cv(Module *src)
{
    set(src, &cv_input, VCA_CV_DEPENDENCY);
    cout << name << " control values are now coming from " << src->name << endl;
}

void Vca::set_cv_amount(float val)
{
    set(val, &cv_amount_float, VCA_CV_SCALE_DEPENDENCY);
    cout << name << " control value scale factor changed to " << cv_amount_float << endl;
}

void Vca::set_cv_amount(Module *src)
{
    set(src, &cv_amount_input, VCA_CV_SCALE_DEPENDENCY);
    cout << name << " control value scale factor is now coming from " << src->name << endl;
}
