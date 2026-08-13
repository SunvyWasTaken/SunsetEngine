//
// Created by sunvy on 11/08/2026.
//

#ifndef SUNSETENGINE_AUDIO_H
#define SUNSETENGINE_AUDIO_H

#include "AudioBuffer.h"
#include "AudioStream.h"

namespace Sunset
{
    using Audio = std::variant<AudioBuffer, AudioStream>;
}

#endif //SUNSETENGINE_AUDIO_H