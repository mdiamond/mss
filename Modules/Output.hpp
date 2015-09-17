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
        struct Output_Data audio, graphics;

        // Constructor and destructor
        Output();
        virtual ~Output();
        // Member functions
        virtual void process();
        virtual void calculate_unique_graphics_objects();
        virtual void copy_graphics_data();
        virtual void update_unique_graphics_objects();
};

#endif
