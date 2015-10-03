/*
 * Matthew Diamond 2015
 * The output module. All audio requested by the callback
 * function comes through here. This module simply
 * processes the modules it depends upon, then makes available
 * full audio buffers to the audio callback function. This file
 * defines the class.
 */

#ifndef synth_output_h
#define synth_output_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/***************************
 * OUTPUT CLASS DEFINITION *
 ***************************/

struct Output_Data
{
    // Input buffers
    std::vector<float> *input_l;
    std::vector<float> *input_r;
};

class Output: public Module
{
    public:
        // Two copies of the output data,
        // one for access by the audio thread,
        // one for access by the main thread
        struct Output_Data audio, graphics;
        // Constructor and destructor
        Output(int);
        virtual ~Output();
        // Member functions
        virtual void process();
        virtual void calculate_unique_graphics_objects();
        virtual void copy_graphics_data();
        void calculate_text_objects();
        Graphics_Object *calculate_on_off_button();
        Graphics_Object *calculate_input_1eft();
        Graphics_Object *calculate_input_right();
        void toggle_audio_on();
};

#endif
