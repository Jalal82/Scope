#include "ui.hpp"

// SDL Window and text surface vars
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font18;

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

// Used for testing
uint8_t sine_wave[256] = {
    0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
    0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
    0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
    0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
    0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
    0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
    0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
    0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
    0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
    0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
    0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
    0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
    0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
    0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
    0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
    0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
    0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
    0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
    0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
    0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
    0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
    0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
    0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
    0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
    0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
    0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
    0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
    0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
    0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
    0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D};

ui::ui()
{
    // Add offcet at startup to lazy to calculate it lameo §_§
    grid_border_box.x++;
    grid_border_box.y++;
    grid_border_box.w++;
    grid_border_box.h++;

    //    sampRate = (16000 * 1000) / tdiv;
    // sampPer = tdiv / 16.0;
    // setTimerFreq(sampRate);
}

ui::~ui()
{
 
}

void ui::key_handler(SDL_Event &event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_d:
        if ((focus_box.x + focus_box.w) < (destRect.x + destRect.w) - 5)
        {
            focus_box.x++;
            focus_box_T.x++;
        }
        move += 10;

        break;

    case SDLK_q:
        if (focus_box.x > destRect.x + 5)
        {
            focus_box.x--;
            focus_box_T.x--;
        }
        if (move > 10)
            move -= 10;
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
        if (vdiv > 0.25)
            vdiv -= 0.25;
        break;

    case SDLK_DOWN:

        if (vdiv < 4)
            vdiv += 0.25;
        break;

    case SDLK_RIGHT:
        if (focus_box.w < privew_box.w - 10 && (((focus_box.x + focus_box.w) < (destRect.x + destRect.w) - 5) || focus_box.x == privew_box.x + 7))
        {
            focus_box.w++;
            T_Offect++;
            focus_box_T.x += T_Offect % 2;
        }
       
         if (tdiv >= 1000)
            tdiv += 1000;
        else if (tdiv >= 100)
            tdiv += 100;
        else if (tdiv >= 10)
            tdiv += 10;
     else if (tdiv >= 1)
            tdiv += 1;

        sampRate = ((cell_size) * 1000 * 1000) / tdiv;
        sampPer = tdiv / (float)cell_size;
        setTimerFreq(sampRate);
        break;

    case SDLK_LEFT:

        if (focus_box.w > 60 && ((focus_box.x > destRect.x + 5) || focus_box.x == privew_box.x + 7))
        {
            focus_box.w--;
            T_Offect--;
            focus_box_T.x -= T_Offect % 2;
        }
        if (tdiv > 10000)
            tdiv -= 10000;
        else if (tdiv > 1000)
            tdiv -= 1000;
        else if (tdiv > 100)
            tdiv -= 100;
        else if (tdiv > 10)
            tdiv -= 10;
          

        sampRate = ((cell_size) * 1000 * 1000) / tdiv;
        sampPer = tdiv / (float)cell_size;

        setTimerFreq(sampRate);

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

    case SDLK_8:
        if (trigVoltage < (Y_CELLS / 2 - 1) * vdiv)
            trigVoltage += 0.1;
        break;


     case SDLK_h:
   
        break;
    case SDLK_2:
        if (trigVoltage > 0)
            trigVoltage -= 0.1;
        break;
        // Add more cases for other keys
    }

    // Update waveform preview aria
    waveform_preview();
}

void ui::setTimerFreq(uint32_t Timer_freq)
{
    // send data over uart;
    std::cout << "[cmd]: Set Device Timer Frequency = ";
    std::cout << Timer_freq<<std::endl;
    cmd = 'T'+std::to_string(Timer_freq)+";";
    send_flag = 1;
    
}

