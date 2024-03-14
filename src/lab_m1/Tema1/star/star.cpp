#include "star.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateStar(
    const std::string &name,
    float radius,
    int z,
    glm::vec3 color
    )
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, radius, z), color), // U 0 
        VertexFormat(glm::vec3(-radius * cos(0.3224995238), radius * sin(0.3224995238), z), color), // N 1
        VertexFormat(glm::vec3(-radius * cos(0.9481483708), -radius * sin(0.9481483708), z), color), // W 2
        VertexFormat(glm::vec3(radius * cos(0.9481483708), -radius * sin(0.9481483708), z), color), // Z 3
        VertexFormat(glm::vec3(radius * cos(0.3224995238), radius * sin(0.3224995238), z), color), // V 4
        VertexFormat(glm::vec3(-radius / (2 / 0.57) * cos(0.3224995238), radius * sin(0.3224995238), z), color), // Q 5
        VertexFormat(glm::vec3(radius / (2 / 0.57) * cos(0.3224995238), radius * sin(0.3224995238), z), color) // R 6
    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices = {
        2, 4, 5, 
        1, 3, 6,
        0, 5, 6
        };


    star->InitFromData(vertices, indices);
    return star;
}
