/*
 * Matthew Diamond 2015
 * The Page graphics object. This object is a page
 * full of modules or a special page for things like
 * saving, adding a module, etc.
 */

#ifndef SYNTH_PAGE_HPP
#define SYNTH_PAGE_HPP

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL.h"

/*************************
 * PAGE CLASS DEFINITION *
 *************************/

class Page: public Graphics_Object
{
public:
    std::vector<Graphics_Object *> graphics_objects;

    // Constructor and destructor
    Page(std::string, SDL_Rect, SDL_Color *,
         std::vector<Graphics_Object *> *);
    virtual ~Page();

    // Member functions
    void render();
    void clicked();
};

#endif

