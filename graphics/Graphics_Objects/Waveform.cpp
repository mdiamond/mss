/*
 * Matthew Diamond 2015
 * Member functions for the Waveform class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL.h"

// Included files
#include "main.hpp"
#include "signal_processing.hpp"

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Objects/Waveform.hpp"

/*****************************
 * WAVEFORM MEMBER FUNCTIONS *
 *****************************/

/*
 * Constructor.
 */
Waveform::Waveform(std::string name_, SDL_Rect location_, SDL_Color color_,
                   SDL_Color background_color_, std::vector<float> *buffer_) :
    Graphics_Object(name_, WAVEFORM, NULL, location_, color_),
    background_color(background_color_), range_low(-1),
    range_high(1), buffer(buffer_),
    render_buffer(std::vector<float>(location.w, 0)),
    background(Rect(name_ + " background rect", location_,
                    background_color_, NULL))
{}

/*
 * Destructor.
 */
Waveform::~Waveform()
{}

/*
 * Calculate the y pixel location in the waveform line at the given sample i.
 */
float Waveform::calculate_y(int i)
{
    float sample;
    float y;

    if(range_low != -1 || range_high != 1)
    {
        sample = scale_sample(render_buffer[i], range_low, range_high, -1, 1);
    }
    else
    {
        sample = render_buffer[i];
    }

    y = (location.y + (location.h / 2.0)) + (-sample * (location.h / 2.0));

    if(y < location.y)
    {
        y = location.y;
    }
    if(y > location.y + location.h)
    {
        y = location.y + location.h;
    }

    return y;
}

/*
 * Copy the buffer to be rendered to inside of this waveform object. This
 * should always be done with audio locked before rendering to avoid
 * interleaving between the main thread and the audio thread.
 */
void Waveform::copy_buffer()
{
    int index = 0;
    for(unsigned int i = BUFFER_SIZE - location.w; i < BUFFER_SIZE; i ++)
    {
        if(buffer != NULL)
        {
            render_buffer[index] = (*buffer)[i];
        }
        else
        {
            render_buffer[index] = 0;
        }
        index ++;
    }
}

/*
 * Render the waveform.
 */
void Waveform::render()
{
    SDL_Point zero = {0, 0};
    std::vector<SDL_Point> points;

    points = std::vector<SDL_Point>(location.w, zero);

    for(unsigned int i = 0; i < render_buffer.size(); i ++)
    {
        points[i].x = location.x + i;
        points[i].y = calculate_y(i);
    }

    background.render();

    if(!SELECTING_SRC
       || (SELECTING_SRC && parent != NULL && parent->mouse_over()))
    {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    }
    else
    {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a / 2);
    }
    SDL_RenderDrawLines(RENDERER, &points[0], points.size());
}

/*
 * Waveforms do not respond to clicks.
 */
void Waveform::clicked()
{}

/*
 * Update the location of this waveform object.
 */
void Waveform::update_location(SDL_Rect location_)
{
    location = location_;
    background.update_location(location_);
    updated = true;
}

