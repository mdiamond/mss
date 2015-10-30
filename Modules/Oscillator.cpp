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
 * Constructor.
 */
Oscillator::Oscillator() :
    Module(OSCILLATOR)
{
    int num_inputs = 5;

    current_phase = 0;
    previous_phase_offset = 0;

    dependencies = vector<Module *>(num_inputs, NULL);
    output = vector<float>(BUFFER_SIZE, 0);

    input_floats = vector<float>(num_inputs, 0);
    input_strs = vector<string>(num_inputs, "");
    inputs = vector<vector<float> *>(num_inputs, NULL);
    inputs_live = vector<bool>(num_inputs, false);

    // Frequency starts at 0, phase offset at 0,
    // pulse width at .5, range low at -1, range high
    // at 1
    input_floats[OSCILLATOR_PULSE_WIDTH] = .5;
    input_floats[OSCILLATOR_RANGE_LOW] = -1;
    input_floats[OSCILLATOR_RANGE_HIGH] = 1;

    waveform_type = SIN;

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
    if(phase < input_floats[OSCILLATOR_PULSE_WIDTH])
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
    for(unsigned short i = 0; i < output.size(); i ++)
    {
        for(unsigned int j = 0; j < dependencies.size(); j ++)
            if(inputs_live[j] && j != OSCILLATOR_PHASE_OFFSET)
                input_floats[j] = inputs[j]->at(i);

        if(inputs_live[OSCILLATOR_PHASE_OFFSET])
        {
            int previous_phase_offset = input_floats[OSCILLATOR_PHASE_OFFSET];
            input_floats[OSCILLATOR_PHASE_OFFSET] = inputs[OSCILLATOR_PHASE_OFFSET]->at(i);
            phase_offset_diff = input_floats[OSCILLATOR_PHASE_OFFSET] - previous_phase_offset;
        }

        // Calculate and store the current samples amplitude
        // based on phase
        if(input_floats[OSCILLATOR_FREQUENCY] < 1 && input_floats[OSCILLATOR_FREQUENCY] > -1)
        {
            if(waveform_type == SIN)
                output[i] = produce_sin_sample(current_phase);
            else if(waveform_type == TRI)
                output[i] = produce_tri_sample(current_phase);
            else if(waveform_type == SAW)
                output[i] = produce_saw_sample(current_phase);
            else if(waveform_type == SQR)
                output[i] = produce_sqr_sample(current_phase);
        }
        else
            if(waveform_type != SQR || input_floats[OSCILLATOR_PULSE_WIDTH] == .5)
                output[i] = WAVETABLES[waveform_type][(int) (current_phase * SAMPLE_RATE)];
            else
                output[i] = produce_sqr_sample(current_phase);

        // If the oscillator has an abnormal range, scale the sample to
        // that range
        if(input_floats[OSCILLATOR_RANGE_LOW] != -1
           || input_floats[OSCILLATOR_RANGE_HIGH] != 1)
            output[i] = scale_sample(output[i], -1, 1,
                                     input_floats[OSCILLATOR_RANGE_LOW],
                                     input_floats[OSCILLATOR_RANGE_HIGH]);

        current_phase += ((float) input_floats[OSCILLATOR_FREQUENCY] / SAMPLE_RATE) + phase_offset_diff;
        while(current_phase > 1)
            current_phase -= 1;
        while(current_phase < 0)
            current_phase += 1;
    }

    processed = true;
}

