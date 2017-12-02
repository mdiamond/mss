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
#include "graphics_config.hpp"

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

    SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLines(RENDERER, &points[0], points.size());
}

/*
 * Waveforms do not respond to clicks.
 */
bool Waveform::clicked()
{
    return false;
}

/*
 * Update the location of this waveform object.
 */
void Waveform::update_location(SDL_Rect location_)
{
    location = location_;
    background.update_location(location_);
    updated = true;
}

/*
 * Scale a sample and return the scaled sample.
 */
float scale_sample(float sample, float original_low,
                   float original_high, float low, float high)
{
    sample = (sample - original_low) / (original_high - original_low);
    sample *= high - low;
    sample += low;

    return sample;
}