void ui::waveform_preview()
{
    // local vars for drawing
    uint8_t point, old_point;
    uint16_t x1, y1, x2, y2;

    // Draw Wave preview black aria
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &privew_box);

    // Draw focus box
    SDL_SetRenderDrawColor(renderer, 100, 50, 60, 255);
    SDL_RenderDrawRect(renderer, &focus_box);
    surface_T = TTF_RenderText_Blended(font18, "T", ORANGE_COLOR);
    texture_T = SDL_CreateTextureFromSurface(renderer, surface_T);
    SDL_RenderCopy(renderer, texture_T, nullptr, &focus_box_T);

    // Draw waveform preview sin wave
    for (size_t j = 0; j <= 21; j++)
    {
        for (uint8_t i = 0; i <= 240; i += 15)
        {

            point = sine_wave[i];

            if (i > 0)
            {
                float timeStep = 0.063;
                float voltageStep = 0.03;
                uint8_t zero_level = 15;
                x1 = (i + 255 * j) * timeStep;             // Adjust the position based on your requirements
                y1 = (old_point)*voltageStep + zero_level; // Adjust the amplitude based on your requirements
                x2 = (i + 1 + 255 * j) * timeStep;         // Adjust the position based on your requirements
                y2 = (point)*voltageStep + zero_level;     // Adjust the amplitude based on your requirements
                if ((x1 + 306) > focus_box.x && (x2 + 306) < (focus_box.x + focus_box.w))
                {
                    thickLineRGBA(renderer, x1 + 306, y1 + 2, x2 + 306, y2 + 2, 1, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, 255);
                }
                else
                {
                    thickLineRGBA(renderer, x1 + 306, y1 + 2, x2 + 306, y2 + 2, 1, 115, 115, 115, 255);
                }
            }
            old_point = point;
        }
    }

    // Render text
    surface = TTF_RenderText_Blended(font18, "[                                                   ]", GREEN_COLOR);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    // Mode Surface
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderFillRect(renderer, &ModeRect);
    roundedBoxRGBA(renderer, ModeRect.x, ModeRect.y, ModeRect.x + ModeRect.w, ModeRect.y + ModeRect.h, 3, 50, 140, 50, 255);
    SDL_SetRenderDrawColor(renderer, 1, 233, 0, 255);
    surface = TTF_RenderText_Blended(font18, (Mode == 1) ? "    Runing    " : "      Stop      ", {1, 233, 0, 255});
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, nullptr, &ModeRect);

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    SDL_DestroyTexture(texture_T);
    SDL_FreeSurface(surface_T);

    // Render Trigger aria
    SDL_SetRenderDrawColor(renderer, 100, 60, 0, 255);
    // SDL_RenderFillRect(renderer, &trigger_box);
    roundedBoxRGBA(renderer, trigger_box.x, trigger_box.y, trigger_box.x + trigger_box.w, trigger_box.y + trigger_box.h, 5, 100, 60, 0, 255);

    char trig_txt[50];
    if (trigVoltage < 1.0)
        if (trig == RISING)
            snprintf(trig_txt, 50, "   RISING        %dmV   ", int(trigVoltage * 1000));
        else
            snprintf(trig_txt, 50, "   FALLING     %dmV   ", int(trigVoltage * 1000));

    else
    {
        if (trig == RISING)
            snprintf(trig_txt, 50, "   RISING        %.2fV  ", trigVoltage);
        else
            snprintf(trig_txt, 50, "   FALLING     %.2fV  ", trigVoltage);
    }

    surface = TTF_RenderText_Blended(font18, trig_txt, YELLOW_COLOR);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, nullptr, &trigger_box);

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    // Render Voltage aria
    SDL_SetRenderDrawColor(renderer, 100, 60, 0, 255);
    // SDL_RenderFillRect(renderer, &trigger_box);
    roundedBoxRGBA(renderer, VoltageRect.x, VoltageRect.y, VoltageRect.x + VoltageRect.w, VoltageRect.y + VoltageRect.h, 5, 100, 60, 0, 255);
    snprintf(trig_txt, 50, " CH1  %.2fV/d   ", vdiv);

    surface = TTF_RenderText_Blended(font18, trig_txt, YELLOW_COLOR);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, nullptr, &VoltageRect);

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    // Render Time aria
    SDL_SetRenderDrawColor(renderer, 100, 60, 0, 255);
    // SDL_RenderFillRect(renderer, &trigger_box);
    roundedBoxRGBA(renderer, TimeRect.x, TimeRect.y, TimeRect.x + TimeRect.w, TimeRect.y + TimeRect.h, 5, 100, 60, 0, 255);
    if (tdiv < 1000)
        snprintf(trig_txt, 50, " Time:  %.2fus/d   ", tdiv);
    else
        snprintf(trig_txt, 50, " Time:  %.2fms/d   ", tdiv / 1000);

    surface = TTF_RenderText_Blended(font18, trig_txt, YELLOW_COLOR);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, nullptr, &TimeRect);

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    // Mesurement  aria
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // SDL_RenderFillRect(renderer, &trigger_box);
    roundedBoxRGBA(renderer, MesureRect.x, MesureRect.y, MesureRect.x + MesureRect.w, MesureRect.y + MesureRect.h, 3, 255, 255, 255, 255);
    roundedBoxRGBA(renderer, MesureRect.x + 1, MesureRect.y + 1, MesureRect.x + MesureRect.w - 2, MesureRect.y + MesureRect.h - 1, 3, 0, 0, 0, 255);
    // // Cleanup
    // SDL_DestroyTexture(texture);
    // SDL_FreeSurface(surface);
}

