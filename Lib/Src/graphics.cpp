#include "graphics.hpp"

graphics::graphics(uint8_t id)
{
    _id = id;
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // Create a resizable window
    window = SDL_CreateWindow("S Scope", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, 0);
    // Load the font
    font18 = TTF_OpenFont("./Res/Aller_Rg.ttf", 18);

    // Set the minimum/max window size
    SDL_SetWindowMinimumSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetWindowMaximumSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Clear the window with back color
    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    SDL_RenderClear(renderer);
}

graphics::~graphics()
{
    if (send_thread.joinable())
    {
        send_thread.detach();
    }
    std::cout << "[info]: Closing threads and cleaning up memory resources!" << std::endl;
}

void graphics::_sendThread(usb &device)
{

    while (isRunning)
    {
        // device.send_cmd("Hello from cpp\n");
        device.write_uint8_buffer(sine_wave, sizeof(sine_wave));
        // Sleep for some time before sending the next data
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if (!isRunning)
        {
            break;
        }
    }
}

void graphics::sendThread(usb &device)
{
    send_thread = std::thread([this, &device]()
                              { _sendThread(device); });
}

void graphics::_receiveThread(usb &device)
{
    auto startTime = std::chrono::steady_clock::now();
    std::vector<uint8_t> receivedData;

    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
    while (isRunning)
    {
        if (scope_ui.Mode)
        {
            // get the samples from hw register
            receivedData = device.read_uint8_buffer(RECIEVE_LENGTH);

            for (const auto &sample : receivedData)
            {
                SAMPLES_BUFFER[SAMPLE_INDEX] = sample;
                SAMPLE_INDEX++;
                if (LOG && LOG_INDEX < LOG_BUFFER_DEPTH)
                {
                    LOG_BUFFER[LOG_INDEX] = sample;
                    LOG_INDEX++;
                }
                samplespersecond++;
            }
            finishedConversion = 1;

            currentTime = std::chrono::steady_clock::now();
            elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);

            if (elapsedTime >= std::chrono::seconds(1))
            {
                startTime = currentTime;
#if RECIEVE_DEBUG
                std::cout << "Recivead sampels/sec: ";
                std::cout << samplespersecond << std::endl;
                std::cout << scope_ui.measuredFreq << std::endl;
#endif
                samplespersecond = 0;
            }

            if (!isRunning)
            {
                break;
            }
        }
    }
}

void graphics::scope_loop(usb &device)
{

    std::thread receiveThread([this, &device]()
                              { _receiveThread(device); });

    // Render waveform preview aria before the loop
    scope_ui.waveform_preview();

    while (isRunning)
    {
        Uint32 frameStart = SDL_GetTicks(); // Get the current time at the start of the frame
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }

            else if (event.type == SDL_KEYDOWN)
            {
                scope_ui.key_handler(event);
            }
        }

        // Clear the window with back color
        // SDL_SetRenderDrawColor(renderer, BACK_COLOR.r, BACK_COLOR.g, BACK_COLOR.b, BACK_COLOR.a);
        // SDL_RenderClear(renderer);

        // Clear the drawing aria and render the scope grid
        scope_ui.grid();

        // Find the trigger point
        scope_ui.getTrigger();

        // Draw the wave
        scope_ui.draw_wave(ORANGE_COLOR);

        // Update the window
        SDL_RenderPresent(renderer);

        Uint32 frameEnd = SDL_GetTicks();             // Get the current time at the end of the frame
        Uint32 frameDuration = frameEnd - frameStart; // Calculate the duration of the frame

        // Delay if the frame finished faster than the desired frame time (1/60 seconds)
        if (frameDuration < 16)
        {
            SDL_Delay(16 - frameDuration);
        }
    }
    if (receiveThread.joinable())
    {
        receiveThread.detach();
    }

    // Cleanup and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}