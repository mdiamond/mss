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
#include "../Graphics_Objects/Toggle_Button.hpp"
#include "../Graphics_Objects/Waveform.hpp"

using namespace std;

/*******************************
 * OSCILLATOR MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor
 */
Oscillator::Oscillator(string _name, int _number)
{
    name = _name;
    type = OSCILLATOR;
    number = _number;

    current_phase = 0;
    previous_phase_offset = 0;

    dependencies = new vector<Module *>(5, NULL);

    frequency = 0;
    frequency_str = "0";
    input_frequency = new vector<float>(BUFFER_SIZE, 0);

    phase_offset = 0;
    phase_offset_str = "0";
    input_phase_offset = new vector<float>(BUFFER_SIZE, 0);

    pulse_width = .5;
    pulse_width_str = ".5";
    input_pulse_width = new vector<float>(BUFFER_SIZE, 0);

    range_low = -1;
    range_low_str = "-1";
    input_range_low = new vector<float>(BUFFER_SIZE, 0);

    range_high = 1;
    range_high_str = "1";
    input_range_high = new vector<float>(BUFFER_SIZE, 0);

    output = new vector<float>(BUFFER_SIZE, 0);

    waveform_type = SIN;

    wavetable = SIN_WAVE;

    sin_on = true;
    tri_on = false;
    saw_on = false;
    sqr_on = false;
}

/*
 * Destructor.
 */
Oscillator::~Oscillator()
{

}

float Oscillator::produce_sin_sample(float phase)
{
    return sin(2 * M_PI * phase);
}

float Oscillator::produce_tri_sample(float phase)
{
    if(phase < .25)
        return phase / .25;
    else if(phase < .5)
        return 1 - ((phase - .25) / .25);
    else if(phase < .75)
        return 0 - ((phase - .5) / .25);
    else
        return ((phase - .75) / .25) - 1;
}

float Oscillator::produce_saw_sample(float phase)
{
    if(phase < .5)
        return 1 - (phase / .5);
    else
        return 0 - ((phase - .5) / .5);
}

float Oscillator::produce_sqr_sample(float phase)
{
    if(phase < pulse_width)
        return 1;
    else
        return -1;
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

    float phase_offset_diff;
    // Calculate an amplitude for each sample
    for(unsigned short i = 0; i < BUFFER_SIZE; i ++)
    {

        // Update any control values
        if((*dependencies)[OSCILLATOR_FREQUENCY_DEPENDENCY] != NULL)
            frequency = (*(input_frequency))[i];
        if((*dependencies)[OSCILLATOR_PHASE_OFFSET_DEPENDENCY] != NULL)
        {
            previous_phase_offset = phase_offset;
            phase_offset = (*(input_phase_offset))[i];
            phase_offset_diff = phase_offset - previous_phase_offset;
        }
        if((*dependencies)[OSCILLATOR_PULSE_WIDTH_DEPENDENCY] != NULL)
            pulse_width = (*(input_pulse_width))[i];
        if((*dependencies)[OSCILLATOR_RANGE_LOW_DEPENDENCY] != NULL)
            range_low = (*(input_range_low))[i];
        if((*dependencies)[OSCILLATOR_RANGE_HIGH_DEPENDENCY] != NULL)
            range_high = (*(input_range_high))[i];

        // Calculate and store the current samples amplitude
        // based on phase
        if(frequency < 1 && frequency > -1)
        {
            if(waveform_type == SIN)
                (*output)[i] = produce_sin_sample(current_phase);
            else if(waveform_type == TRI)
                (*output)[i] = produce_tri_sample(current_phase);
            else if(waveform_type == SAW)
                (*output)[i] = produce_saw_sample(current_phase);
            else if(waveform_type == SQR)
                (*output)[i] = produce_sqr_sample(current_phase);
        }
        else
            if(waveform_type != SQR || pulse_width == .5)
                (*output)[i] = (*wavetable)[(int) (current_phase * wavetable->size())];
            else
                (*output)[i] = produce_sqr_sample(current_phase);

        current_phase += ((float) frequency / SAMPLE_RATE) + phase_offset_diff;
        if(current_phase > 1)
            current_phase -= 1;
    }

    // If the oscillator has an abnormal range, scale it into that range
    if(range_low != -1 || range_high != 1)
        scale_signal(output, -1, 1, range_low, range_high);

    processed = true;
}

