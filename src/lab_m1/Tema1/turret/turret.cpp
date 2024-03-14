#include "turret.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateTurret(
    const std::string &name,
    float radius,
    glm::vec3 color,
    float z)
{
    float test = radius / 2.0;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, z), color), // A 0
        VertexFormat(glm::vec3(-0.75 * test, 1.5 * test, z), color), // B 1
        VertexFormat(glm::vec3(0.25 * test, 0, z), color), // C 2
        VertexFormat(glm::vec3(-0.75 * test, -1.5 * test, z), color), // D 3
        VertexFormat(glm::vec3(-1.75 * test, 0, z), color), // E 4
        VertexFormat(glm::vec3(0 * test, 0.3 * test, z), color), // F 5
        VertexFormat(glm::vec3(0 * test, -0.3 * test, z), color), // G 6
        VertexFormat(glm::vec3(1.5 * test, 0.3 * test, z), color), // H 7
        VertexFormat(glm::vec3(1.5 * test, -0.3 * test, z), color), // I 8
        
    };

    Mesh* turret = new Mesh(name);
    std::vector<unsigned int> indices = {
        2, 4, 1,
        3, 2, 4,
        6, 8, 5,
        8, 7, 5
        };


    turret->InitFromData(vertices, indices);
    return turret;
}
