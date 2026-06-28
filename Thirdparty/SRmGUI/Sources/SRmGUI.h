//
// Created by sunvy on 28/06/2026.

#pragma once

#include "Context.h"

#include "SRmGUI_fwd.h"

namespace SRmGUI
{
    Context& CreateContext();

    Context& GetContext();

    void Shutdown();

    const FormeDatas& GetDrawData();
}
