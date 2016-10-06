/*
 * Matthew Diamond 2015
 * Code adapted very closely from Joel Sjögren's stack overflow post here:
 * https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
 * The color modifier class. Use this class to create a modifier that can be output before any
 * stdout text to change its color.
 */

#ifndef SYNTH_COLOR_MODIFIER_HPP
#define SYNTH_COLOR_MODIFIER_HPP

/************
 * INCLUDES *
 ************/

// Included libraries
#include <ostream>

/********************
 * COLOR CODES ENUM *
 ********************/

enum ColorCode
{
    FG_RED      = 31,
    FG_GREEN    = 32,
    FG_BLUE     = 34,
    FG_PINK     = 35,
    FG_DEFAULT  = 39,
    BG_RED      = 41,
    BG_GREEN    = 42,
    BG_BLUE     = 44,
    BG_DEFAULT  = 49
};

/***********************************
 * COLOR MODIFIER CLASS DEFINITION *
 ***********************************/

class Color_Modifier
{
public:
    ColorCode code;

    Color_Modifier(ColorCode pCode) : code(pCode)
    {}

    friend std::ostream& operator<<(std::ostream& os, const Color_Modifier& mod)
    {
        return os << "\033[" << mod.code << "m";
    }
};

#endif
