//
// Created by sunvy on 28/06/2026.
//

#pragma once

namespace SRmGUI
{
    namespace Forme
    {
        struct Rectangle;
        struct Image;
        struct Text;
        using Type = std::variant<Rectangle, Image, Text>;
    }

    using FormeDatas = std::vector<Forme::Type>;

    static void Opengl_Init();

    static void Opengl_Shutdown();

    static void Opengl_DrawData(const FormeDatas& formesData);
}
