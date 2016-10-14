/*
 * Matthew Diamond 2016
 * All initialization related functions.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <vector>

// Included SDL components
#include "SDL.h"
#include "SDL_ttf.h"

// Included files
#include "graphics_object_utils.hpp"
#include "initialize.hpp"
#include "main.hpp"
#include "populate_wavetables.hpp"
#include "signal_processing.hpp"

// Included modules classes
#include "Modules/Output.hpp"

// Included "other" classes
#include "Color_Modifier.hpp"

/********************************
 * MAIN INITIALIZATION FUNCTION *
 ********************************/

/*
 * Initialize all of the stuff that needs to be initialized
 * before audio can be processed:
 *   - initialize SDL
 *   - open the audio device
 *   - initialize the utilities sub page
 *   - open a window
 *   - create a renderer
 *   - initialize SDL_ttf
 *   - open ttf fonts
 *   - initialize the synthesizer output module
 *   - start audio to begin requesting buffers from the audio
 *     callback function
 * Return true if all of this succeeds, false if anything fails.
 */
bool initialize()
{
    // system("clear");

    // Initialize SDL with the video and audio subsystems
    if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1))
    {
        std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "SDL initialized." << std::endl;

    // Initialize audio device
    if(!open_audio_device())
    {
        return false;
    }

    // Initialize truetype
    if(TTF_Init() == -1)
    {
        std::cout << "Could not initialize TTF: " << TTF_GetError() << std::endl;
        return false;
    }

    // Open ttf fonts
    if(!load_fonts())
    {
        return false;
    }

    // Open a window
    if(!open_window())
    {
        return false;
    }

    // Create a renderer
    if(!create_renderer())
    {
        return false;
    }

    // Initialize utilities sub page
    initialize_utilities_page();

    // Create a texture to render to
    if(!create_texture())
    {
        return false;
    }

    // Populate wavetables
    populate_wavetables();

    // Initialize the output module
    initialize_output();

    // Set colors on the utilities page graphics objects
    prettify_utilities_page();

    // Unpause the audio
    SDL_PauseAudio(0);
    std::cout << "Audio unpaused." << std::endl;

    return true;
}

/********************************************
 * GRAPHICS RELATED INITIALIZATION FUNCTION *
 ********************************************/

/*
 * Open a window using width and height specified in main.cpp.
 */
int open_window()
{
    Uint32 window_flags = SDL_WINDOW_ALLOW_HIGHDPI;

    WINDOW = SDL_CreateWindow("mss",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              window_flags);

    if(WINDOW == NULL)
    {
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

    if(WINDOW == NULL)
    {
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
    TEXTURE = SDL_CreateTexture(RENDERER, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET,
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
    FONT_REGULAR = TTF_OpenFont("../fonts/visitor1.ttf", 10);

    if(!FONT_REGULAR)
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
             "next page (button)"
            };

    colors = std::vector<SDL_Color *>(10, &WHITE);
    text_colors = std::vector<SDL_Color *>(10, &BLACK);
    texts = {"ADD ADSR", "ADD DELAY", "ADD FILTER", "ADD MIXER", "ADD MULTIPLIER",
             "ADD NOISE", "ADD OSCILLATOR", "ADD SAH", "PREVIOUS PAGE", "NEXT PAGE"
            };
    parents = std::vector<Module *>(10, NULL);

    tmp_graphics_objects = initialize_button_objects(names, locations, colors,
                                                     text_colors, texts, parents);
    graphics_objects.insert(graphics_objects.end(), tmp_graphics_objects.begin(),
                            tmp_graphics_objects.end());

    names = {"save patch (text box)", "load patch (text box)"};
    locations = {{2, WINDOW_HEIGHT - 17, (WINDOW_WIDTH / 2) - 2, 15},
        {
            (WINDOW_WIDTH / 2) + 2, WINDOW_HEIGHT - 17,
            (WINDOW_WIDTH / 2) - 3, 15
        }
    };
    colors = std::vector<SDL_Color *>(2, &WHITE);
    text_colors = std::vector<SDL_Color *>(2, &BLACK);
    texts = {"enter a name here to save your patch", "enter a patch name to load here"};
    fonts = std::vector<TTF_Font *>(2, FONT_REGULAR);
    parents = std::vector<Module *>(2, NULL);

    tmp_graphics_objects = initialize_text_box_objects(names, locations, colors,
                                                       text_colors, texts, fonts, parents);
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
        if(UTILITIES_PAGE->graphics_objects[i]->graphics_object_type
           == Graphics_Object::BUTTON)
        {
            ((Button *) UTILITIES_PAGE->graphics_objects[i])->set_colors(
                &MODULES[0]->primary_module_color, &MODULES[0]->secondary_module_color);
        }
        else if(UTILITIES_PAGE->graphics_objects[i]->graphics_object_type
                == Graphics_Object::TEXT_BOX)
        {
            ((Text_Box *) UTILITIES_PAGE->graphics_objects[i])->set_colors(
                &MODULES[0]->primary_module_color, &MODULES[0]->secondary_module_color);
        }
    }
}

/*****************************************
 * AUDIO RELATED INITIALIZATION FUNCTION *
 *****************************************/

/*
 * Open the audio device with a simple configuration.
 */
int open_audio_device()
{
    SDL_AudioSpec wanted, obtained;

    wanted.freq = SAMPLE_RATE;
    wanted.format = AUDIO_F32SYS;
    wanted.channels = 2;
    wanted.samples = 512;
    wanted.callback = audio_callback;
    wanted.userdata = NULL;

    if(SDL_OpenAudio(&wanted, &obtained) == -1)
    {
        std::cout << RED_STDOUT << "Could not open the audio device: "
                  << SDL_GetError() << DEFAULT_STDOUT << std::endl;
        return 0;
    }

    std::cout << "Audio device opened." << std::endl;
    std::cout << "Audio details:" << std::endl;
    std::cout << "    Sample rate: " << obtained.freq << std::endl;
    std::cout << "    Format: " << obtained.format << std::endl;
    std::cout << "    Channels: " << obtained.channels << std::endl;
    std::cout << "    Buffer size in samples: " << obtained.samples << std::endl;
    std::cout << "    Buffer size in bytes: " << obtained.size << std::endl;

    BUFFER_SIZE = obtained.samples;

    // Return success
    return 1;
}

/*
 * Initialize the one and only output module.
 */
void initialize_output()
{
    // Create the output module
    Output *output = new Output();
    output->initialize_graphics_objects();
    MODULES.push_back(output);

    std::cout << "Output initialized." << std::endl;
}

