/*
 * Matthew Diamond 2015
 * The mixer module. This module attenuates and then sums
 * a few signals. It is capable of automatically attenuating
 * based on how many channels are active if desired.
 */

#ifndef synth_mixer_h
#define synth_mixer_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/*******************************
 * MIXER GRAPHICS OBJECTS ENUM *
 *******************************/

enum Mixer_Graphics_Objects
{
    MIXER_SIGNALS_TEXT = 3,
    MIXER_OUTPUT_WAVEFORM,
    MIXER_SIGNAL_1_INPUT_TEXT_BOX,
    MIXER_SIGNAL_1_MULTIPLIER_INPUT_TEXT_BOX,
    MIXER_SIGNAL_2_INPUT_TEXT_BOX,
    MIXER_SIGNAL_2_MULTIPLIER_INPUT_TEXT_BOX,
    MIXER_SIGNAL_3_INPUT_TEXT_BOX,
    MIXER_SIGNAL_3_MULTIPLIER_INPUT_TEXT_BOX,
    MIXER_SIGNAL_4_INPUT_TEXT_BOX,
    MIXER_SIGNAL_4_MULTIPLIER_INPUT_TEXT_BOX,
    MIXER_SIGNAL_5_INPUT_TEXT_BOX,
    MIXER_SIGNAL_5_MULTIPLIER_INPUT_TEXT_BOX,
    MIXER_SIGNAL_6_INPUT_TEXT_BOX,
    MIXER_SIGNAL_6_MULTIPLIER_INPUT_TEXT_BOX,
    MIXER_SIGNAL_7_INPUT_TEXT_BOX,
    MIXER_SIGNAL_7_MULTIPLIER_INPUT_TEXT_BOX,
    MIXER_SIGNAL_8_INPUT_TEXT_BOX,
    MIXER_SIGNAL_8_MULTIPLIER_INPUT_TEXT_BOX,
    MIXER_SIGNAL_1_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_1_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_2_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_2_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_3_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_3_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_4_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_4_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_5_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_5_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_6_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_6_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_7_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_7_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_8_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_8_MULTIPLIER_INPUT_TOGGLE_BUTTON,
};

/***************************
 * MIXER DEPENDENCIES ENUM *
 ***************************/

enum Mixer_Dependencies
{
    MIXER_SIGNAL_1 = 0,
    MIXER_SIGNAL_1_MULTIPLIER,
    MIXER_SIGNAL_2,
    MIXER_SIGNAL_2_MULTIPLIER,
    MIXER_SIGNAL_3,
    MIXER_SIGNAL_3_MULTIPLIER,
    MIXER_SIGNAL_4,
    MIXER_SIGNAL_4_MULTIPLIER,
    MIXER_SIGNAL_5,
    MIXER_SIGNAL_5_MULTIPLIER,
    MIXER_SIGNAL_6,
    MIXER_SIGNAL_6_MULTIPLIER,
    MIXER_SIGNAL_7,
    MIXER_SIGNAL_7_MULTIPLIER,
    MIXER_SIGNAL_8,
    MIXER_SIGNAL_8_MULTIPLIER,
};

/**************************
 * MIXER CLASS DEFINITION *
 **************************/

class Mixer: public Module
{
    public:
        // A boolean to represent whether or not auto attenuation
        // should be used
        bool auto_attenuate;

        // Constructor and destructor
        Mixer();
        virtual ~Mixer();

        // Member functions, explained in Module.hpp
        virtual void process();
        virtual void update_control_values();
        virtual void calculate_unique_graphics_object_locations();
        virtual void initialize_unique_graphics_objects();
};

#endif
