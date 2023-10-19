#include "usb.hpp"

usb::usb(std::string port, int baude_rate)
    : _io(), _serial(_io, port)
{
    _serial.set_option(asio::serial_port_base::baud_rate(baude_rate));
    
}

usb::~usb()
{
    _serial.close();
}

std::string usb::read_line()
{
    // Reading data char by char, code is optimized for simplicity, not speed
    char c;
    std::string result;
    for (;;)
    {
        asio::read(_serial, asio::buffer(&c, 1));
       
        switch (c)
        {
        case '\r':
            break;
        case '\n':
            return result;
        default:
            result += c;
        }
    }
}

std::vector<uint8_t> usb::read_uint8_buffer(size_t count)
{
    std::vector<uint8_t> buffer(count);
    for (size_t i = 0; i < count; ++i)
    {
        uint8_t value;
        asio::read(_serial, asio::buffer(&value, sizeof(uint8_t)));
        buffer[i] = value;
    }
    return buffer;
}

void usb::write_uint8_buffer(const uint8_t* buffer, size_t size)
{
    asio::write(_serial, asio::buffer(buffer, size));
}

void usb::send_cmd(std::string cmd)
{
     asio::write(_serial, asio::buffer(cmd));
}

