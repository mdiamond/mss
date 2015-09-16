/*
 * Matthew Diamond 2015
 * The oscillator module. This module is capable of
 * generating a sine, square, triangle, and sawtooth
 * wave given a frequency and pulse width. It can then
 * be modulated with some other oscillator. The
 * resulting waveform can then be processed or output.
 * This module fills its output buffer with the generated signal.
 * This file defines the class and includes any files or
 * libraries necessary for Output.cpp.
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
        Oscillator(std::string *);
        virtual ~Oscillator();
        // member functions
        virtual void process();
        virtual void copy_graphics_data();
        virtual void render();
        void render_text();
};

#endif
