//
// Created by sunvy on 11/08/2026.
//

#include "Audio.h"

namespace
{

}

namespace Sunset
{
    AudioBuffer Audio::Load(const std::string_view &path)
    {
        return {};
    }

    void Audio::Play2D(const std::shared_ptr<AudioBuffer> &audio)
    {
        // Get All source to find if there is already a source with the buffer.
        // Play the buffer sound.
    }

    void Audio::Play(const AudioBuffer &audioClip)
    {
    }

    void Audio::CreateAllSource()
    {
    }

    void Audio::ClearAllSource()
    {
    }
}
