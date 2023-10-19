#include "ui.hpp"

// SDL Window and text surface vars
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font18;
SDL_Surface *surface;
SDL_Texture *texture;
SDL_Surface *surface_T;
SDL_Texture *texture_T;

// Colors
const SDL_Color YELLOW_COLOR = {192, 192, 0, 255};
const SDL_Color BLACK_COLOR = {0, 0, 0, 255};
const SDL_Color WHITE_COLOR = {255, 255, 255, 255};
const SDL_Color BACK_COLOR = {0, 32, 31, 255};
const SDL_Color GRID_COLOR = {75, 75, 75, 255};
const SDL_Color GREEN_COLOR = {63, 96, 96, 255};
const SDL_Color ORANGE_COLOR = {192, 128, 0, 255};

int WINDOW_WIDTH = 1200;
int WINDOW_HEIGHT = 620;

// SAMPLES Data Buffer and vars
uint8_t SAMPLES_BUFFER[SAMPLES_BUFFER_DEPTH] = {0};
uint16_t SAMPLE_INDEX = 0;
volatile uint8_t finishedConversion = 0; // this lets us know when we're done capturing data
volatile uint16_t samplespersecond = 0;

// LOG Buffers and vars
bool LOG = false;
uint8_t LOG_BUFFER[LOG_BUFFER_DEPTH] = {0};
uint16_t LOG_INDEX = 0;



ui::ui()
{
}

ui::~ui()
{
}

void ui::key_handler(SDL_Event &event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_d:

        if (focus_box.w > 60 && ((focus_box.x > destRect.x + 5) || focus_box.x == privew_box.x + 7))
        {
            focus_box.w--;

            focus_box_T.x -= (focus_box.w - 307) % 2;
        }
        if (tdiv >= 1000)
            tdiv += 1000;
        else if (tdiv >= 100)
            tdiv += 100;
        else if (tdiv >= 10)
            tdiv += 10;
        break;

    case SDLK_q:

        if (focus_box.w < privew_box.w - 10 && (((focus_box.x + focus_box.w) < (destRect.x + destRect.w) - 5) || focus_box.x == privew_box.x + 7))
        {
            focus_box.w++;
            focus_box_T.x += (focus_box.w - 307) % 2;
        }

        if (tdiv > 20)
        {
            if (tdiv > 1000)
                tdiv -= 1000;
            else if (tdiv > 100)
                tdiv -= 100;
            else if (tdiv > 10)
                tdiv -= 10;
        }
        break;

    case SDLK_z:
        if (vdiv < 2.5)
            vdiv += 0.5;
        break;
    case SDLK_s:
        if (vdiv > 0.5)
            vdiv -= 0.5;
        break;

    case SDLK_UP:

        break;

    case SDLK_DOWN:

        break;

    case SDLK_RIGHT:
        if ((focus_box.x + focus_box.w) < (destRect.x + destRect.w) - 5)
        {
            focus_box.x++;
            focus_box_T.x++;
        }
        break;

    case SDLK_LEFT:
        if (focus_box.x > destRect.x + 5)
        {
            focus_box.x--;
            focus_box_T.x--;
        }
        break;

    case SDLK_t:
        if (trig == RISING)
            trig = FALLING;
        else
            trig = RISING;
        break;

    case SDLK_SPACE:
        if (Mode)
            Mode = 0; // Runing
        else
            Mode = 1; // Stop
        break;
        // Add more cases for other keys
    }
}
