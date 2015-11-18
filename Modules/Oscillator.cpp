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
#include "../Graphics_Objects/Input_Text_Box.hpp"
#include "../Graphics_Objects/Input_Toggle_Button.hpp"
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
    current_phase = 0;
    previous_phase_offset = 0;

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
            input_floats[OSCILLATOR_PHASE_OFFSET] = inputs[OSCILLATOR_PHASE_OFFSET]->at(i);
            phase_offset_diff = input_floats[OSCILLATOR_PHASE_OFFSET] - previous_phase_offset;
            previous_phase_offset = input_floats[OSCILLATOR_PHASE_OFFSET];
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
        vector<string> names, texts, prompt_texts, text_offs;
        vector<SDL_Rect> locations;
        vector<SDL_Color> colors, background_colors, color_offs, text_color_ons, text_color_offs;
        vector<TTF_Font *> fonts;
        vector<float> range_lows, range_highs;
        vector<int> input_nums;
        vector<vector<float> *> buffers;
        vector<Module *> parents;
        vector<bool> bs;

        names = {"oscillator frequency (text)", "oscillator phase offset (text)", "oscillator pulse width (text)",
                 "oscillator range low/high (text)"};
        locations = {{x_text, y4, 0, 0}, {x_text, y6, 0, 0}, {x_text, y8, 0, 0}, {x_text, y10, 0, 0}};
        colors = vector<SDL_Color>(4, text_color);
        texts = {"FREQUENCY:", "PHASE OFFSET:", "PULSE WIDTH:", "RANGE LOW & HIGH:"};
        fonts = vector<TTF_Font *>(4, FONT_REGULAR);

        create_text_objects(names, locations, colors, texts, fonts);

        names = {"waveform visualizer (waveform)"};
        locations = {{x_text_box, y3, w_waveform, h_waveform}};
        colors = {color};
        background_colors = {text_color};
        range_lows = {-1};
        range_highs = {1};
        buffers = {&output};

        create_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);

        names = {"oscillator frequency (input text box)", "oscillator phase offset (input text box)",
                 "oscillator pulse width (input text box)", "oscillator range low (input text box)",
                 "oscillator range high (input text box)"};
        locations = {{x_text_box, y5, w_text_box, h_text_box}, {x_text_box, y7, w_text_box, h_text_box},
                     {x_text_box, y9, w_text_box, h_text_box}, {x_text_box, y11, w_range, h_text_box},
                     {x_range_high, y11, w_range, h_text_box}};
        colors = vector<SDL_Color>(5, text_color);
        texts = vector<string>(5, "");
        prompt_texts = vector<string>(5, "# or input");
        fonts = vector<TTF_Font *>(5, FONT_SMALL);
        parents = vector<Module *>(5, this);
        input_nums = {OSCILLATOR_FREQUENCY, OSCILLATOR_PHASE_OFFSET, OSCILLATOR_PULSE_WIDTH,
                      OSCILLATOR_RANGE_LOW, OSCILLATOR_RANGE_HIGH};

        create_input_text_box_objects(names, locations, colors, texts, prompt_texts, fonts, parents, input_nums);

        names = {"oscillator frequency input (input toggle button)",
                 "oscillator phase offset input (input toggle button)",
                 "oscillator pulse width input (input toggle button)",
                 "oscillator range low input (input toggle button)",
                 "oscillator range high input (input toggle button)"};
        locations = {{x_input_toggle_button, y5, w_input_toggle_button, h_text_box},
                     {x_input_toggle_button, y7, w_input_toggle_button, h_text_box},
                     {x_input_toggle_button, y9, w_input_toggle_button, h_text_box},
                     {x_range_low_input_toggle_button, y11, w_input_toggle_button, h_text_box},
                     {x_input_toggle_button, y11, w_input_toggle_button, h_text_box},
                     {x_text_box, y12, w_wave_selector, h_text_box}};
        colors = vector<SDL_Color>(5, WHITE);
        color_offs = vector<SDL_Color>(5, BLACK);
        text_color_ons = vector<SDL_Color>(5, RED);
        text_color_offs = vector<SDL_Color>(5, WHITE);
        fonts = vector<TTF_Font *>(5, FONT_SMALL);
        texts = vector<string>(5, "I");
        text_offs = texts;
        bs = {inputs_live[OSCILLATOR_FREQUENCY], inputs_live[OSCILLATOR_PHASE_OFFSET],
              inputs_live[OSCILLATOR_PULSE_WIDTH], inputs_live[OSCILLATOR_RANGE_LOW],
              inputs_live[OSCILLATOR_RANGE_HIGH]};
        parents = vector<Module *>(5, this);
        input_nums = {OSCILLATOR_FREQUENCY, OSCILLATOR_PHASE_OFFSET,
                      OSCILLATOR_PULSE_WIDTH, OSCILLATOR_RANGE_LOW,
                      OSCILLATOR_RANGE_HIGH};

        create_input_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                           text_color_offs, fonts, texts, text_offs, bs, parents, input_nums);

        names = {"oscillator sin toggle (toggle button)", "oscillator tri toggle (toggle button)",
                 "oscillator saw toggle (toggle button)", "oscillator sqr toggle (toggle button)"};
        locations = {{x_text_box, y12, w_wave_selector, h_text_box},
                     {x_text_box + w_wave_selector + 2, y12, w_wave_selector, h_text_box},
                     {x_text_box + ((w_wave_selector + 2) * 2), y12, w_wave_selector, h_text_box},
                     {x_text_box + ((w_wave_selector + 2) * 3), y12, w_wave_selector, h_text_box}};
        colors = vector<SDL_Color>(4, WHITE);
        color_offs = vector<SDL_Color>(4, BLACK);
        text_color_ons = vector<SDL_Color>(4, BLACK);
        text_color_offs = vector<SDL_Color>(4, WHITE);
        fonts = vector<TTF_Font *>(4, FONT_REGULAR);
        texts = {"SIN", "TRI", "SAW", "SQR"};
        text_offs = {"SIN", "TRI", "SAW", "SQR"};
        bs = {sin_on, tri_on, saw_on, sqr_on};
        parents = vector<Module *>(4, this);

        create_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                     text_color_offs, fonts, texts, text_offs, bs, parents);
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
