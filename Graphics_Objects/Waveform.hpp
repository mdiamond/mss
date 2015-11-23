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
        // The background color, the range of display,
        // the pointer to the buffer to be rendered,
        // and an internal buffer to store the buffer
        // in the main thread
        SDL_Color *background_color;
        float range_low, range_high;
        std::vector<float> *buffer, render_buffer;

        // Constructor and destructor
        Waveform(std::string, SDL_Rect, SDL_Color *, SDL_Color *,
                 float, float, std::vector<float> *);
        virtual ~Waveform();

        // Virtual member functions
        virtual void render();
        virtual void clicked();

        // Member functions
        //   Calculate the y pixel location for a given sample
        float calculate_y(int);
        //   Copy the buffer to be rendered into internal memory
        void copy_buffer();
};

#endif
