/*
 * Matthew Diamond 2015
 * Member functions for the Graphics_Object class.
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

// Included classes
#include "Graphics_Object.hpp"

/************************************
 * GRAPHICS_OBJECT MEMBER FUNCTIONS *
 ************************************/

/*
 * Constructor.
 */
Graphics_Object::Graphics_Object(std::string _name, int _type, Module *_parent,
								 SDL_Rect _location, SDL_Color *_color) :
	name(_name), type(_type), parent(_parent),
    location(_location), color(_color), updated(true)
{

}

/*
 * Destructor.
 */
Graphics_Object::~Graphics_Object()
{

}

/*
 * Check if this graphics object was clicked.
 * Return true if it was, false if it wasn't.
 */
bool Graphics_Object::was_clicked()
{
    if(MOUSE_X >= location.x && MOUSE_X < location.x + location.w &&
       MOUSE_Y >= location.y && MOUSE_Y < location.y + location.h)
        return true;

    return false;
}

/*
 * Update the location of this graphics object to the one given.
 */
void Graphics_Object::update_location(SDL_Rect _location)
{
    if(type == BUTTON)
        ((Button *) this)->update_location(_location);
    else if(type == TEXT_BOX || type == INPUT_TEXT_BOX)
        ((Text_Box *) this)->update_location(_location);
    else if(type == TOGGLE_BUTTON || type == INPUT_TOGGLE_BUTTON)
        ((Toggle_Button *) this)->update_location(_location);
    else if(type == WAVEFORM)
        ((Waveform *) this)->update_location(_location);
    else
        location = _location;
}
