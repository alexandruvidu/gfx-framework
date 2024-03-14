#include "square.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSq(
    const std::string &name,
    float length,
    glm::vec3 color,
    bool fill)
{

    float t = length;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-2/2 - t/2, -2/2 - t/2, 0), color),
        VertexFormat(glm::vec3(2/2 + t/2, -2/2 - t/2, 0), color),
        VertexFormat(glm::vec3(2/2 + t/2, 2/2 + t/2, 0), color),
        VertexFormat(glm::vec3(-2/2 - t/2, 2/2 + t/2, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
