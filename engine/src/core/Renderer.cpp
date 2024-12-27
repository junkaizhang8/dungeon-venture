#include "Renderer.h"

#include "utils/debug.h"

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib,
                    const Shader& shader) const
{
    shader.bind();
    va.bind();
    ib.bind();
    glCall(
        glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const { glCall(glClear(GL_COLOR_BUFFER_BIT)); }