/*
 * Matthew Diamond 2015
 * The output module. All audio requested by the callback
 * function comes through here. This module simply
 * processes the modules it depends upon, then makes available
 * full audio buffers to the audio callback function. This file
 * defines the class.
 */

#ifndef SYNTH_OUTPUT_HPP
#define SYNTH_OUTPUT_HPP

/************
 * INCLUDES *
 ************/

// No includes necessary

/***************************
 * OUTPUT CLASS DEFINITION *
 ***************************/

class Output: public Module
{
public:
    // Output graphics objects enum
    enum OuputGraphicsObjects
    {
        OUTPUT_AUDIO_TOGGLE_TEXT = 3,
        OUTPUT_INPUT_L_TEXT,
        OUTPUT_INPUT_R_TEXT,
        OUTPUT_INPUT_L_WAVEFORM,
        OUTPUT_INPUT_R_WAVEFORM,
        OUTPUT_INPUT_L_INPUT_TEXT_BOX,
        OUTPUT_INPUT_R_INPUT_TEXT_BOX,
        OUTPUT_INPUT_L_INPUT_TOGGLE_BUTTON,
        OUTPUT_INPUT_R_INPUT_TOGGLE_BUTTON,
        OUTPUT_AUDIO_TOGGLE_TOGGLE_BUTTON
    };

    // Output dependencies enum
    enum OuputDependencies
    {
        OUTPUT_INPUT_L = 0,
        OUTPUT_INPUT_R
    };

    // Constructor and destructor
    Output();
    virtual ~Output();

    // Member functions, explained in Module.hpp
    virtual void process();
    virtual void update_control_values();
    virtual void calculate_unique_graphics_object_locations();
    virtual void initialize_unique_graphics_objects();
    virtual std::string get_unique_text_representation();
    virtual void button_function(Button *);
    virtual void toggle_button_function(Toggle_Button *);

    // Member functions particular to this module
    //   Toggle audio processing
    void toggle_audio_on();
};

#endif