void Oscillator::update_unique_graphics_objects()
{
    // Update the range of the waveform viewer
    // ((Waveform *) graphics_objects[OSCILLATOR_OUTPUT_WAVEFORM])->range_low = input_floats[OSCILLATOR_RANGE_LOW];
    // ((Waveform *) graphics_objects[OSCILLATOR_OUTPUT_WAVEFORM])->range_high = input_floats[OSCILLATOR_RANGE_HIGH];
    // Update text boxes
    // if(inputs_live[OSCILLATOR_FREQUENCY])
    //     ((Text_Box *) graphics_objects[OSCILLATOR_FREQUENCY_TEXT_BOX])->update_current_text(to_string(input_floats[OSCILLATOR_FREQUENCY]));
    // if(inputs_live[OSCILLATOR_PHASE_OFFSET])
    //     ((Text_Box *) graphics_objects[OSCILLATOR_PHASE_OFFSET_TEXT_BOX])->update_current_text(to_string(input_floats[OSCILLATOR_PHASE_OFFSET]));
    // if(inputs_live[OSCILLATOR_PULSE_WIDTH])
    //     ((Text_Box *) graphics_objects[OSCILLATOR_PULSE_WIDTH_TEXT_BOX])->update_current_text(to_string(input_floats[OSCILLATOR_PULSE_WIDTH]));
    // if(inputs_live[OSCILLATOR_RANGE_LOW])
    //     ((Text_Box *) graphics_objects[OSCILLATOR_RANGE_LOW_TEXT_BOX])->update_current_text(to_string(input_floats[OSCILLATOR_RANGE_LOW]));
    // if(inputs_live[OSCILLATOR_RANGE_HIGH])
    //     ((Text_Box *) graphics_objects[OSCILLATOR_RANGE_HIGH_TEXT_BOX])->update_current_text(to_string(input_floats[OSCILLATOR_RANGE_HIGH]));
}

void Oscillator::update_unique_control_values()
{
    // // Update any control values
    // if(live_frequency)
    //     frequency = (*(frequency_input))[current_sample];
    // if(live_phase_offset)
    //     phase_offset = (*(phase_offset_input))[current_sample];
    // if(live_pulse_width)
    //     pulse_width = (*(pulse_width_input))[current_sample];
    // if(live_range_low)
    //     range_low = (*(range_low_input))[current_sample];
    // if(live_range_high)
    //     range_high = (*(range_high_input))[current_sample];
}

/*
 * Calculate the locations of any graphics objects that are
 * unique to this module type.
 */
