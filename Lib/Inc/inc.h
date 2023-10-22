

#include <iostream>
#include <vector>
#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "usb.hpp"

// GLOBAL FLAGS
#define MAX_ADC 255.0
#define HW_OFFSET 1.65
#define ADC_VCC 3.3

#define RECIEVE_LENGTH 4096*5
#define SAMPLES_BUFFER_DEPTH 16400 * 2 // 16.4 kbps
#define LOG_BUFFER_DEPTH 16400 * 10    // 10 sec of loging with 16.4 kbps

#define samplingRate 2.27273e6

#define RECIEVE_DEBUG 1

// Colors
extern const SDL_Color YELLOW_COLOR;
extern const SDL_Color BLACK_COLOR;
extern const SDL_Color WHITE_COLOR;
extern const SDL_Color BACK_COLOR;
extern const SDL_Color GRID_COLOR;
extern const SDL_Color GREEN_COLOR;
extern const SDL_Color ORANGE_COLOR;

// SDL Window and text surface vars
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font *font18;


extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

// SAMPLES Data Buffer and vars
extern uint8_t SAMPLES_BUFFER[SAMPLES_BUFFER_DEPTH];
extern uint16_t SAMPLE_INDEX;
extern volatile uint8_t finishedConversion; // this lets us know when we're done capturing data
extern volatile uint16_t samplespersecond;
// LOG Buffers and vars
extern bool LOG;
extern uint8_t LOG_BUFFER[LOG_BUFFER_DEPTH];
extern uint16_t LOG_INDEX;

// Used for testing
extern uint8_t sine_wave[256];