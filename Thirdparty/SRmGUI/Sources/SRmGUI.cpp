//
// Created by sunvy on 28/06/2026.
//

#include "SRmGUI.h"

#include "Type.h"

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

    void Shutdown()
    {
        if (m_CurrentContext)
            m_CurrentContext.reset();
    }

    FormeDatas GetFormeDatas()
    {
        if (!m_CurrentContext)
        {
            return {};
        }
        return m_CurrentContext->GetData();
    }
}
