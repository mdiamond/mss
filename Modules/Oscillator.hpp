/*
 * Matthew Diamond 2015
 * The oscillator module. This module is capable of
 * generating a sine, square, triangle, and sawtooth
 * wave given a frequency and pulse width. It can then
 * be modulated with some other oscillator. The
 * resulting waveform can then be processed or output.
 * This module fills its output buffer with the generated signal.
 * This file defines the class.
 */

#ifndef synth_oscillator_h
#define synth_oscillator_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/*******************************
 * OSCILLATOR CLASS DEFINITION *
 *******************************/

struct Oscillator_Data
{
    // Oscillator settings and frequency modulator/
    // modulation settings
    float frequency;
    float shifted_frequency;
    std::string frequency_str;
    float phase;
    float amplitude;
    int fm_on;
    float modulation_index;
    // Output buffer
    std::vector<float> *output;
};

class Oscillator: public Module
{
    public:
        struct Oscillator_Data audio, graphics;

        // oscillator to modulate this one
        Oscillator *modulator;
        // Constructor and destructor
        Oscillator(std::string *, int);
        virtual ~Oscillator();
        // member functions
        virtual void process();
        virtual void calculate_unique_graphics_objects();
        virtual void copy_graphics_data();
        Graphics_Object *calculate_waveform_visualizer();
        Graphics_Object *calculate_frequency();
        void calculate_text_objects();
        void set_frequency(float);
};

#endif