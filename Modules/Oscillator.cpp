/*
 * Matthew Diamond 2015
 * Member functions for the Oscillator class.
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
#include "Oscillator.hpp"
#include "../Graphics_Objects/Text.hpp"
#include "../Graphics_Objects/Waveform.hpp"

using namespace std;

/*******************************
 * OSCILLATOR MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor
 */
Oscillator::Oscillator(string *_name, int _number)
{
    name = *_name;
    type = OSCILLATOR;
    number = _number;
    current_phase = 0;

    dependencies = new vector<Module *>(5, NULL);

    frequency = 0;
    frequency_str = "0";
    input_frequency = new vector<float>(BUFFER_SIZE, 0);

    phase_offset = 0;
    phase_offset_str = "0";
    input_phase_offset = new vector<float>(BUFFER_SIZE, 0);

    pulse_width = 0;
    pulse_width_str = "0";
    input_pulse_width = new vector<float>(BUFFER_SIZE, 0);

    range_low = -1;
    range_low_str = "-1";
    input_range_low = new vector<float>(BUFFER_SIZE, 0);

    range_high = 1;
    range_high_str = "1";
    input_range_high = new vector<float>(BUFFER_SIZE, 0);

    output = new vector<float>(BUFFER_SIZE, 0);

    waveform_type = SIN;
}

/*
 * Destructor.
 */
Oscillator::~Oscillator()
{

}

/*
 * Process all dependencies, then
 * fill the output buffer with a waveform given
 * the data contained within this class and the
 * audio device information.
 */
void Oscillator::process()
{
    // Process any dependencies
    process_dependencies();

    // Calculate an amplitude for each sample
    for(unsigned short i = 0; i < BUFFER_SIZE; i ++)
    {
        // Update any control values
        if((*dependencies)[OSCILLATOR_FREQUENCY_DEPENDENCY] != NULL)
            frequency = (*(input_frequency))[i];
        if((*dependencies)[OSCILLATOR_PHASE_OFFSET_DEPENDENCY] != NULL)
            phase_offset = (*(input_phase_offset))[i];
        if((*dependencies)[OSCILLATOR_PULSE_WIDTH_DEPENDENCY] != NULL)
            pulse_width = (*(input_pulse_width))[i];
        if((*dependencies)[OSCILLATOR_RANGE_LOW_DEPENDENCY] != NULL)
            range_low = (*(input_range_low))[i];
        if((*dependencies)[OSCILLATOR_RANGE_HIGH_DEPENDENCY] != NULL)
            range_high = (*(input_range_high))[i];

        // Calculate and store the current samples amplitude
        // based on phase
        (*output)[i] = sin(current_phase);
        current_phase += (2 * M_PI * (frequency / SAMPLE_RATE));
        if(current_phase > (2 * M_PI))
            current_phase -= (2 * M_PI);
    }
    if(range_low != -1 || range_high != 1)
    {
        scale_signal(output, -1, 1, range_low, range_high);
    }

    processed = true;
}

void Oscillator::update_unique_graphics_objects()
{
    // Update text boxes
    if((*dependencies)[OSCILLATOR_FREQUENCY_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[5])->text->text = to_string(frequency);
        ((Text_Box *) (*graphics_objects)[5])->text->updated = true;
    }
    if((*dependencies)[OSCILLATOR_PHASE_OFFSET_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[7])->text->text = to_string(phase_offset);
        ((Text_Box *) (*graphics_objects)[7])->text->updated = true;
    }
    if((*dependencies)[OSCILLATOR_PULSE_WIDTH_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[9])->text->text = to_string(pulse_width);
        ((Text_Box *) (*graphics_objects)[9])->text->updated = true;
    }
    if((*dependencies)[OSCILLATOR_RANGE_LOW_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[11])->text->text = to_string(range_low);
        ((Text_Box *) (*graphics_objects)[11])->text->updated = true;
    }
    if((*dependencies)[OSCILLATOR_RANGE_HIGH_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[12])->text->text = to_string(range_high);
        ((Text_Box *) (*graphics_objects)[12])->text->updated = true;
    }
}