void Oscillator::calculate_unique_graphics_objects()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        x_input_toggle_button, w_input_toggle_button,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10, y11, y12,
        x_range_high, x_range_low_input_toggle_button, w_range,
        w_wave_selector;
    SDL_Rect location;
    Text *text;
    Text_Box *text_box;
    Waveform *waveform;
    Toggle_Button *toggle_button;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    w_text_box = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4) - 10;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box;
    w_input_toggle_button = 10;
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
    y12 = y11 + 27;
    x_range_high = upper_left.x + (MODULE_WIDTH / 2) + 1;
    w_range = (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3) - 10;
    x_range_low_input_toggle_button = x_text_box + w_range;
    w_wave_selector = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) / 4) - 2;

    // If the 4th graphics object is null, that means the graphics objects have not
    // been calculated before, and we must make them from scratch
    if(graphics_objects.size() == 3)
    {
        // graphics_objects[3] is the waveform visualizer
        location = {x_text_box, y3, w_waveform, h_waveform};
        waveform = new Waveform("waveform visualizer (waveform)", location, WHITE, -1, 1, &output);
        graphics_objects.push_back(waveform);

        // graphics_objects[4] is the display text "FREQUENCY:"
        location = {x_text, y4, 0, 0};
        text = new Text("oscillator frequency (text)", location, text_color, "FREQUENCY:", FONT_REGULAR);
        graphics_objects.push_back(text);

        // graphics_objects[5] is the text box for entering and displaying frequency
        location = {x_text_box, y5, w_text_box, h_text_box};
        text_box = new Text_Box("oscillator frequency (text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects.push_back(text_box);

        // graphics_objects[6] is the toggle button for selecting or disabling frequency input
        location = {x_input_toggle_button, y5, w_input_toggle_button, h_text_box};
        toggle_button = new Toggle_Button("oscillator frequency input (toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[OSCILLATOR_FREQUENCY], this);
        graphics_objects.push_back(toggle_button);

        // graphics_objects[7] is the display text "PHASE OFFSET:"
        location = {x_text, y6, 0, 0};
        text = new Text("oscillator phase offset (text)", location, text_color, "PHASE OFFSET:", FONT_REGULAR);
        graphics_objects.push_back(text);

        // graphics_objects[8] is the text box for entering and displaying phase offset
        location = {x_text_box, y7, w_text_box, h_text_box};
        text_box = new Text_Box("oscillator phase offset (text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects.push_back(text_box);

        // graphics_objects[9] is the toggle button for selecting or disabling phase offset input
        location = {x_input_toggle_button, y7, w_input_toggle_button, h_text_box};
        toggle_button = new Toggle_Button("oscillator phase offset input (toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[OSCILLATOR_PHASE_OFFSET], this);
        graphics_objects.push_back(toggle_button);

        // graphics_objects[10] is the display text "PULSE WIDTH:"
        location = {x_text, y8, 0, 0};
        text = new Text("oscillator pulse width (text)", location, text_color, "PULSE WIDTH:", FONT_REGULAR);
        graphics_objects.push_back(text);

        // graphics_objects[11] is the text box for entering and displaying pulse width
        location = {x_text_box, y9, w_text_box, h_text_box};
        text_box = new Text_Box("oscillator pulse width (text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects.push_back(text_box);

        // graphics_objects[12] is the toggle button for selecting or disabling pulse width input
        location = {x_input_toggle_button, y9, w_input_toggle_button, h_text_box};
        toggle_button = new Toggle_Button("oscillator pulse width input (toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[OSCILLATOR_PULSE_WIDTH], this);
        graphics_objects.push_back(toggle_button);

        // graphics_objects[13] is the display text "RANGE (LOW - HIGH):"
        location = {x_text, y10, 0, 0};
        text = new Text("oscillator range low/high (text)", location, text_color, "RANGE LOW  HIGH:", FONT_REGULAR);
        graphics_objects.push_back(text);

        // graphics_objects[14] is the text box for entering and displaying range low
        location = {x_text_box, y11, w_range, h_text_box};
        text_box = new Text_Box("oscillator range low (text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects.push_back(text_box);

        // graphics_objects[15] is the toggle button for selecting or disabling range low input
        location = {x_range_low_input_toggle_button, y11, w_input_toggle_button, h_text_box};
        toggle_button = new Toggle_Button("oscillator range low input (toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[OSCILLATOR_RANGE_LOW], this);
        graphics_objects.push_back(toggle_button);

        // graphics_objects[16] is the text box for entering and displaying range high
        location = {x_range_high, y11, w_range, h_text_box};
        text_box = new Text_Box("oscillator range high (text box)", location, text_color,
                                "", "# or input", FONT_SMALL, this);
        graphics_objects.push_back(text_box);

        // graphics_objects[17] is the toggle button for selecting or disabling range high input
        location = {x_input_toggle_button, y11, w_input_toggle_button, h_text_box};
        toggle_button = new Toggle_Button("oscillator range high input (toggle button)", location, WHITE,
                                          BLACK, RED, WHITE, FONT_SMALL, "I", "I", inputs_live[OSCILLATOR_RANGE_HIGH], this);
        graphics_objects.push_back(toggle_button);

        // graphics_objects[18] is the button for selecting sine wave output
        location = {x_text_box, y12, w_wave_selector, h_text_box};
        toggle_button = new Toggle_Button("oscillator sin toggle (toggle button)", location,
                                          WHITE, BLACK, BLACK, WHITE, FONT_SMALL, "SIN",
                                          "SIN", sin_on, this);
        graphics_objects.push_back(toggle_button);

        // graphics_objects[19] is the button for selecting sine wave output
        location = {x_text_box + w_wave_selector + 2, y12, w_wave_selector, h_text_box};
        toggle_button = new Toggle_Button("oscillator tri toggle (toggle button)", location,
                                          WHITE, BLACK, BLACK, WHITE, FONT_SMALL, "TRI",
                                          "TRI", tri_on, this);
        graphics_objects.push_back(toggle_button);

        // graphics_objects[20] is the button for selecting sine wave output
        location = {x_text_box + ((w_wave_selector + 2) * 2), y12, w_wave_selector, h_text_box};
        toggle_button = new Toggle_Button("oscillator saw toggle (toggle button)", location,
                                          WHITE, BLACK, BLACK, WHITE, FONT_SMALL, "SAW",
                                          "SAW", saw_on, this);
        graphics_objects.push_back(toggle_button);

        // graphics_objects[21] is the button for selecting sine wave output
        location = {x_text_box + ((w_wave_selector + 2) * 3), y12, w_wave_selector, h_text_box};
        toggle_button = new Toggle_Button("oscillator sqr toggle (toggle button)", location,
                                          WHITE, BLACK, BLACK, WHITE, FONT_SMALL, "SQR",
                                          "SQR", sqr_on, this);
        graphics_objects.push_back(toggle_button);
    }

    // Otherwise, simply update the locations of all of the graphics objects
    else
    {
        location = {x_text_box, y3, w_waveform, h_waveform};
        graphics_objects[OSCILLATOR_OUTPUT_WAVEFORM]->update_location(location);

        location = {x_text, y4, 0, 0};
        graphics_objects[OSCILLATOR_FREQUENCY_TEXT]->update_location(location);

        location = {x_text_box, y5, w_text_box, h_text_box};
        graphics_objects[OSCILLATOR_FREQUENCY_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y5, w_input_toggle_button, h_text_box};
        graphics_objects[OSCILLATOR_FREQUENCY_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text, y6, 0, 0};
        graphics_objects[OSCILLATOR_PHASE_OFFSET_TEXT]->update_location(location);

        location = {x_text_box, y7, w_text_box, h_text_box};
        graphics_objects[OSCILLATOR_PHASE_OFFSET_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y7, w_input_toggle_button, h_text_box};
        graphics_objects[OSCILLATOR_PHASE_OFFSET_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text, y8, 0, 0};
        graphics_objects[OSCILLATOR_PULSE_WIDTH_TEXT]->update_location(location);

        location = {x_text_box, y9, w_text_box, h_text_box};
        graphics_objects[OSCILLATOR_PULSE_WIDTH_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y9, w_input_toggle_button, h_text_box};
        graphics_objects[OSCILLATOR_PULSE_WIDTH_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text, y10, 0, 0};
        graphics_objects[OSCILLATOR_RANGE_TEXT]->update_location(location);

        location = {x_text_box, y11, w_range, h_text_box};
        graphics_objects[OSCILLATOR_RANGE_LOW_TEXT_BOX]->update_location(location);

        location = {x_range_low_input_toggle_button, y11, w_input_toggle_button, h_text_box};
        graphics_objects[OSCILLATOR_RANGE_LOW_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_range_high, y11, w_range, h_text_box};
        graphics_objects[OSCILLATOR_RANGE_HIGH_TEXT_BOX]->update_location(location);

        location = {x_input_toggle_button, y11, w_input_toggle_button, h_text_box};
        graphics_objects[OSCILLATOR_RANGE_HIGH_INPUT_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box, y12, w_wave_selector, h_text_box};
        graphics_objects[OSCILLATOR_SIN_WAVE_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box + w_wave_selector + 2, y12, w_wave_selector, h_text_box};
        graphics_objects[OSCILLATOR_TRI_WAVE_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box + ((w_wave_selector + 2) * 2), y12, w_wave_selector, h_text_box};
        graphics_objects[OSCILLATOR_SAW_WAVE_TOGGLE_BUTTON]->update_location(location);

        location = {x_text_box + ((w_wave_selector + 2) * 3), y12, w_wave_selector, h_text_box};
        graphics_objects[OSCILLATOR_SQR_WAVE_TOGGLE_BUTTON]->update_location(location);
    }
}

void Oscillator::set_frequency(float val)
{
    set(val, OSCILLATOR_FREQUENCY);
    cout << name << " frequency changed to " << val << endl;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_FREQUENCY_INPUT_TOGGLE_BUTTON])->b = false;
}

void Oscillator::set_frequency(Module *src)
{
    set(src, OSCILLATOR_FREQUENCY);
    cout << name << " frequency is now coming from " << src->name << endl;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_FREQUENCY_INPUT_TOGGLE_BUTTON])->b = true;
}

void Oscillator::set_phase_offset(float val)
{
    set(val, OSCILLATOR_PHASE_OFFSET);
    cout << name << " phase offset changed to " << val << endl;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_PHASE_OFFSET_INPUT_TOGGLE_BUTTON])->b = false;
}

void Oscillator::set_phase_offset(Module *src)
{
    set(src, OSCILLATOR_PHASE_OFFSET);
    cout << name << " phase offset is now coming from " << src->name << endl;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_PHASE_OFFSET_INPUT_TOGGLE_BUTTON])->b = true;
}

