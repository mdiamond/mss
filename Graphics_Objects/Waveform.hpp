/*
 * Matthew Diamond 2015
 * The Waveform graphics object. This object is a waveform
 * visualizer for a given buffer rendered wherever is desired.
 */

#ifndef synth_waveform_h
#define synth_waveform_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

/*****************************
 * WAVEFORM CLASS DEFINITION *
 *****************************/

class Waveform: public Graphics_Object
{
    public:
        float range_low, range_high;
        std::vector<float> *buffer, render_buffer;
        // Constructor and destructor
        Waveform(std::string, SDL_Rect, SDL_Color, float, float, std::vector<float> *);
        virtual ~Waveform();
        // Member functions
        virtual void render();
        virtual void clicked();
        float calculate_y(int);
        void copy_buffer();
};

#endif
