//
// Created by sunvy on 04/07/2026.
//

#include "GameInstance.h"

#include "GameFramework/World/World.h"

namespace Sunset
{
    GameInstance::GameInstance()
        : m_ActiveWorld(std::make_shared<World>())
    {
    }

    GameInstance::~GameInstance()
    {
        LOG("Engine", info, "GameInstance::~GameInstance()")
    }

    void GameInstance::Update(float dt)
    {
        if (m_ActiveWorld)
            m_ActiveWorld->Update(dt);
    }

    void GameInstance::Draw()
    {
        if (m_ActiveWorld)
            m_ActiveWorld->Draw();
    }
} // Sunset