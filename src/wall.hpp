#pragma once

#include <memory>
#include "vertex.hpp"
#include "gridobject.hpp"
#include "renderer.hpp"

#define GRID_WALL_WIDTH 1
#define WALL_COLOUR YELLOW
#define WALL_SELECTED_COLOUR GREEN

typedef int wall_id;

class Wall : public GridObject
{
public:
    Wall() : id(largestId++) {}
    Wall(const std::shared_ptr<Vertex> &start, const std::shared_ptr<Vertex> &end)
        : id(largestId++), startVertex(start), endVertex(end) {}
    ~Wall() override = default;
    void drawObj(const Renderer *const renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) const override;
    void drawObjSelected(const Renderer *const renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) const override;
    void resetVertices();
    wall_id getId() const { return id; }
    std::shared_ptr<Vertex> getStartVertex() const { return startVertex; }
    void setStartVertex(const std::shared_ptr<Vertex> &v) { startVertex = v; }
    std::shared_ptr<Vertex> getEndVertex() const { return endVertex; }
    void setEndVertex(const std::shared_ptr<Vertex> &v) { endVertex = v; }

private:
    static wall_id largestId;
    wall_id id;
    std::shared_ptr<Vertex> startVertex;
    std::shared_ptr<Vertex> endVertex;
};