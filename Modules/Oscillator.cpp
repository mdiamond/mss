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
#include "../graphics_object_utils.hpp"
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

void Oscillator::update_control_values()
{
    
}

void Oscillator::calculate_unique_graphics_object_locations()
{
    int x_text, x_text_box, w_text_box, h_text_box,
        x_input_toggle_button, w_input_toggle_button,
        w_waveform, h_waveform,
        y3, y4, y5, y6, y7, y8, y9, y10, y11, y12,
        x_range_high, x_range_low_input_toggle_button, w_range,
        w_wave_selector;

    x_text = upper_left.x + MODULE_BORDER_WIDTH + 2;
    x_text_box = upper_left.x + MODULE_BORDER_WIDTH + 2;
    w_text_box = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 4) - 11;
    h_text_box = 15;
    x_input_toggle_button = x_text_box + w_text_box + 1;
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
    w_range = (((MODULE_WIDTH / 2) - MODULE_BORDER_WIDTH) - 3) - 11;
    x_range_low_input_toggle_button = x_text_box + w_range + 1;
    w_wave_selector = ((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) / 4) - 2;

    graphics_object_locations.push_back({x_text, y4, 0, 0});
    graphics_object_locations.push_back({x_text, y6, 0, 0});
    graphics_object_locations.push_back({x_text, y8, 0, 0});
    graphics_object_locations.push_back({x_text, y10, 0, 0});
    graphics_object_locations.push_back({x_text_box, y3, w_waveform, h_waveform});
    graphics_object_locations.push_back({x_text_box, y5, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y7, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y9, w_text_box, h_text_box});
    graphics_object_locations.push_back({x_text_box, y11, w_range, h_text_box});
    graphics_object_locations.push_back({x_range_high, y11, w_range, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y5, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y7, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y9, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_range_low_input_toggle_button, y11, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_input_toggle_button, y11, w_input_toggle_button, h_text_box});
    graphics_object_locations.push_back({x_text_box, y12, w_wave_selector, h_text_box});
    graphics_object_locations.push_back({x_text_box + w_wave_selector + 2, y12, w_wave_selector, h_text_box});
    graphics_object_locations.push_back({x_text_box + ((w_wave_selector + 2) * 2), y12, w_wave_selector, h_text_box});
    graphics_object_locations.push_back({x_text_box + ((w_wave_selector + 2) * 3), y12, w_wave_selector, h_text_box});
}

