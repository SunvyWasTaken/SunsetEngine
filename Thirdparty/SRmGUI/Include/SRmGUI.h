//
// Created by sunvy on 28/06/2026.

#pragma once

#include "Context.h"
#include "Widget.h"
#include "Text.h"
#include "Panel.h"

namespace SRmGUI
{
    Context& CreateContext();

    Context& GetContext();

    bool HasContext();

    void Shutdown();

    const FormeDatas& GetDrawData();
}
