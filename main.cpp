#include <iostream>
#include "graphics.hpp"

#ifdef _WIN32
int WinMain()
#else
int main()
#endif
{
    usb device1("COM10", 1180000);
    usb device2("COM9", 1180000);

    graphics scope(0);
    scope.sendThread(device1);
    scope.scope_loop(device2);

    return 0;
}