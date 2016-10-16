/*
 * Matthew Diamond 2015
 * The Text Box graphics object. This object allows the user
 * to type into it. The data entered can be used to change parameters.
 */

#ifndef MSS_TEXT_BOX_HPP
#define MSS_TEXT_BOX_HPP

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL.h"

// Included graphics classes
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Text.hpp"

/*****************************
 * TEXT BOX CLASS DEFINITION *
 *****************************/

class Text_Box: public Graphics_Object
{
public:
    // The text color, whether or not this text box
    // is active, the font, the background rectangle,
    // the text, the prompt text, and the typing text
    SDL_Color text_color;
    bool active;
    TTF_Font *font;
    Rect background;
    Text text;
    Text prompt_text;
    Text typing_text;

    // Constructor and destructor
    Text_Box(std::string, SDL_Rect, SDL_Color, SDL_Color,
             std::string, TTF_Font *, Module *);
    virtual ~Text_Box();

    // Member functions
    void render();
    void clicked();
    //   Add character to the typing buffer
    void add_characters(char *);
    //   Delete the final character from the typing buffer
    void delete_character();
    //   Confirm the entry in the text box
    virtual void entered();
    //   Override the default graphics object update_location() function
    void update_location(SDL_Rect);
    //   Update the currently displayed text
    void update_current_text(std::string);
    //   Cancel text input
    void cancel_input();
    //   Set the colors of the text box
    void set_colors(SDL_Color, SDL_Color);
};

#endif