void Oscillator::update_unique_graphics_objects()
{
    // Update text boxes
    if((*dependencies)[OSCILLATOR_FREQUENCY_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[OSCILLATOR_FREQUENCY_TEXT_BOX])->text->text = to_string(frequency);
        ((Text_Box *) (*graphics_objects)[OSCILLATOR_FREQUENCY_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[OSCILLATOR_PHASE_OFFSET_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[OSCILLATOR_PHASE_OFFSET_TEXT_BOX])->text->text = to_string(phase_offset);
        ((Text_Box *) (*graphics_objects)[OSCILLATOR_PHASE_OFFSET_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[OSCILLATOR_PULSE_WIDTH_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[OSCILLATOR_PULSE_WIDTH_TEXT_BOX])->text->text = to_string(pulse_width);
        ((Text_Box *) (*graphics_objects)[OSCILLATOR_PULSE_WIDTH_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[OSCILLATOR_RANGE_LOW_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[OSCILLATOR_RANGE_LOW_TEXT_BOX])->text->text = to_string(range_low);
        ((Text_Box *) (*graphics_objects)[OSCILLATOR_RANGE_LOW_TEXT_BOX])->text->updated = true;
    }
    if((*dependencies)[OSCILLATOR_RANGE_HIGH_DEPENDENCY] != NULL)
    {
        ((Text_Box *) (*graphics_objects)[OSCILLATOR_RANGE_HIGH_TEXT_BOX])->text->text = to_string(range_high);
        ((Text_Box *) (*graphics_objects)[OSCILLATOR_RANGE_HIGH_TEXT_BOX])->text->updated = true;
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
        y3, y4, y5, y6, y7, y8, y9, y10, y11, y12,
        x_range_high, w_range,
        w_wave_selector;
    SDL_Rect location;
    Text *text;
    Text_Box *text_box;
    Waveform *waveform;
    Toggle_Button *toggle_button;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 5;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    w_text_box = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_text_box = 15;
    w_waveform = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4);
    h_waveform = 45;
    y3 = upper_left.y + MODULE_BORDER_WIDTH + 23;
    y4 = y3 + 46;
    y5 = y4 + 15;   
    y6 = y5 + 16;
    y7 = y6 + 15;
    y8 = y7 + 16;
    y9 = y8 + 15;
    y10 = y9 + 16;
    y11 = y10 + 15;
    y12 = y11 + 26;
    x_range_high = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_range = (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3);
    w_wave_selector = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) / 4) - 2;

    // If the 4th graphics object is null, that means the graphics objects have not
    // been calculated before, and we must make them from scratch
    if(graphics_objects->size() == 3)
    {
        // graphics_objects[3] is the waveform visualizer
        location = {x_text_box, y3, w_waveform, h_waveform};
        waveform = new Waveform("waveform visualizer (waveform)", &location, &WHITE, output);
        graphics_objects->push_back(waveform);

        // graphics_objects[4] is the display text "FREQUENCY:"
        location = {x_text, y4, 0, 0};
        text = new Text("oscillator frequency (text)", &location, &text_color, "FREQUENCY:", FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[5] is the text box for entering and displaying frequency
        location = {x_text_box, y5, w_text_box, h_text_box};
        text_box = new Text_Box("oscillator frequency (text_box)", &location, &text_color,
                                "", "# or input", FONT_REGULAR, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[6] is the display text "PHASE OFFSET:"
        location = {x_text, y6, 0, 0};
        text = new Text("oscillator phase offset (text)", &location, &text_color, "PHASE OFFSET:", FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[7] is the text box for entering and displaying phase offset
        location = {x_text_box, y7, w_text_box, h_text_box};
        text_box = new Text_Box("oscillator phase offset (text_box)", &location, &text_color,
                                "", "# or input", FONT_REGULAR, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[8] is the display text "PULSE WIDTH:"
        location = {x_text, y8, 0, 0};
        text = new Text("oscillator pulse width (text)", &location, &text_color, "PULSE WIDTH:", FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[9] is the text box for entering and displaying pulse width
        location = {x_text_box, y9, w_text_box, h_text_box};
        text_box = new Text_Box("oscillator pulse width (text_box)", &location, &text_color,
                                "", "# or input", FONT_REGULAR, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[10] is the display text "RANGE (LOW - HIGH):"
        location = {x_text, y10, 0, 0};
        text = new Text("oscillator range low/high (text)", &location, &text_color, "RANGE (LOW-HIGH):", FONT_REGULAR);
        graphics_objects->push_back(text);

        // graphics_objects[11] is the text box for entering and displaying range low
        location = {x_text_box, y11, w_range, h_text_box};
        text_box = new Text_Box("oscillator range low (text_box)", &location, &text_color,
                                "", "# or input", FONT_REGULAR, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[12] is the text box for entering and displaying range high
        location = {x_range_high, y11, w_range, h_text_box};
        text_box = new Text_Box("oscillator range high (text_box)", &location, &text_color,
                                "", "# or input", FONT_REGULAR, this);
        graphics_objects->push_back(text_box);

        // graphics_objects[13] is the button for selecting sine wave output
        location = {x_text_box, y12, w_wave_selector, h_text_box};
        toggle_button = new Toggle_Button("oscillator sin toggle (toggle button)", &location,
                                          &WHITE, &BLACK, "SIN", "SIN", &sin_on, this);
        graphics_objects->push_back(toggle_button);

        // graphics_objects[13] is the button for selecting sine wave output
        location = {x_text_box + w_wave_selector + 2, y12, w_wave_selector, h_text_box};
        toggle_button = new Toggle_Button("oscillator tri toggle (toggle button)", &location,
                                          &WHITE, &BLACK, "TRI", "TRI", &tri_on, this);
        graphics_objects->push_back(toggle_button);

        // graphics_objects[13] is the button for selecting sine wave output
        location = {x_text_box + ((w_wave_selector + 2) * 2), y12, w_wave_selector, h_text_box};
        toggle_button = new Toggle_Button("oscillator saw toggle (toggle button)", &location,
                                          &WHITE, &BLACK, "SAW", "SAW", &saw_on, this);
        graphics_objects->push_back(toggle_button);

        // graphics_objects[13] is the button for selecting sine wave output
        location = {x_text_box + ((w_wave_selector + 2) * 3), y12, w_wave_selector, h_text_box};
        toggle_button = new Toggle_Button("oscillator sqr toggle (toggle button)", &location,
                                          &WHITE, &BLACK, "SQR", "SQR", &sqr_on, this);
        graphics_objects->push_back(toggle_button);
    }

    // Otherwise, simply update the locations of all of the graphics objects
    else
    {
        location = {x_text_box, y3, w_waveform, h_waveform};
        (*graphics_objects)[OSCILLATOR_OUTPUT_WAVEFORM]->update_location(&location);

        location = {x_text, y4, 0, 0};
        (*graphics_objects)[OSCILLATOR_FREQUENCY_TEXT]->update_location(&location);

        location = {x_text_box, y5, w_text_box, h_text_box};
        (*graphics_objects)[OSCILLATOR_FREQUENCY_TEXT_BOX]->update_location(&location);

        location = {x_text, y6, 0, 0};
        (*graphics_objects)[OSCILLATOR_PHASE_OFFSET_TEXT]->update_location(&location);

        location = {x_text_box, y7, w_text_box, h_text_box};
        (*graphics_objects)[OSCILLATOR_PHASE_OFFSET_TEXT_BOX]->update_location(&location);

        location = {x_text, y8, 0, 0};
        (*graphics_objects)[OSCILLATOR_PULSE_WIDTH_TEXT]->update_location(&location);

        location = {x_text_box, y9, w_text_box, h_text_box};
        (*graphics_objects)[OSCILLATOR_PULSE_WIDTH_TEXT_BOX]->update_location(&location);

        location = {x_text, y10, 0, 0};
        (*graphics_objects)[OSCILLATOR_RANGE_TEXT]->update_location(&location);

        location = {x_text_box, y11, w_range, h_text_box};
        (*graphics_objects)[OSCILLATOR_RANGE_LOW_TEXT_BOX]->update_location(&location);

        location = {x_range_high, y11, w_range, h_text_box};
        (*graphics_objects)[OSCILLATOR_RANGE_HIGH_TEXT_BOX]->update_location(&location);

        // graphics_objects[13] is the button for selecting sine wave output
        location = {x_text_box, y12, w_wave_selector, h_text_box};
        (*graphics_objects)[OSCILLATOR_SIN_WAVE_TOGGLE_BUTTON]->update_location(&location);

        // graphics_objects[13] is the button for selecting sine wave output
        location = {x_text_box + w_wave_selector + 2, y12, w_wave_selector, h_text_box};
        (*graphics_objects)[OSCILLATOR_TRI_WAVE_TOGGLE_BUTTON]->update_location(&location);

        // graphics_objects[13] is the button for selecting sine wave output
        location = {x_text_box + ((w_wave_selector + 2) * 2), y12, w_wave_selector, h_text_box};
        (*graphics_objects)[OSCILLATOR_SAW_WAVE_TOGGLE_BUTTON]->update_location(&location);

        // graphics_objects[13] is the button for selecting sine wave output
        location = {x_text_box + ((w_wave_selector + 2) * 3), y12, w_wave_selector, h_text_box};
        (*graphics_objects)[OSCILLATOR_SQR_WAVE_TOGGLE_BUTTON]->update_location(&location);
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

void Oscillator::switch_waveform(int _waveform_type)
{
    sin_on = false;
    tri_on = false;
    saw_on = false;
    sqr_on = false;

    if(_waveform_type == SIN)
    {
        wavetable = SIN_WAVE;
        waveform_type = SIN;
        sin_on = true;
        cout << name << " is now outputting a sine wave" << endl;
    }
    else if(_waveform_type == TRI)
    {
        wavetable = TRI_WAVE;
        waveform_type = TRI;
        tri_on = true;
        cout << name << " is now outputting a triangle wave" << endl;
    }
    else if(_waveform_type == SAW)
    {
        wavetable = SAW_WAVE;
        waveform_type = SAW;
        saw_on = true;
        cout << name << " is now outputting a sawtooth wave" << endl;
    }
    else
    {
        wavetable = SQR_WAVE;
        waveform_type = SQR;
        sqr_on = true;
        cout << name << " is now outputting a square wave" << endl;
    }
}
