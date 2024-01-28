#pragma once

#include "vertextree.hpp"
#include "walltree.hpp"
#include "vertexgraph.hpp"

struct MapData
{
    VertexTree vertexTree;
    WallTree wallTree;
    VertexGraph graph;
};