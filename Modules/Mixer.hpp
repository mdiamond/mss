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
    MIXER_OUTPUT_WAVEFORM = 3,
    MIXER_SIGNAL_1_TEXT,
    MIXER_SIGNAL_1_TEXT_BOX,
    MIXER_SIGNAL_1_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_1_MULTIPLIER_TEXT_BOX,
    MIXER_SIGNAL_1_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_2_TEXT_BOX,
    MIXER_SIGNAL_2_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_2_MULTIPLIER_TEXT_BOX,
    MIXER_SIGNAL_2_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_3_TEXT_BOX,
    MIXER_SIGNAL_3_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_3_MULTIPLIER_TEXT_BOX,
    MIXER_SIGNAL_3_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_4_TEXT_BOX,
    MIXER_SIGNAL_4_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_4_MULTIPLIER_TEXT_BOX,
    MIXER_SIGNAL_4_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_5_TEXT_BOX,
    MIXER_SIGNAL_5_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_5_MULTIPLIER_TEXT_BOX,
    MIXER_SIGNAL_5_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_6_TEXT_BOX,
    MIXER_SIGNAL_6_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_6_MULTIPLIER_TEXT_BOX,
    MIXER_SIGNAL_6_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_7_TEXT_BOX,
    MIXER_SIGNAL_7_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_7_MULTIPLIER_TEXT_BOX,
    MIXER_SIGNAL_7_MULTIPLIER_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_8_TEXT_BOX,
    MIXER_SIGNAL_8_INPUT_TOGGLE_BUTTON,
    MIXER_SIGNAL_8_MULTIPLIER_TEXT_BOX,
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
        // Set up floats, buffers, and strings to represent the
        // different channels and their respective attenuations
        float signal_1_float, signal_1_multiplier_float,
              signal_2_float, signal_2_multiplier_float,
              signal_3_float, signal_3_multiplier_float,
              signal_4_float, signal_4_multiplier_float,
              signal_5_float, signal_5_multiplier_float,
              signal_6_float, signal_6_multiplier_float,
              signal_7_float, signal_7_multiplier_float,
              signal_8_float, signal_8_multiplier_float;
        std::vector<float> *signal_1_input, *signal_1_multiplier_input,
                           *signal_2_input, *signal_2_multiplier_input,
                           *signal_3_input, *signal_3_multiplier_input,
                           *signal_4_input, *signal_4_multiplier_input,
                           *signal_5_input, *signal_5_multiplier_input,
                           *signal_6_input, *signal_6_multiplier_input,
                           *signal_7_input, *signal_7_multiplier_input,
                           *signal_8_input, *signal_8_multiplier_input;
        std::string signal_1_str, signal_1_multiplier_str,
                    signal_2_str, signal_2_multiplier_str,
                    signal_3_str, signal_3_multiplier_str,
                    signal_4_str, signal_4_multiplier_str,
                    signal_5_str, signal_5_multiplier_str,
                    signal_6_str, signal_6_multiplier_str,
                    signal_7_str, signal_7_multiplier_str,
                    signal_8_str, signal_8_multiplier_str;
        // A boolean to represent whether or not auto attenuation
        // should be used
        bool auto_attenuate;

        // Constructor and destructor
        Mixer(std::string, int);
        virtual ~Mixer();

        // Member functions
        virtual void process();
        virtual void calculate_unique_graphics_objects();
        virtual void update_unique_graphics_objects();
        virtual void update_unique_control_values();
        void set_signal(Module *, short);
        void set_signal_multiplier(Module *, short);
        void set_signal_multiplier(float, short);
};

#endif
