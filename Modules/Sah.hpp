/*
 * Matthew Diamond 2015
 * The multiplier module. This module applies control values
 * to another signal. Essentially, it just multiplies two signals,
 * but it is also possible to specify an amount to apply for the
 * control values signal.
 */

#ifndef synth_sah_h
#define synth_sah_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/*****************************
 * SAH GRAPHICS OBJECTS ENUM *
 *****************************/

enum Sah_Graphics_Objects
{
    SAH_RESET_SAMPLER_BUTTON = 3,
    SAH_INPUT_TEXT,
    SAH_HOLD_TIME_TEXT,
    SAH_OUTPUT_WAVEFORM,
    SAH_SIGNAL_INPUT_TEXT_BOX,
    SAH_HOLD_TIME_INPUT_TEXT_BOX,
    SAH_SIGNAL_INPUT_TOGGLE_BUTTON,
    SAH_HOLD_TIME_INPUT_TOGGLE_BUTTON
};

/*************************
 * SAH DEPENDENCIES ENUM *
 *************************/

enum Sah_Dependencies
{
    SAH_SIGNAL = 0,
    SAH_HOLD_TIME
};

/************************
 * SAH CLASS DEFINITION *
 ************************/

class Sah: public Module
{
    float sample;
    double time_to_next_sample;

    public:
        // Constructor and destructor
        Sah();
        virtual ~Sah();

        // Member functions, explained in Module.hpp
        virtual void process();
        virtual void update_control_values();
        virtual void calculate_unique_graphics_object_locations();
        virtual void initialize_unique_graphics_objects();
        virtual std::string get_unique_text_representation();

        // Member functions particular to this module
        //   Reset phase
        void reset_sampler();
};

#endif
