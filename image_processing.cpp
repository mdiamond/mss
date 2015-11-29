/*
 * Matthew Diamond 2015
 * Image processing functions and graphics related
 * initialization functions
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <queue>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "graphics_object_utils.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "main_helpers.hpp"
#include "signal_processing.hpp"

// Included classes
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Page.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Waveform.hpp"
#include "Module.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"

/****************************
 * INITIALIZATION FUNCTIONS *
 ****************************/

/*
 * Open a window using width and height specified in main.cpp.
 */
int open_window()
{
    Uint32 window_flags = SDL_WINDOW_ALLOW_HIGHDPI;

    WINDOW = SDL_CreateWindow("synth",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              window_flags);

    if(WINDOW == NULL) {
        std::cout << RED_STDOUT << "Could not create window: "
             << SDL_GetError() << DEFAULT_STDOUT << std::endl;
        return 0;
    }

    std::cout << "Window opened." << std::endl;

    return 1;
}

/*
 * Create a renderer for the window created before.
 */
int create_renderer()
{
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE |
                          SDL_RENDERER_PRESENTVSYNC;

    RENDERER = SDL_CreateRenderer(WINDOW, -1, render_flags);

    if(WINDOW == NULL) {
        std::cout << RED_STDOUT << "Could not create renderer: "
             << SDL_GetError() << DEFAULT_STDOUT << std::endl;
        return 0;
    }

    std::cout << "Renderer created." << std::endl;

    return 1;
}

/*
 * Create a texture to render to so that the GPU will be used
 * isntead of the CPU when rendering to a surface.
 */
int create_texture()
{
    TEXTURE = SDL_CreateTexture(RENDERER, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                WINDOW_WIDTH, WINDOW_HEIGHT);

    if(TEXTURE == NULL) {
        std::cout << RED_STDOUT << "Could not create texture: "
             << SDL_GetError() << DEFAULT_STDOUT << std::endl;
        return 0;
    }

    std::cout << "Texture created." << std::endl;

    SDL_SetRenderTarget(RENDERER, TEXTURE);

    return 1;
}

/*
 * Load fonts into external variables so that all
 * modules can render text.
 */
int load_fonts()
{
    FONT_REGULAR = TTF_OpenFont("fonts/SourceCodePro-Regular.ttf", 11);
    FONT_SMALL = TTF_OpenFont("fonts/SourceCodePro-Regular.ttf", 10);
    FONT_BOLD = TTF_OpenFont("fonts/SourceCodePro-Bold.ttf", 11);

    if(!FONT_REGULAR || !FONT_BOLD)
    {
        std::cout << RED_STDOUT << "Could not open one of the TTF fonts: "
             << TTF_GetError() << DEFAULT_STDOUT << std::endl;
        return 0;
    }

    std::cout << "Fonts loaded." << std::endl;

    return 1;
}

/******************************
 * IMAGE PROCESSING FUNCTIONS *
 ******************************/

/*
 * Determine where in the window each of the modules and their
 * respective graphics objects are meant to be rendered.
 */
void update_graphics_objects()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        MODULES[i]->update_graphics_object_locations();
    }

    MODULES_CHANGED = false;
}

/*
 * Return the module (if any) that is currently
 * being hovered over by the mouse.
 */
Module *hovering_over()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(!MODULES[i]->graphics_objects[0]->was_clicked())
            return MODULES[i];

    return NULL;
}

/*
 * Create the very first sub page on every page, the utilities sub page.
 * This sub page contains the buttons for adding new modules, switching
 * pages, and saving or loading the current patch.
 */
void initialize_utilities_sub_page(std::vector<Graphics_Object *> *sub_page_graphics_objects,
                                   std::vector<Page *> *sub_pages, Page *current_sub_page)
{

    std::vector<std::string> names, texts;
    std::vector<SDL_Rect> locations;
    std::vector<SDL_Color *> colors, text_colors;
    std::vector<Module *> parents;
    std::vector<TTF_Font *> fonts;

    names = std::vector<std::string>();
    locations = std::vector<SDL_Rect>();
    colors = std::vector<SDL_Color *>();
    text_colors = std::vector<SDL_Color *>();
    texts = std::vector<std::string>();
    fonts = std::vector<TTF_Font *>();
    parents = std::vector<Module *>();

    std::vector<Graphics_Object *> tmp_graphics_objects;

    // Create the background and add it to the list of graphics
    // objects
    Rect *background = new Rect("background (rect)", WINDOW_RECT, &BLACK, NULL);
    sub_page_graphics_objects->push_back(background);

    names = {"add oscillator (button)", "add multiplier (button)",
             "add mixer (button)", "previous page (button)", "next page (button)"};
    locations = {{2, WINDOW_HEIGHT - MENU_HEIGHT - 17, 101, 15}, {105, WINDOW_HEIGHT - MENU_HEIGHT - 17, 101, 15},
                 {208, WINDOW_HEIGHT - MENU_HEIGHT - 17, 66, 15}, {WINDOW_WIDTH - 163, WINDOW_HEIGHT - MENU_HEIGHT - 17, 92, 15},
                 {WINDOW_WIDTH - 163 + 94, WINDOW_HEIGHT - MENU_HEIGHT - 17, 66, 15}};
    colors = std::vector<SDL_Color *>(5, &MODULES[0]->color);
    text_colors = std::vector<SDL_Color *>(5, &MODULES[0]->text_color);
    texts = {"ADD OSCILLATOR", "ADD MULTIPLIER", "ADD MIXER", "PREVIOUS PAGE", "NEXT PAGE"};
    parents = std::vector<Module *>(5, NULL);

    tmp_graphics_objects = initialize_button_objects(names, locations, colors, text_colors, texts, parents);
    sub_page_graphics_objects->insert(sub_page_graphics_objects->end(), tmp_graphics_objects.begin(),
                                      tmp_graphics_objects.end());

    names = {"save patch (text box)", "load patch (text box)"};
    locations = {{2, WINDOW_HEIGHT - 17, (WINDOW_WIDTH / 2) - 2, 15},
                 {(WINDOW_WIDTH / 2) + 2, WINDOW_HEIGHT - 17, (WINDOW_WIDTH / 2) - 4, 15}};
    colors = std::vector<SDL_Color *>(2, &MODULES[0]->color);
    text_colors = std::vector<SDL_Color *>(2, &MODULES[0]->text_color);
    texts = {"enter a name here to save your patch", "enter a patch name to load here"};
    fonts = std::vector<TTF_Font *>(2, FONT_REGULAR);
    parents = std::vector<Module *>(2, NULL);

    tmp_graphics_objects = initialize_text_box_objects(names, locations, colors, text_colors, texts, fonts, parents);
    sub_page_graphics_objects->insert(sub_page_graphics_objects->end(), tmp_graphics_objects.begin(),
                                      tmp_graphics_objects.end());

    // Create the sub page and add it to the list of sub pages
    // for the current page
    current_sub_page = new Page("utilities & background (page)", WINDOW_RECT, &BLACK,
                                      sub_page_graphics_objects, NULL);
    sub_pages->push_back(current_sub_page);
}

