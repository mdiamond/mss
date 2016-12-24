/*
 * Matthew Diamond 2015
 * The Text Box graphics object. This object allows the user
 * to type into it. The data entered can be used to change parameters.
 */

#ifndef GRAPHICS_TEXT_BOX_HPP
#define GRAPHICS_TEXT_BOX_HPP

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
    // The text color, whether or not this text box is active, whether or not
    // the most recently entered text could be converted to a float, and the
    // float if so, the font, the background rectangle, the text, the prompt
    // text, and the typing text
    SDL_Color text_color;
    bool active;
    bool is_float;
    float as_float;
    TTF_Font *font;
    Rect background;
    Text text;
    Text prompt_text;
    Text typing_text;

    // Constructor and destructor
    Text_Box(std::string, SDL_Rect, SDL_Color, SDL_Color,
             std::string, Graphics_Listener *);
    virtual ~Text_Box();

    // Public member functions
    virtual void render();
    virtual bool clicked();
    virtual void update_location(SDL_Rect);
    //   Add character to the typing buffer
    void add_characters(char *);
    //   Delete the final character from the typing buffer
    void delete_character();
    //   Confirm the entry in the text box
    virtual void entered();
    //   Update the currently displayed text
    void update_current_text(std::string);
    //   Cancel text input
    void cancel_input();
    //   Set the colors of the text box
    void set_colors(SDL_Color, SDL_Color);

private:
    // Private member functions
    //   Check if the string entered is a float, store it
    void store_float();
};

#endif

