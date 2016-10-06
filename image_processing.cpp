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
#include "SDL.h"
#include "SDL_ttf.h"

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

    if(TEXTURE == NULL)
    {
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
    FONT_REGULAR = TTF_OpenFont("../fonts/SourceCodePro-Regular.ttf", 11);
    FONT_SMALL = TTF_OpenFont("../fonts/SourceCodePro-Regular.ttf", 10);
    FONT_BOLD = TTF_OpenFont("../fonts/SourceCodePro-Bold.ttf", 11);

    if(!FONT_REGULAR || !FONT_SMALL || !FONT_BOLD)
    {
        std::cout << RED_STDOUT << "Could not open one of the TTF fonts: "
             << TTF_GetError() << DEFAULT_STDOUT << std::endl;
        return 0;
    }

    std::cout << "Fonts loaded." << std::endl;

    return 1;
}

/*
 * Create the very first sub page on every page, the utilities sub page.
 * This sub page contains the buttons for adding new modules, switching
 * pages, and saving or loading the current patch.
 */
void initialize_utilities_page()
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
    std::vector<Graphics_Object *> graphics_objects;

    int y = 2;
    locations.push_back({y, WINDOW_HEIGHT - MENU_HEIGHT - 15, 60, 15});
    y += 62;
    locations.push_back({y, WINDOW_HEIGHT - MENU_HEIGHT - 15, 67, 15});
    y += 69;
    locations.push_back({y, WINDOW_HEIGHT - MENU_HEIGHT - 15, 74, 15});
    y += 76;
    locations.push_back({y, WINDOW_HEIGHT - MENU_HEIGHT - 15, 66, 15});
    y += 68;
    locations.push_back({y, WINDOW_HEIGHT - MENU_HEIGHT - 15, 101, 15});
    y += 103;
    locations.push_back({y, WINDOW_HEIGHT - MENU_HEIGHT - 15, 67, 15});
    y += 69;
    locations.push_back({y, WINDOW_HEIGHT - MENU_HEIGHT - 15, 102, 15});
    y += 104;
    locations.push_back({y, WINDOW_HEIGHT - MENU_HEIGHT - 15, 53, 15});
    locations.push_back({WINDOW_WIDTH - 162, WINDOW_HEIGHT - MENU_HEIGHT - 15, 93, 15});
    locations.push_back({WINDOW_WIDTH - 162 + 95, WINDOW_HEIGHT - MENU_HEIGHT - 15, 65, 15});

    // Create the background and add it to the list of graphics
    // objects
    Rect *background = new Rect("background (rect)", WINDOW_RECT, &BLACK, NULL);
    graphics_objects.push_back(background);

    names = {"add adsr (button)", "add delay (button)", "add filter (button)",
             "add mixer (button)", "add multiplier (button)", "add noise (button)",
             "add oscillator (button)", "add sah (button)", "previous page (button)",
             "next page (button)"};

    colors = std::vector<SDL_Color *>(10, &WHITE);
    text_colors = std::vector<SDL_Color *>(10, &BLACK);
    texts = {"ADD ADSR", "ADD DELAY", "ADD FILTER", "ADD MIXER", "ADD MULTIPLIER",
             "ADD NOISE", "ADD OSCILLATOR", "ADD SAH", "PREVIOUS PAGE", "NEXT PAGE"};
    parents = std::vector<Module *>(10, NULL);

    tmp_graphics_objects = initialize_button_objects(names, locations, colors, text_colors, texts, parents);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(),
                                      tmp_graphics_objects.end());

    names = {"save patch (text box)", "load patch (text box)"};
    locations = {{2, WINDOW_HEIGHT - 17, (WINDOW_WIDTH / 2) - 2, 15},
               {(WINDOW_WIDTH / 2) + 2, WINDOW_HEIGHT - 17, (WINDOW_WIDTH / 2) - 3, 15}};
    colors = std::vector<SDL_Color *>(2, &WHITE);
    text_colors = std::vector<SDL_Color *>(2, &BLACK);
    texts = {"enter a name here to save your patch", "enter a patch name to load here"};
    fonts = std::vector<TTF_Font *>(2, FONT_REGULAR);
    parents = std::vector<Module *>(2, NULL);

    tmp_graphics_objects = initialize_text_box_objects(names, locations, colors, text_colors, texts, fonts, parents);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(),
                            tmp_graphics_objects.end());

    // Create the sub page and add it to the list of sub pages
    // for the current page
    UTILITIES_PAGE = new Page("utilities & background (page)", WINDOW_RECT, &BLACK,
                              &graphics_objects);

    std::cout << "Utilities page initialized." << std::endl;
}

/*
 * Set the graphics objects on the utilities page to the same color as
 * the output module.
 */
void prettify_utilities_page()
{
    for(unsigned int i = 0; i < UTILITIES_PAGE->graphics_objects.size(); i ++)
    {
        if(UTILITIES_PAGE->graphics_objects[i]->graphics_object_type == BUTTON)
            ((Button *) UTILITIES_PAGE->graphics_objects[i])->set_colors(&MODULES[0]->primary_module_color, &MODULES[0]->secondary_module_color);
        else if(UTILITIES_PAGE->graphics_objects[i]->graphics_object_type == TEXT_BOX)
            ((Text_Box *) UTILITIES_PAGE->graphics_objects[i])->set_colors(&MODULES[0]->primary_module_color, &MODULES[0]->secondary_module_color);
    }
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
        if(MODULES[i] != NULL)
            MODULES[i]->update_graphics_object_locations();

    MODULES_CHANGED = false;
}

/*
 * Return the module (if any) that is currently
 * being hovered over by the mouse.
 */
Module *hovering_over()
{
    Module *module = NULL;

    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(MODULES[i] != NULL)
            if(MODULES[i]->was_clicked())
                module = MODULES[i];

    return module;
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
    // (no top-level page has any graphics objects, only their sub pages
    // contain graphics objects)
    Page *current_sub_page = NULL;
    std::vector<Graphics_Object *> graphics_objects;
    std::vector<Graphics_Object *> sub_page_graphics_objects;
    std::string current_page_name;
    Page *current_page = NULL;

    // For each module
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(graphics_objects.size() == 0)
            graphics_objects.push_back((Graphics_Object *) UTILITIES_PAGE);

        if(MODULES[i] != NULL)
        {
            // Mark each graphics objects in the module as updated
            for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
                MODULES[i]->graphics_objects[j]->updated = true;

            // Add the module to the list of sub page graphics objects
            graphics_objects.push_back((Graphics_Object *) MODULES[i]);
        }

        // If this is the last sub page in the current page, or
        // if there are no more modules to take into consideration
        if(i % (MODULES_PER_COLUMN * MODULES_PER_ROW) ==
           (unsigned) (MODULES_PER_COLUMN * MODULES_PER_ROW) - 1 ||
           i == MODULES.size() - 1)
        {
            // Create the page using the created vector of sub pages, add it
            // to the global list of pages
            current_page = new Page(std::to_string(i / (MODULES_PER_COLUMN * MODULES_PER_ROW)) + " (page)",
                                    WINDOW_RECT, &BLACK, &graphics_objects);
            PAGES.push_back(current_page);
            current_page = NULL;
            graphics_objects.clear();
            sub_page_graphics_objects.clear();
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
        if(MODULES[i] != NULL)
            for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
                if(MODULES[i]->graphics_objects[j]->graphics_object_type == WAVEFORM)
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