// Clear and Draw the scope grid
void ui::grid()
{
    // Set the drawing color
    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, 255); // black color
    // Render the filled rectangle
    SDL_RenderFillRect(renderer, &grid_box);

    // Draw borders
    SDL_SetRenderDrawColor(renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, 255); // white color
    SDL_RenderDrawRect(renderer, &grid_border_box);

    // Draw the grid
    SDL_SetRenderDrawColor(renderer, GRID_COLOR.r, GRID_COLOR.g, GRID_COLOR.b, GRID_COLOR.a);
    for (int x = grid_x_o; x <= grid_w - grid_x_o; x += cell_size)
    {
        SDL_RenderDrawLine(renderer, x, grid_y_o, x, grid_h - grid_y_o);
    }
    for (int y = grid_y_o; y <= grid_h - grid_y_o; y += cell_size)
    {
        SDL_RenderDrawLine(renderer, grid_x_o, y, grid_w - grid_x_o, y);
    }

    // Draw zero level
    SDL_SetRenderDrawColor(renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, 255); // white color
    SDL_RenderDrawLine(renderer, grid_border_box.x, zero_line_y, zero_line_x2, zero_line_y);
}

// This function finds the trigger point and cumpute signal main frequency using basic approach nothing fancy like FFT, TBI
void ui::getTrigger()
{
    int trigLevel = (max_adc_value * (trigVoltage / (2.0 * atten) + offsetVoltage)) / 3.3; // ADC level at which we should trigger
    int trigPoint2;                                                                        // another trigger point, this will help us determine the period of the signal

    trigPoint = 0;
    trigged = 0;
    measuredFreq = 0;

    // The trigged variable will be 0 if we're not triggering, 1 if we only found 1 trigger point and 2 if we have at least two trigger points
    for (int i = 1; i < RECIEVE_LENGTH / 2 && trigged != 2; i++) // we're looking for trigger points in the first half of the buffer
        if ((trig == RISING && SAMPLES_BUFFER[i] >= trigLevel && SAMPLES_BUFFER[i - 1] < trigLevel) || (trig == FALLING && SAMPLES_BUFFER[i] <= trigLevel && SAMPLES_BUFFER[i - 1] > trigLevel))
        {
            if (!trigged) // Looking for the first trigger point
            {
                trigPoint = i;
                trigged = 1;
            }
            else // Looking for the second one
            {
                trigPoint2 = i;
                trigged = 2;
            }
        }

    if (trigged == 2) // If we found at least two trigger points
    {
        sigPer = sampPer * (trigPoint2 - trigPoint); // we compute the period of the signal in uS
        measuredFreq = 1000000.0 / sigPer;           // and then we convert it into frequency, in Hz
    }
}

// Draw the waveform trace on the screen
void ui::draw_wave(SDL_Color wave_color)
{
    maxVoltage = LOWER_VOLTAGE;
    minVoltage = UPPER_VOLTAGE;

    for (int i = 0; i <= RECIEVE_LENGTH / 2; i++)
    {
        // If we're looping through the buffer, let's compute the minimum and maximum voltage values while we're at it
        float voltage1 = atten * frontendVoltage(SAMPLES_BUFFER[i + trigPoint + move]);
        float voltage2 = atten * frontendVoltage(SAMPLES_BUFFER[i + trigPoint + 1 + move]);
        if (voltage2 > maxVoltage)
            maxVoltage = voltage2;
        if (voltage2 < minVoltage)
            minVoltage = voltage2;

        // Draw lines between sample points
        topClip = 0;
        bottomClip = 0;
        int16_t y1 = (cell_size * Y_CELLS / 2 - 1) - (voltage1 * cell_size / vdiv);
        int16_t y2 = (cell_size * Y_CELLS / 2 - 1) - (voltage2 * cell_size / vdiv);
        if (y1 > (cell_size * Y_CELLS))
        {
            y1 = (cell_size * Y_CELLS);
            bottomClip = 1;
        }
        if (y2 > (cell_size * Y_CELLS))
        {
            y2 = (cell_size * Y_CELLS);
            bottomClip = 1;
        }
        if (y1 < 0)
        {
            y1 = 0;
            topClip = 1;
        }
        if (y2 < 0)
        {
            y2 = 0;
            topClip = 1;
        }

        // Draw line betwen 2 samples in grid aria
        if (i + 2 <= draw_aria_max_x)
            thickLineRGBA(renderer, (i + 1) +grid_x_o, y1 + grid_border_box.y,( i + 2 ) + grid_x_o, y2 + grid_border_box.y, 2, wave_color.r, wave_color.g, wave_color.b, wave_color.a);
    }
}

// Convert ADC sample to voltage at frontend input
float ui::frontendVoltage(uint16_t samp)
{
    return 2 * (((3.3 * samp) / max_adc_value) - offsetVoltage);
}