//
// Created by sunvy on 14/06/2026.
//

#pragma once

namespace Sunset
{
    class Mesh;

    struct MeshLoader
    {
        static Mesh* LoadMesh(const std::string& path);
    };
} // Sunset