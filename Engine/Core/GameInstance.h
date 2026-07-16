//
// Created by sunvy on 04/07/2026.
//

#pragma once

namespace Sunset
{
    class World;

    class GameInstance
    {
    public:
        GameInstance();
        virtual ~GameInstance();

        void Update(float dt);

        void Draw();

        std::shared_ptr<World> m_ActiveWorld;
    };
} // Sunset