/*
 * Matthew Diamond 2015
 * Member functions for the Page class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "../main.hpp"

// Included classes
#include "../Graphics_Object.hpp"
#include "Page.hpp"

using namespace std;

/*************************
 * PAGE MEMBER FUNCTIONS *
 *************************/

/*
 * Constructor.
 */
Page::Page(string _name, SDL_Rect _location, SDL_Color _color,
           vector<Graphics_Object *> *_graphics_objects,
           vector<Page *> *_sub_pages) :
    Graphics_Object(_name, PAGE, NULL, _location, _color)
{
    if(_sub_pages != NULL)
        sub_pages = vector<Page *>(*_sub_pages);
    else
        sub_pages = vector<Page *>();
    if(_graphics_objects != NULL)
        graphics_objects = vector<Graphics_Object *>(*_graphics_objects);
    else
        graphics_objects = vector<Graphics_Object *>();
}

/*
 * Destructor.
 */
Page::~Page()
{
    for(unsigned int i = 0; i < sub_pages.size(); i ++)
        delete sub_pages[i];
}

/*
 * Render the graphics objects.
 */
void Page::render()
{
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
        graphics_objects[i]->render();
    for(unsigned int i = 0; i < sub_pages.size(); i ++)
        sub_pages[i]->render();
}

/*
 * Send the click to all the contained graphics objects.
 */
void Page::clicked()
{
    for(unsigned int i = 0; i < graphics_objects.size(); i ++)
        if(graphics_objects[i]->was_clicked())
            graphics_objects[i]->clicked();
    for(unsigned int i = 0; i < sub_pages.size(); i ++)
        if(sub_pages[i]->was_clicked())
            sub_pages[i]->clicked();
}
