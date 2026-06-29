//
// Created by sunvy on 28/06/2026.

#pragma once

#include "Context.h"
#include "Widget.h"

namespace SRmGUI
{
    Context& CreateContext();

    Context& GetContext();

    bool HasContext();

    void Shutdown();

    const FormeDatas& GetDrawData();
}
