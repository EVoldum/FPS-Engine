#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include <iostream>

static ma_engine engine;

void initAudio()
{
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
    {
        std::cout << "Failed to initialize audio\n";
    }
}

void playSound(const std::string &path)
{
    ma_engine_play_sound(&engine, path.c_str(), NULL);
}

void cleanupAudio()
{
    ma_engine_uninit(&engine);
}