void Oscillator::update_unique_control_values()
{
    // // Update any control values
    // if(live_frequency)
    //     frequency = (*(input_frequency))[current_sample];
    // if(live_phase_offset)
    //     phase_offset = (*(input_phase_offset))[current_sample];
    // if(live_pulse_width)
    //     pulse_width = (*(input_pulse_width))[current_sample];
    // if(live_range_low)
    //     range_low = (*(input_range_low))[current_sample];
    // if(live_range_high)
    //     range_high = (*(input_range_high))[current_sample];
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Oscillator::calculate_unique_graphics_objects()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10, y11,
        x_range_high, w_range;
    SDL_Rect location;
    string object_name, contents, prompt;
    Text *text;
    Text_Box *text_box;
    Waveform *waveform;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 5;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    w_text_box = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_text_box = 15;
    w_waveform = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_waveform = 55;
    y3 = upper_left.y + MODULE_BORDER_WIDTH + 23;
    y4 = upper_left.y + MODULE_BORDER_WIDTH + 80;
    y5 = upper_left.y + MODULE_BORDER_WIDTH + 97;   
    y6 = upper_left.y + MODULE_BORDER_WIDTH + 117;
    y7 = upper_left.y + MODULE_BORDER_WIDTH + 134;
    y8 = upper_left.y + MODULE_BORDER_WIDTH + 152;
    y9 = upper_left.y + MODULE_BORDER_WIDTH + 169;
    y10 = upper_left.y + MODULE_BORDER_WIDTH + 187;
    y11 = upper_left.y + MODULE_BORDER_WIDTH + 204;
    x_range_high = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_range = (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3);

    // If the 4th graphics object is null, that means the graphics objects have not
    // been calculated before, and we must make them from scratch
    if(graphics_objects->size() == 3)
    {
        // graphics_objects[3] is the waveform visualizer
        location = {x_text_box, y3, w_waveform, h_waveform};
        object_name = "waveform visualizer (waveform)";
        waveform = new Waveform(&object_name, &location, &WHITE, output);
        graphics_objects->push_back(waveform);

        // graphics_objects[4] is the display text "FREQUENCY:"
        location = {x_text, y4, 0, 0};
        object_name = "oscillator frequency (text)";
        contents = "FREQUENCY:";
        text = new Text(&object_name, &location, &text_color, &contents, FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[5] is the text box for entering and displaying frequency
        location = {x_text_box, y5, w_text_box, h_text_box};
        object_name = "oscillator frequency (text_box)";
        contents = "";
        prompt = "# or input";
        text_box = new Text_Box(&object_name, &location, &text_color, &frequency_str,
                                &contents, &prompt, FONT_REGULAR, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[6] is the display text "PHASE OFFSET:"
        location = {x_text, y6, 0, 0};
        object_name = "oscillator phase offset (text)";
        contents = "PHASE OFFSET:";
        text = new Text(&object_name, &location, &text_color, &contents, FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[7] is the text box for entering and displaying phase offset
        location = {x_text_box, y7, w_text_box, h_text_box};
        object_name = "oscillator phase offset (text_box)";
        contents = "";
        prompt = "# or input";
        text_box = new Text_Box(&object_name, &location, &text_color, &phase_offset_str,
                                &contents, &prompt, FONT_REGULAR, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[8] is the display text "PULSE WIDTH:"
        location = {x_text, y8, 0, 0};
        object_name = "oscillator pulse width (text)";
        contents = "PULSE WIDTH:";
        text = new Text(&object_name, &location, &text_color, &contents, FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[9] is the text box for entering and displaying pulse width
        location = {x_text_box, y9, w_text_box, h_text_box};
        object_name = "oscillator pulse width (text_box)";
        contents = "";
        prompt = "# or input";
        text_box = new Text_Box(&object_name, &location, &text_color, &pulse_width_str,
                                &contents, &prompt, FONT_REGULAR, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[10] is the display text "RANGE (LOW - HIGH):"
        location = {x_text, y10, 0, 0};
        object_name = "oscillator range low/high (text)";
        contents = "RANGE (LOW-HIGH):";
        text = new Text(&object_name, &location, &text_color, &contents, FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[11] is the text box for entering and displaying range low
        location = {x_text_box, y11, w_range, h_text_box};
        object_name = "oscillator range low (text_box)";
        contents = "";
        prompt = "# or input";
        text_box = new Text_Box(&object_name, &location, &text_color, &range_low_str,
                                &contents, &prompt, FONT_REGULAR, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[12] is the text box for entering and displaying range high
        location = {x_range_high, y11, w_range, h_text_box};
        object_name = "oscillator range high (text_box)";
        contents = "";
        prompt = "# or input";
        text_box = new Text_Box(&object_name, &location, &text_color, &range_high_str,
                                &contents, &prompt, FONT_REGULAR, this);
        graphics_objects->push_back(text_box);
    }
    // Otherwise, simply update the locations of all of the graphics objects
    else
    {
        location = {x_text_box, y3, w_waveform, h_waveform};
        (*graphics_objects)[OSCILLATOR_OUTPUT_WAVEFORM]->location = location;

        location = {x_text, y4, 0, 0};
        (*graphics_objects)[OSCILLATOR_FREQUENCY_TEXT]->location = location;

        location = {x_text_box, y5, w_text_box, h_text_box};
        (*graphics_objects)[OSCILLATOR_FREQUENCY_TEXT_BOX]->location = location;

        location = {x_text, y6, 0, 0};
        (*graphics_objects)[OSCILLATOR_PHASE_OFFSET_TEXT]->location = location;

        location = {x_text_box, y7, w_text_box, h_text_box};
        (*graphics_objects)[OSCILLATOR_PHASE_OFFSET_TEXT_BOX]->location = location;

        location = {x_text, y8, 0, 0};
        (*graphics_objects)[OSCILLATOR_PULSE_WIDTH_TEXT]->location = location;

        location = {x_text_box, y9, w_text_box, h_text_box};
        (*graphics_objects)[OSCILLATOR_PULSE_WIDTH_TEXT_BOX]->location = location;

        location = {x_text, y10, 0, 0};
        (*graphics_objects)[OSCILLATOR_RANGE_TEXT]->location = location;

        location = {x_text_box, y11, w_range, h_text_box};
        (*graphics_objects)[OSCILLATOR_RANGE_LOW_TEXT_BOX]->location = location;

        location = {x_range_high, y11, w_range, h_text_box};
        (*graphics_objects)[OSCILLATOR_RANGE_HIGH_TEXT_BOX]->location = location;
    }
}

void Oscillator::set_frequency(float val)
{
    set(val, &frequency, OSCILLATOR_FREQUENCY_DEPENDENCY);
    cout << name << " frequency changed to " << frequency << endl;
}

void Oscillator::set_frequency(Module *src)
{
    set(src, &input_frequency, OSCILLATOR_FREQUENCY_DEPENDENCY);
    cout << name << " frequency is now coming from " << src->name << endl;
}

void Oscillator::set_phase_offset(float val)
{
    set(val, &phase_offset, OSCILLATOR_PHASE_OFFSET_DEPENDENCY);
    cout << name << " phase offset changed to " << phase_offset << endl;
}

void Oscillator::set_phase_offset(Module *src)
{
    set(src, &input_phase_offset, OSCILLATOR_PHASE_OFFSET_DEPENDENCY);
    cout << name << " phase offset is now coming from " << src->name << endl;
}

void Oscillator::set_pulse_width(float val)
{
    set(val, &pulse_width, OSCILLATOR_PULSE_WIDTH_DEPENDENCY);
    cout << name << " pulse width changed to " << pulse_width << endl;
}

void Oscillator::set_pulse_width(Module *src)
{
    set(src, &input_pulse_width, OSCILLATOR_PULSE_WIDTH_DEPENDENCY);
    cout << name << " pulse width is now coming from " << src->name << endl;
}

void Oscillator::set_range_low(float val)
{
    set(val, &range_low, OSCILLATOR_RANGE_LOW_DEPENDENCY);
    cout << name << " range low changed to " << range_low << endl;
}

void Oscillator::set_range_low(Module *src)
{
    set(src, &input_range_low, OSCILLATOR_RANGE_LOW_DEPENDENCY);
    cout << name << " range low is now coming from " << src->name << endl;
}

void Oscillator::set_range_high(float val)
{
    set(val, &range_high, OSCILLATOR_RANGE_HIGH_DEPENDENCY);
    cout << name << " range high changed to " << range_high << endl;
}

void Oscillator::set_range_high(Module *src)
{
    set(src, &input_range_high, OSCILLATOR_RANGE_HIGH_DEPENDENCY);
    cout << name << " range high is now coming from " << src->name << endl;
}
