//
// Created by sunvy on 28/06/2026.
//

#pragma once

namespace SRmGUI
{
    using TextureID = uint32_t;

    namespace Forme
    {
        struct Base
        {
            glm::vec2 Position {0, 0};
            glm::vec2 Size {0, 0};
            glm::vec4 Color {1.0f};
        };

        struct Rectangle : Base
        {
            int Radius = 0;
        };
        struct Image : Base
        {
            TextureID texture = 0;
            glm::vec4 Uv{1.f, 1.f, 0.f, 0.f};
        };
        struct Text : Base
        {
            std::string text;
        };

        using Type = std::variant<Rectangle, Image, Text>;
    }

    using FormeDatas = std::vector<Forme::Type>;

    template<class... Ts>
    struct overloads : Ts... { using Ts::operator()...; };
}
