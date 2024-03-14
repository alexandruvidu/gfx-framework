#include "hex.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

#define INNER_RADIUS 0.66
#define INNER_COLOR glm::vec3(94 / 255.0, 178 / 255.0, 162 / 255.0)

Mesh* object2D::CreateHex(
    const std::string &name,
    float radius,
    glm::vec3 color,
    float z)
{
    float test = radius / 2.0;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, z), color), // A
        VertexFormat(glm::vec3(0, 2 * test, z), color), // B
        VertexFormat(glm::vec3(1.7320508056767 * test, 1 * test, z), color), // C
        VertexFormat(glm::vec3(1.7320508056767 * test, -1 * test, z), color), // D
        VertexFormat(glm::vec3(0, -2 * test, z), color), // E
        VertexFormat(glm::vec3(-1.7320508056767 * test, -1 * test, z), color), // F
        VertexFormat(glm::vec3(-1.7320508056767 * test, 1 * test, z), color), // G

        VertexFormat(glm::vec3(0, 2 * test * INNER_RADIUS, z + 1), INNER_COLOR), // B 7
        VertexFormat(glm::vec3(1.7320508056767 * test * INNER_RADIUS, 1 * test * INNER_RADIUS, z + 1), INNER_COLOR), // C 8
        VertexFormat(glm::vec3(1.7320508056767 * test * INNER_RADIUS, -1 * test * INNER_RADIUS, z + 1), INNER_COLOR), // D 9 
        VertexFormat(glm::vec3(0, -2 * test * INNER_RADIUS, z + 1), INNER_COLOR), // E 10
        VertexFormat(glm::vec3(-1.7320508056767 * test * INNER_RADIUS, -1 * test * INNER_RADIUS, z + 1), INNER_COLOR), // F 11 
        VertexFormat(glm::vec3(-1.7320508056767 * test * INNER_RADIUS, 1 * test * INNER_RADIUS, z + 1), INNER_COLOR) // G 12
    };

    Mesh* hex = new Mesh(name);
    std::vector<unsigned int> indices = { 
        0, 1, 6, // ABG
        0, 6, 5, // AGF
        0, 5, 4, // AFE
        0, 4, 3, //
        0, 3, 2,
        0, 2, 1,

        0, 7, 12,
        0, 12, 11,
        0, 11, 10,
        0, 10, 9,
        0, 9, 8,
        0, 8, 7

        };


    hex->InitFromData(vertices, indices);
    return hex;
}
