#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

float squareSide;

Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    squareSide = 100;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 3.14 * 2 / 15;
    rot2 = 0;
    rot3 = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab3::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    int xc = resolution.x / 4;
    int yc = resolution.y / 4;
    rot2 += deltaTimeSeconds * angularStep;
    rot3 += deltaTimeSeconds * angularStep;
    glm::vec3 sun = glm::vec3(xc - squareSide / 2, yc - squareSide / 2, 1);
    // TODO(student): Update steps for translation, rotation and scale,
    // in order to create animations. Use the class variables in the
    // class header, and if you need more of them to complete the task,
    // add them over there!

    modelMatrix1 = glm::mat3(1);
    modelMatrix1 *= transform2D::Translate(xc - squareSide / 2, yc - squareSide / 2);
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented.
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix1);


    modelMatrix2 = glm::mat3(1);
    modelMatrix2 *= transform2D::Translate(sun[0], sun[1]);
    modelMatrix2 *= transform2D::Rotate(rot3);
    modelMatrix2 *= transform2D::Translate(-1 * sun[0], -1 * sun[1]);
    modelMatrix2 *= transform2D::Translate(4 * squareSide, 0);
    modelMatrix2 *= modelMatrix1;

    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix2);
    // float sq3_x = sq2_x - 1.5 * squareSide;
    // float sq3_y = sq2_y - 1.5 * squareSide;
    glm::vec3 earth = glm::vec3(
        transform2D::Translate(sun[0], sun[1]) *
        transform2D::Rotate(rot2) *
        transform2D::Translate(-1 * sun[0], -1 * sun[1]) *
        transform2D::Translate(4 * squareSide, 0) * sun);


    modelMatrix3 = glm::mat3(1);
    // modelMatrix3 *= transform2D::Translate(earth[0], earth[1]);
    // modelMatrix3 *= transform2D::Rotate(rot3);
    // modelMatrix3 *= transform2D::Translate(-1 * earth[0], -1 * earth[1]);
    // modelMatrix3 *= transform2D::Translate(1.5 * squareSide, 0);
    // modelMatrix3 *- modelMatrix2;
    // TODO(student): Create animations by multiplying the current
    // transform matrix with the matrices you just implemented
    // Remember, the last matrix in the chain will take effect first!

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix3);
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
