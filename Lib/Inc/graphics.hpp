#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "ui.hpp"



class graphics
{
private:
    uint8_t _id;

    
    // Thread proto definiton
    void _sendThread(usb &device);
    void _receiveThread(usb &device);
    std::thread send_thread;

    ui scope_ui;

public:
    // Public vars
    bool isRunning = true;

    // class methods
    graphics(uint8_t id);
    ~graphics();

    void sendThread(usb &device);
    void scope_loop(usb &device);
};

#endif