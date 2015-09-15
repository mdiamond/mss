/*
 * Matthew Diamond 2015
 * Member functions for the Oscillator class.
 */

// Include header file
#include "Oscillator.hpp"

using namespace std;

/*******************************
 * OSCILLATOR MEMBER FUNCTIONS *
 *******************************/

/*
 * Constructor
 */
Oscillator::Oscillator(string *_name)
{
    name = *_name;
    type = OSCILLATOR;

    color.r = 0;
    color.g = 255;
    color.b = 0;
    color.a = 255;

    this->audio.frequency = 0;
    this->audio.phase = 0;
    this->audio.amplitude = 1;
    this->audio.fm_on = 0;
    this->audio.modulation_index = 1;
    this->audio.output = new vector<float>(BUFFER_SIZE, 0);

    modulator = NULL;
}

/*
 * Dummy function
 */
Oscillator::~Oscillator()
{

}

/*
 * Process all dependencies, then
 * fill the output buffer with a waveform given
 * the data contained within this class and the
 * audio device information.
 */
void Oscillator::process()
{
    float frequency_shift = 0;
    // Check for any dependencies for frequency modulation
    process_depends();
    // Get oscillator audio info struct
    struct Oscillator_Data *audio_data = &(this->audio);
    // Calculate an amplitude for each sample
    for(int i = 0; i < BUFFER_SIZE; i ++)
    {
        // Calculate and store the current samples amplitude
        // based on phase
        (*(audio_data->output))[i] = audio_data->amplitude * sin(audio_data->phase);
        // Calculate phase for the next sample
        if(audio_data->fm_on)
        {
            frequency_shift = (((modulator->audio).frequency) * audio_data->modulation_index) * (*((modulator->audio).output))[i];
            audio_data->phase += (2 * M_PI * (audio_data->frequency + frequency_shift) / SAMPLE_RATE);
        }
        else
            audio_data->phase += (2 * M_PI * audio_data->frequency / SAMPLE_RATE);
        if(audio_data->phase > (2 * M_PI))
            audio_data->phase -= (2 * M_PI);
    }
}

/*
 * Copy all data from the audio data struct to the
 * graphics data struct to make it available for
 * rendering.
 */
void Oscillator::copy_graphics_data()
{
    this->graphics = this->audio;
    this->graphics.output = new vector<float>(*(this->audio.output));
}

/*
 * This function uses the renderer to create a representation
 * of this module in the window.
 */
void Oscillator::render()
{
    render_waveform();
    render_text();
}

void Oscillator::render_waveform()
{
    int waveform_height = 50;
    SDL_Point zero = {0, 0};
    vector<SDL_Point> points((MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 11, zero);
    int starting_x = upper_left.x + MODULE_BORDER_WIDTH + 5;
    int starting_y = upper_left.y + MODULE_BORDER_WIDTH + 15;
    int waveform_width = (MODULE_WIDTH - (MODULE_BORDER_WIDTH * 2)) - 11;
    SDL_Rect rect = {starting_x, starting_y, waveform_width, waveform_height};
    int index = 0;
    for(unsigned int i = this->graphics.output->size() - waveform_width; i < this->graphics.output->size(); i ++)
    {
        points[index].x = starting_x + index;
        points[index].y = (starting_y + waveform_height / 2) +
                          ((*(this->graphics.output))[this->graphics.output->size() - waveform_width + index]) * (waveform_height / 2);
        index ++;
    }
    SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
    SDL_RenderFillRect(RENDERER, &rect);
    SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255);
    SDL_RenderDrawLines(RENDERER, &points[0], points.size());
}

void Oscillator::render_text()
{
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(FONT_BOLD, name.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(RENDERER, surface);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    int x = upper_left.x + 5;
    int y = upper_left.y + 5;
    SDL_Rect dstrect = {x, y, width, height};
    SDL_RenderCopy(RENDERER, texture, NULL, &dstrect);
}

