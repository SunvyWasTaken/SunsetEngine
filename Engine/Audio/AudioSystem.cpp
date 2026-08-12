//
// Created by sunvy on 11/08/2026.
//

#include "AudioSystem.h"

#include "AL/alc.h"

namespace
{
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
}

namespace Sunset
{
    void AudioSystem::Init()
    {
        LOG("Engine", info, "AudioSystem Init")
        device = alcOpenDevice(nullptr);
        context = alcCreateContext(device, nullptr);
        if (context)
            alcMakeContextCurrent(context);
    }

    void AudioSystem::Shutdown()
    {
        LOG("Engine", info, "AudioSystem Shutdown")
        alcCloseDevice(device);
    }
} // Sunset