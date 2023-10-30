#ifndef UI_HPP
#define UI_HPP

#include "inc.h"

#define UPPER_VOLTAGE (atten * 3.3)
#define LOWER_VOLTAGE (atten * -3.3)

#define RISING 1
#define FALLING 0

#define Y_CELLS 18
#define X_CELLS 31

// #define X_CELLS 31/2

typedef SDL_Surface *(*IMG_LoadFunc)(const char *);

class ui
{
private:
    // SDL2 resoureces for rendring
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Surface *surface_T;
    SDL_Texture *texture_T;



    // Grid Aria Const offset aria and shape vars
    uint8_t cell_size = 30;
    uint8_t grid_x_o = 35;
    uint8_t grid_y_o = 40;
    uint16_t grid_w = 1000;
    uint16_t grid_h = 620;
    SDL_Rect grid_box = {
        grid_x_o,
        grid_y_o,
        grid_w - grid_x_o * 2,
        grid_h - grid_y_o * 2};

    SDL_Rect grid_border_box = {
        grid_x_o,
        grid_y_o,
        grid_w - grid_x_o * 2,
        grid_h - grid_y_o * 2};

    SDL_Rect h_box1 = {
        grid_w - grid_x_o,
        grid_y_o,
        100,
        grid_h - grid_x_o * 2};

    int zero_line_y = 9 * cell_size + grid_border_box.y;
    int zero_line_x2 = grid_border_box.x + grid_border_box.w;

    // Waveform preview offset and shape vars
    uint16_t waveform_privew_x = 300;
    uint16_t waveform_privew_y = 5;
    uint16_t waveform_privew_w = 366;
    uint16_t waveform_privew_h = 33;
    SDL_Rect privew_box = {
        waveform_privew_x,
        waveform_privew_y,
        waveform_privew_w,
        waveform_privew_h};
    SDL_Rect focus_box = {
        307,
        14,
        60,
        16};
    SDL_Rect focus_box_T = {
        focus_box.x + 25,
        3,
        13,
        16};
    SDL_Rect destRect = {
        302,
        8,
        waveform_privew_w - 4,
        25};
    uint16_t T_Offect = 0;
    uint16_t move = 0; // used to move the wavefrom across x-axis

    // Trigger Aria Rnedring variables
    SDL_Rect trigger_box = {
        waveform_privew_x + waveform_privew_w + 150,
        6,
        150,
        28};

    // Scope Mode Rendring Aria, vars and Texture vars
    SDL_Rect ModeRect = {
        140,
        6,
        110,
        28};

    // Voltage dev Rendring Aria
    SDL_Rect VoltageRect = {
        140,
        grid_box.y + grid_box.h + 7,
        110,
        28};

    // Time dev Rendring Aria
    SDL_Rect TimeRect = {
        waveform_privew_x + waveform_privew_w + 150,
        grid_box.y + grid_box.h + 7,
        140,
        28};

    // Mesure Aria
    SDL_Rect MesureRect = {
        grid_w - grid_x_o + 5,
        grid_y_o,
        230,
        grid_h - grid_y_o * 2};

    int atten = 2;  // Attenuation
    float vdiv = 1; // Volts per division

    uint8_t trigged;         // whether or not we're triggered
    int trigPoint;           // triggering point
    float trigVoltage = 0.0; // Trigger level
    uint8_t trig = RISING;   // Trigger slope

    float tdiv = 20;   // uS per division
    float sampPer;     // Sample period in uS (how long it takes to measure one sample)
    uint32_t sampRate; // Sample rate

    float maxVoltage, minVoltage; // Voltage measurements
    float sigPer;                 // Time measurements

    float offsetVoltage = 1.6540283; // Reference voltage of the the analog frontend

    uint16_t max_adc_value = 255; // 255 for 8 bits resolution

    uint8_t topClip, bottomClip; // Whether or not we're clipping through the graticule

    uint8_t _t_id;

    // Precalculated data used for rendring waveform
    uint16_t draw_aria_max_x = grid_w - grid_x_o * 2;

public:
    float measuredFreq;
    uint8_t Mode = 1; // 0  for Stop and 1 for Runing

    // Send cmd to device public vars
    std::string cmd = "";
    bool send_flag = false;

    ui();
    ~ui();

    void key_handler(SDL_Event &event);
    void waveform_preview();
    void grid();
    void getTrigger();
    void draw_wave(SDL_Color wave_color);
    float frontendVoltage(uint16_t samp);
    void setTimerFreq(uint32_t Timer_freq);
};

#endif