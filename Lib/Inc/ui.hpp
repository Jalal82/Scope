#ifndef UI_HPP
#define UI_HPP


#include "inc.h"

#define UPPER_VOLTAGE (atten * 3.3)
#define LOWER_VOLTAGE (atten * -3.3)

#define RISING 1
#define FALLING 0



class ui
{
private:
    // Grid Aria Const offset aria and shape vars
    uint8_t grid_x_o = 35;
    uint8_t grid_y_o = 40;
    uint16_t grid_w = 1000;
    uint16_t grid_h = 620;
    SDL_Rect grid_box = {
        grid_x_o,
        grid_y_o,
        grid_w - grid_x_o * 2,
        grid_h - grid_y_o * 2};
    SDL_Rect h_box1 = {
        grid_w - grid_x_o,
        grid_y_o,
        100,
        grid_h - grid_x_o * 2};

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

    // Scope Mode Rendring Aria, vars and Texture vars
    uint8_t Mode = 1; // 0  for Stop and 1 for Runing
    SDL_Rect ModeRect = {
        150,
        8,
        100,
        25};

    int atten = 2;  // Attenuation
    float vdiv = 2; // Volts per division

    uint8_t trigged;       // whether or not we're triggered
    int trigPoint;         // triggering point
    float trigVoltage = 0; // Trigger level
    uint8_t trig = RISING; // Trigger slope

    float tdiv = 20;   // uS per division
    float sampPer;     // Sample period in uS (how long it takes to measure one sample)

    float maxVoltage, minVoltage; // Voltage measurements
    float measuredFreq, sigPer;   // Time measurements

    uint8_t _t_id;

public:
    ui();
    ~ui();

    void key_handler(SDL_Event &event);
};



#endif