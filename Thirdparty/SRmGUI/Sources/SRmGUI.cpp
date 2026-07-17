//
// Created by sunvy on 28/06/2026.
//

#include "SRmGUI.h"

#include "../Include/Type.h"

namespace
{
    std::unique_ptr<SRmGUI::Context> m_CurrentContext = nullptr;
}

namespace SRmGUI
{
    Context& CreateContext()
    {
        if (!m_CurrentContext)
            m_CurrentContext = std::make_unique<Context>();
        return *m_CurrentContext;
    }

    Context& GetContext()
    {
        return *m_CurrentContext;
    }

    bool HasContext()
    {
        return m_CurrentContext != nullptr;
    }

    void Shutdown()
    {
        if (m_CurrentContext)
            m_CurrentContext.reset();
    }

    const FormeDatas& GetDrawData()
    {
        if (!m_CurrentContext)
        {
        }
        return m_CurrentContext->GetData();
    }
}
