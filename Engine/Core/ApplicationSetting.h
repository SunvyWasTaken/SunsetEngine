//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_APPLICATIONSETTING_H
#define SUNSETCRAFT_APPLICATIONSETTING_H

#include <string>
#include <glm/vec2.hpp>

namespace SunsetEngine
{
    struct ApplicationSetting
    {
        glm::ivec2 WindowSize;
        std::string WindowTitle;
    };
}


#endif //SUNSETCRAFT_APPLICATIONSETTING_H