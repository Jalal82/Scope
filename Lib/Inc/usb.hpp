#ifndef USB_HPP
#define USB_HPP

#include <iostream>
#include <cstring>
// USB Com Library
#include "asio.hpp"

class usb
{
private:
    std::string _port;
    int _baude_rate;
    asio::io_service _io;
    asio::serial_port _serial;

public:
    usb(std::string port, int baude_rate);
    ~usb();

    std::string read_line();
    std::vector<uint8_t> read_uint8_buffer(size_t count);
    void write_uint8_buffer(const uint8_t* buffer, size_t size);
    void send_cmd(std::string cmd);
};

#endif