void Oscillator::set_pulse_width(float val)
{
    set(val, OSCILLATOR_PULSE_WIDTH);
    cout << name << " pulse width changed to " << val << endl;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_PULSE_WIDTH_INPUT_TOGGLE_BUTTON])->b = false;
}

void Oscillator::set_pulse_width(Module *src)
{
    set(src, OSCILLATOR_PULSE_WIDTH);
    cout << name << " pulse width is now coming from " << src->name << endl;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_PULSE_WIDTH_INPUT_TOGGLE_BUTTON])->b = true;
}

void Oscillator::set_range_low(float val)
{
    set(val, OSCILLATOR_RANGE_LOW);
    cout << name << " range low changed to " << val << endl;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_RANGE_LOW_INPUT_TOGGLE_BUTTON])->b = false;
}

void Oscillator::set_range_low(Module *src)
{
    set(src, OSCILLATOR_RANGE_LOW);
    cout << name << " range low is now coming from " << src->name << endl;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_RANGE_LOW_INPUT_TOGGLE_BUTTON])->b = true;
}

void Oscillator::set_range_high(float val)
{
    set(val, OSCILLATOR_RANGE_HIGH);
    cout << name << " range high changed to " << val << endl;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_PULSE_WIDTH_INPUT_TOGGLE_BUTTON])->b = false;
}

