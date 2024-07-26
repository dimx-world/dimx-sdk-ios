#pragma once

#include <config/Config.h>

class Mesh;
struct MeshBuilder
{
    static bool construct(Mesh& mesh, const Config& config);
};
