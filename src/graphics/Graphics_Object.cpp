/*
 * Matthew Diamond 2015
 * Member functions for the Graphics_Object class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <vector>

// Included files
#include "graphics_config.hpp"

// Included SDL components
#include "SDL.h"

// Included graphics classes
#include "Graphics_Object.hpp"
#include "Graphics_Listener.hpp"

/************************************
 * GRAPHICS_OBJECT MEMBER FUNCTIONS *
 ************************************/

/*
 * Constructor.
 */
Graphics_Object::Graphics_Object(std::string name_,
                                 GraphicsObjectType graphics_object_type_,
                                 Graphics_Listener *listener_, SDL_Rect location_,
                                 SDL_Color color_) :
    name(name_), graphics_object_type(graphics_object_type_),
    location(location_), color(color_), updated(true), listener(listener_)
{}

/*
 * Destructor.
 */
Graphics_Object::~Graphics_Object()
{}

/*
 * Update the location of this graphics object to the one given.
 */
void Graphics_Object::update_location(SDL_Rect location_)
{
    location = location_;
    updated = true;
}

/*
 * Set the color of this graphics object to the one given.
 */
void Graphics_Object::set_color(SDL_Color color_)
{
    color = color_;
    updated = true;
}

/*
 * Send this graphics object to the event handling function of its listener.
 * Return true if the listener does anything, false otherwise
 */
bool Graphics_Object::clicked()
{
    if(listener != nullptr)
    {
        return listener->handle_event(this);
    }
    return false;
}

/*
 * Check if this graphics object currently has the mouse over it. Return true
 * if so, false otherwise.
 */
bool Graphics_Object::mouse_over()
{
    if(MOUSE_X >= location.x && MOUSE_X < location.x + location.w
       && MOUSE_Y >= location.y && MOUSE_Y < location.y + location.h)
    {
        return true;
    }

    return false;
}