void Oscillator::initialize_unique_graphics_objects()
{
    vector<string> names, texts, prompt_texts, text_offs;
    vector<SDL_Rect> locations;
    vector<SDL_Color> colors, background_colors, color_offs, text_colors, text_color_ons, text_color_offs;
    vector<TTF_Font *> fonts;
    vector<float> range_lows, range_highs;
    vector<int> input_nums;
    vector<vector<float> *> buffers;
    vector<Module *> parents;
    vector<bool> bs;

    vector<Graphics_Object *> tmp_graphics_objects;

    names = {"oscillator frequency (text)", "oscillator phase offset (text)", "oscillator pulse width (text)",
             "oscillator range low/high (text)"};
    locations = {graphics_object_locations[OSCILLATOR_FREQUENCY_TEXT],
                 graphics_object_locations[OSCILLATOR_PHASE_OFFSET_TEXT],
                 graphics_object_locations[OSCILLATOR_PULSE_WIDTH_TEXT],
                 graphics_object_locations[OSCILLATOR_RANGE_TEXT]};
    colors = vector<SDL_Color>(4, text_color);
    texts = {"FREQUENCY:", "PHASE OFFSET:", "PULSE WIDTH:", "RANGE LOW & HIGH:"};
    fonts = vector<TTF_Font *>(4, FONT_REGULAR);

    tmp_graphics_objects = initialize_text_objects(names, locations, colors, texts, fonts);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {"waveform visualizer (waveform)"};
    locations = {graphics_object_locations[OSCILLATOR_OUTPUT_WAVEFORM]};
    colors = {color};
    background_colors = {text_color};
    range_lows = {-1};
    range_highs = {1};
    buffers = {&output};

    tmp_graphics_objects = initialize_waveform_objects(names, locations, colors, background_colors, range_lows, range_highs, buffers);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());

    names = {"oscillator frequency (input text box)", "oscillator phase offset (input text box)",
             "oscillator pulse width (input text box)", "oscillator range low (input text box)",
             "oscillator range high (input text box)"};
    locations = {graphics_object_locations[OSCILLATOR_FREQUENCY_INPUT_TEXT_BOX],
                 graphics_object_locations[OSCILLATOR_PHASE_OFFSET_INPUT_TEXT_BOX],
                 graphics_object_locations[OSCILLATOR_PULSE_WIDTH_INPUT_TEXT_BOX],
                 graphics_object_locations[OSCILLATOR_RANGE_LOW_INPUT_TEXT_BOX],
                 graphics_object_locations[OSCILLATOR_RANGE_HIGH_INPUT_TEXT_BOX]};
    colors = vector<SDL_Color>(5, text_color);
    text_colors = vector<SDL_Color>(5, color);
    prompt_texts = vector<string>(5, "# or input");
    fonts = vector<TTF_Font *>(5, FONT_SMALL);
    parents = vector<Module *>(5, this);
    input_nums = {OSCILLATOR_FREQUENCY, OSCILLATOR_PHASE_OFFSET, OSCILLATOR_PULSE_WIDTH,
                  OSCILLATOR_RANGE_LOW, OSCILLATOR_RANGE_HIGH};

    initialize_input_text_box_objects(names, locations, colors, text_colors, prompt_texts, fonts, parents, input_nums);

    names = {"oscillator frequency input (input toggle button)",
             "oscillator phase offset input (input toggle button)",
             "oscillator pulse width input (input toggle button)",
             "oscillator range low input (input toggle button)",
             "oscillator range high input (input toggle button)"};
    locations = {graphics_object_locations[OSCILLATOR_FREQUENCY_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[OSCILLATOR_PHASE_OFFSET_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[OSCILLATOR_PULSE_WIDTH_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[OSCILLATOR_RANGE_LOW_INPUT_TOGGLE_BUTTON],
                 graphics_object_locations[OSCILLATOR_RANGE_HIGH_INPUT_TOGGLE_BUTTON]};
    colors = vector<SDL_Color>(5, RED);
    color_offs = vector<SDL_Color>(5, text_color);
    text_color_ons = vector<SDL_Color>(5, WHITE);
    text_color_offs = vector<SDL_Color>(5, color);
    fonts = vector<TTF_Font *>(5, FONT_SMALL);
    texts = vector<string>(5, "I");
    text_offs = texts;
    bs = vector<bool>(5, false);
    parents = vector<Module *>(5, this);
    input_nums = {OSCILLATOR_FREQUENCY, OSCILLATOR_PHASE_OFFSET,
                  OSCILLATOR_PULSE_WIDTH, OSCILLATOR_RANGE_LOW,
                  OSCILLATOR_RANGE_HIGH};

    initialize_input_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                       text_color_offs, fonts, texts, text_offs, bs, parents, input_nums);

    names = {"oscillator sin toggle (toggle button)", "oscillator tri toggle (toggle button)",
             "oscillator saw toggle (toggle button)", "oscillator sqr toggle (toggle button)"};
    locations = {graphics_object_locations[OSCILLATOR_SIN_WAVE_TOGGLE_BUTTON],
                 graphics_object_locations[OSCILLATOR_TRI_WAVE_TOGGLE_BUTTON],
                 graphics_object_locations[OSCILLATOR_SAW_WAVE_TOGGLE_BUTTON],
                 graphics_object_locations[OSCILLATOR_SQR_WAVE_TOGGLE_BUTTON]};
    colors = vector<SDL_Color>(4, text_color);
    color_offs = vector<SDL_Color>(4, BLACK);
    text_color_ons = vector<SDL_Color>(4, BLACK);
    text_color_offs = vector<SDL_Color>(4, WHITE);
    fonts = vector<TTF_Font *>(4, FONT_REGULAR);
    texts = {"SIN", "TRI", "SAW", "SQR"};
    text_offs = {"SIN", "TRI", "SAW", "SQR"};
    bs = {sin_on, tri_on, saw_on, sqr_on};
    parents = vector<Module *>(4, this);

    tmp_graphics_objects = initialize_toggle_button_objects(names, locations, colors, color_offs, text_color_ons,
                                                        text_color_offs, fonts, texts, text_offs, bs, parents);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(), tmp_graphics_objects.end());
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
