/*
 * Matthew Diamond 2015
 * The Text graphics object. This object is just some text in some place in the
 * window, with some color.
 */

#ifndef GRAPHICS_TEXT_HPP
#define GRAPHICS_TEXT_HPP

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL.h"
#include "SDL_ttf.h"

/*************************
 * TEXT CLASS DEFINITION *
 *************************/

class Text: public Graphics_Object
{
public:
    // The font of the text, the text as a string, and the text as a texture
    TTF_Font *font;
    std::string text;
    SDL_Texture *texture;
    int max_width = 1000;

    // Constructor and destructor
    Text(std::string, SDL_Rect, SDL_Color, std::string);
    virtual ~Text();

    // Member functions
    virtual void render();
    virtual bool clicked();
    //   Update the text displayed
    void update_text(std::string);
};

#endif

