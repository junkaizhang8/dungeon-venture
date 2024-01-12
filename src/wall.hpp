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
    Wall(wall_id wallId, std::shared_ptr<Vertex> start, std::shared_ptr<Vertex> end) : id(wallId),\
                                                                                       startVertex(start),\
                                                                                       endVertex(end) {}
    ~Wall() override = default;
    void drawObj(std::shared_ptr<Renderer> renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) override;
    void drawObjSelected(std::shared_ptr<Renderer> renderer, int gridLeft, int gridRight, int gridTop, int gridBottom) override;
    void resetVertices();
    wall_id getId() { return id; }
    void setId(wall_id newId) { id = newId; }
    std::shared_ptr<Vertex> getStartVertex() { return startVertex; }
    void setStartVertex(std::shared_ptr<Vertex> v) { startVertex = v; }
    std::shared_ptr<Vertex> getEndVertex() { return endVertex; }
    void setEndVertex(std::shared_ptr<Vertex> v) { endVertex = v; }

private:
    static wall_id largestId;
    wall_id id;
    std::shared_ptr<Vertex> startVertex;
    std::shared_ptr<Vertex> endVertex;
};