/*
 * Determine what graphics objects go on what pages. Create those pages.
 */
void calculate_pages()
{
    destroy_pages();
    PAGES.clear();

    // Variables for the current sub page, its sub pages,
    // its sub pages graphics objects, and the current page
    // (no page has any graphics objects, only sub pages
    // contain graphics objects)
    std::string current_sub_page_name;
    Page *current_sub_page = NULL;
    std::vector<Page *> *sub_pages = new std::vector<Page *>();
    std::vector<Graphics_Object *> *sub_page_graphics_objects = new std::vector<Graphics_Object *>();
    std::string current_page_name;
    Page *current_page = NULL;

    // Create the first sub page, which will contain important
    // buttons for special functions
    initialize_utilities_sub_page(sub_page_graphics_objects, sub_pages, current_sub_page);

    // Reset the sub page graphics objects
    delete sub_page_graphics_objects;
    sub_page_graphics_objects = new std::vector<Graphics_Object *>();

    // For each module
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        // Add each of its graphics objects to the current sub page
        for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
        {
            sub_page_graphics_objects->push_back(MODULES[i]->graphics_objects[j]);
            MODULES[i]->graphics_objects[j]->updated = true;
        }

        // Create the sub page using the created vector of graphics objects,
        // add it to the list of sub pages
        current_sub_page = new Page(MODULES[i]->name + " (page)",
                                    MODULES[i]->graphics_objects[0]->location, &BLACK,
                                    sub_page_graphics_objects, NULL);
        sub_pages->push_back(current_sub_page);

        // Reset the sub page graphics objects
        delete sub_page_graphics_objects;
        sub_page_graphics_objects = new std::vector<Graphics_Object *>();

        // If this is the last sub page in the current page, or
        // if there are no more modules to take into consideration
        if(i % (MODULES_PER_COLUMN * MODULES_PER_ROW) ==
           (unsigned) (MODULES_PER_COLUMN * MODULES_PER_ROW) - 1 ||
           i == MODULES.size() - 1)
        {
            // Create the page using the created vector of sub pages, add it
            // to the global list of pages
            current_page = new Page(std::to_string(i / (MODULES_PER_COLUMN * MODULES_PER_ROW)) + " (page)",
                                    WINDOW_RECT, &BLACK,
                                    NULL, sub_pages);
            PAGES.push_back(current_page);

            // Delete the vector of sub pages and sub page graphics objects,
            // only re-initialize them and restart the process of calculating a new page
            // if there are still modules left to take into consideration
            delete sub_pages;
            delete sub_page_graphics_objects;
            if(i != MODULES.size() - 1)
            {
                sub_pages = new std::vector<Page *>();
                sub_page_graphics_objects = new std::vector<Graphics_Object *>();
                initialize_utilities_sub_page(sub_page_graphics_objects, sub_pages, current_sub_page);
                delete sub_page_graphics_objects;
                sub_page_graphics_objects = new std::vector<Graphics_Object *>();
            }
        }
    }
}

/*
 * Render the GUI in the window by using the
 * member functions in each module that render
 * their representations
 */
void draw_surface()
{
    // If the set of modules has changed, calculate the
    // graphics objects, then calculate the pages
    if(MODULES_CHANGED)
    {
        update_graphics_objects();
        calculate_pages();
    }

    // Clear the renderer
    SDL_RenderClear(RENDERER);

    // Copy audio data into waveform objects so that they will render without hiccups
    SDL_LockAudio();
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
            if(MODULES[i]->graphics_objects[j]->type == WAVEFORM)
                ((Waveform *) MODULES[i]->graphics_objects[j])->copy_buffer();
    SDL_UnlockAudio();

    // Render graphics objects for the current page
    PAGES[CURRENT_PAGE]->render();

    // Copy the contents of the texture into the renderer, then present the renderer
    SDL_SetRenderTarget(RENDERER, NULL);
    SDL_RenderCopy(RENDERER, TEXTURE, NULL, NULL);
    SDL_RenderPresent(RENDERER);
    SDL_SetRenderTarget(RENDERER, TEXTURE);
}