void Oscillator::set_range_high(Module *src)
{
    set(src, OSCILLATOR_RANGE_HIGH);
    cout << name << " range high is now coming from " << src->name << endl;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_RANGE_LOW_INPUT_TOGGLE_BUTTON])->b = true;
}

void Oscillator::switch_waveform(int _waveform_type)
{
    sin_on = false;
    tri_on = false;
    saw_on = false;
    sqr_on = false;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_SIN_WAVE_TOGGLE_BUTTON])->b = false;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_TRI_WAVE_TOGGLE_BUTTON])->b = false;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_SAW_WAVE_TOGGLE_BUTTON])->b = false;
    ((Toggle_Button *) graphics_objects[OSCILLATOR_SQR_WAVE_TOGGLE_BUTTON])->b = false;

    if(_waveform_type == SIN)
    {
        waveform_type = SIN;
        sin_on = true;
        ((Toggle_Button *) graphics_objects[OSCILLATOR_SIN_WAVE_TOGGLE_BUTTON])->b = true;
        cout << name << " is now outputting a sine wave" << endl;
    }
    else if(_waveform_type == TRI)
    {
        waveform_type = TRI;
        tri_on = true;
        ((Toggle_Button *) graphics_objects[OSCILLATOR_TRI_WAVE_TOGGLE_BUTTON])->b = true;
        cout << name << " is now outputting a triangle wave" << endl;
    }
    else if(_waveform_type == SAW)
    {
        waveform_type = SAW;
        saw_on = true;
        ((Toggle_Button *) graphics_objects[OSCILLATOR_SAW_WAVE_TOGGLE_BUTTON])->b = true;
        cout << name << " is now outputting a sawtooth wave" << endl;
    }
    else
    {
        waveform_type = SQR;
        sqr_on = true;
        ((Toggle_Button *) graphics_objects[OSCILLATOR_SQR_WAVE_TOGGLE_BUTTON])->b = true;
        cout << name << " is now outputting a square wave" << endl;
    }
}
