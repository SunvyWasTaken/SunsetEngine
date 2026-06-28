//
// Created by sunvy on 28/06/2026.

#pragma once

#include "Context.h"

#include "SRmGUI_fwd.h"

namespace SRmGUI
{
    static Context& CreateContext();
    static void Shutdown();
    static FormeDatas& GetDrawData();
}
