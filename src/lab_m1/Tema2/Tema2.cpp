#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <math.h>
#include <random>

using namespace std;
using namespace m1;

// Color constants
#define RAIL_COLOR glm::vec3(145 / 255.0, 150 / 255.0, 155 / 255.0)
#define BODY_COLOR glm::vec3(20 / 255.0, 40 / 255.0, 15 / 255.0)
#define CANNON_COLOR glm::vec3(60 / 255.0, 100 / 255.0, 40 / 255.0)
#define PROJECTILE_COLOR glm::vec3(0, 0, 0)
#define BUILDING_COLOR glm::vec3(1, 1, 1)

// Object sizes
#define WORLD_SIZE 100
#define BUILDING_SIZE 1.0f

// Timers
#define SHOOT_TIMER 3
#define GAME_TIMER 90
#define STATE_TIMER 3
#define MIN_STATE_TIMER 3

// Projectile constants
#define PROJECTILE_HEIGHT glm::vec3(0, 1, 0)
#define PROJECTILE_TIMER 10.0f
#define PROJECTILE_SPEED 5.0f
#define PROJECTILE_OFFSET 2.0f
#define PROJECTILE_SCALE_FACTOR transform3D::Scale(.1, .1, .1)
#define PROJECTILE_RADIUS 0.1f

// Tank constants
#define TANK_RADIUS sqrt(1.3f * 1.3f + .8f * .8f)
#define TANK_HEALTH 3
#define TANK_SPEED 3.5f
#define TANK_STATE_TIMER 1.5f
#define TANK_ANGULAR_SPEED 2.0f
#define LOCK_DISTANCE 20


int GetRandomNumberInRange(const int minInclusive, const int maxInclusive) {
    static std::random_device randomDevice;
    static std::default_random_engine randomEngine(randomDevice());
    std::uniform_int_distribution<int> uniformDist(minInclusive, maxInclusive);
    return uniformDist(randomEngine);
}

int GetNextMovementState(const int currentState) {
    int randomChange = GetRandomNumberInRange(0, 1);
    int nextState = currentState;
    switch (currentState) {
        case 4:
        case 0:
        case 1:
            nextState = (randomChange == 1) ? 2 : 3;
            break;
        case 2:
        case 3:
            nextState = (randomChange == 1) ? 0 : 1;
            break;
        default:
            break;
    }
    return nextState;
}

void Tema2::TankShoot(Tank* tank) {
    glm::vec3 direction = glm::normalize(glm::vec3(tank->turretForward.x, 0, tank->turretForward.z));
    projectiles.push_back({tank->pos + direction * PROJECTILE_OFFSET + PROJECTILE_HEIGHT, tank->turretForward, PROJECTILE_TIMER});
    tank->shootTimer = SHOOT_TIMER;
}

void Tema2::MoveTank(Tank* tank, glm::vec3 amount) {
    tank->pos += amount;
}

void Tema2::RotateTank(Tank* tank, float angle) {
    tank->tankRotation += angle;
    glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(tank->forward, 1);
    tank->forward = glm::normalize(glm::vec3(aux));

    aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(tank->turretForward, 1);
    tank->turretForward = glm::normalize(glm::vec3(aux));
}

void Tema2::EnemyMovement(float deltaTime) {
    for(auto i = enemies.begin(); i != enemies.end(); ++i) {
        if((*i).health > 0) {
            (*i).shootTimer -= deltaTime;
            (*i).stateTimer -= deltaTime;
            if((*i).shootTimer <= 0 && (*i).state == 4)
                TankShoot(&(*i));
            if(glm::distance((*i).pos, tank.pos) < LOCK_DISTANCE) {
                (*i).state = 4;
                (*i).stateTimer = 1;
                if(!(*i).locked)
                    (*i).shootTimer = 1;
                (*i).locked = 1;
            }
            else if((*i).stateTimer <= 0) {
                (*i).locked = 0;
                (*i).state = GetNextMovementState((*i).state);
                (*i).stateTimer = rand() % STATE_TIMER + MIN_STATE_TIMER;   
            }
            
            if((*i).state == 0) {
                glm::vec3 direction = glm::normalize(glm::vec3((*i).forward.x, 0, (*i).forward.z));
                MoveTank(&(*i), direction * TANK_SPEED * deltaTime);
            }
            if((*i).state == 1) {
                glm::vec3 direction = glm::normalize(glm::vec3((*i).forward.x, 0, (*i).forward.z));
                MoveTank(&(*i), -direction * TANK_SPEED * deltaTime);
            }
            if((*i).state == 2) {
                RotateTank(&(*i), TANK_ANGULAR_SPEED * deltaTime);
            }
            if((*i).state == 3) {
                RotateTank(&(*i), -TANK_ANGULAR_SPEED * deltaTime);
            }
            if((*i).state == 4) {
                glm::vec3 d1 = glm::normalize(glm::vec3((*i).turretForward.x, 0, (*i).turretForward.z));
                glm::vec3 d2 = glm::normalize(glm::vec3((*i).pos.x, 0, (*i).pos.z) - glm::vec3(tank.pos.x, 0, tank.pos.z)); 
                glm::vec3 vect = glm::cross(d1, d2);
                float scalar = glm::dot(d1, d2);
                float angle = acos(scalar), offset;

                if(angle < 0.1f)
                    offset = angle;
                else 
                    offset = TANK_ANGULAR_SPEED * deltaTime;
                
                if(abs(scalar) != 1) {
                    glm::vec4 aux;
                    if(vect.y < 0) {
                        aux = glm::rotate(glm::mat4(1.f), offset, glm::vec3(0, 1, 0)) * glm::vec4((*i).turretForward, 1);
                        (*i).turretRotation += offset;
                    }
                    else {
                        aux = glm::rotate(glm::mat4(1.f), -offset, glm::vec3(0, 1, 0)) * glm::vec4((*i).turretForward, 1);
                        (*i).turretRotation -= offset;
                    }
                    (*i).turretForward = glm::normalize(glm::vec3(aux));
                }
            } 
        }
    }
}

void Tema2::ProjectileMovement(float deltaTime) {
    for(auto i = projectiles.begin(); i != projectiles.end(); ++i) {
        glm::vec3 direction = glm::normalize(glm::vec3((*i).forward.x, 0, (*i).forward.z));
        (*i).pos += direction * PROJECTILE_SPEED * deltaTime;
        (*i).timer -= deltaTime;
        if((*i).timer <= 0) {
            projectiles.erase(i);
            i--;
        }
    }
}

void Tema2::CollisionTank(Tank* tank1, Tank* tank2) {
    glm::vec3 dif = tank1->pos - tank2->pos;
    float mod = 2 * TANK_RADIUS - glm::distance(tank1->pos, tank2->pos);
    glm::vec3 P = mod * glm::normalize(dif);
    tank1->pos += P * .8f;
    tank2->pos -= P * .8f;
}

void Tema2::CollisionBuilding(Tank* tank, Building* building, float deltaTime) {
    if(tank->pos.x + TANK_RADIUS > building->pos.x - BUILDING_SIZE / 2 * building->sizeX && 
        tank->pos.x - TANK_RADIUS < building->pos.x + BUILDING_SIZE / 2 * building->sizeX &&
        tank->pos.z + TANK_RADIUS > building->pos.z - BUILDING_SIZE / 2 * building->sizeZ &&
        tank->pos.z - TANK_RADIUS < building->pos.z + BUILDING_SIZE / 2 * building->sizeZ) {
            glm::vec3 dir = glm::normalize(building->pos - tank->pos);
            tank->pos -= dir * TANK_SPEED * deltaTime;
    }
}

void Tema2::CollisionBound(Tank* tank, float deltaTime) {
    if(tank->pos.x + TANK_RADIUS > WORLD_SIZE / 2 || tank->pos.x - TANK_RADIUS < -WORLD_SIZE / 2 ||
    tank->pos.z + TANK_RADIUS > WORLD_SIZE / 2 || tank->pos.z - TANK_RADIUS < -WORLD_SIZE / 2) {
        glm::vec3 dir = glm::normalize(glm::vec3(0, 0, 0) - tank->pos);
        tank->pos += dir * TANK_SPEED * deltaTime;
    }
}

void Tema2::CollisionDetection(float deltaTime) {
    // Tank-enemy collision
    for(auto i = enemies.begin(); i != enemies.end(); ++i) {
        for(auto j = enemies.begin(); j != enemies.end(); ++j)
            if(i != j)
                if(glm::distance((*i).pos, (*j).pos) < 2 * TANK_RADIUS)
                    CollisionTank(&(*i), &(*j));
        if(glm::distance((*i).pos, tank.pos) < 2 * TANK_RADIUS)
            CollisionTank(&(*i), &tank);
    }

    // Enemy-building collision
    for(auto i = enemies.begin(); i != enemies.end(); ++i)
        for(auto j = buildings.begin(); j != buildings.end(); ++j)
            CollisionBuilding(&(*i), &(*j), deltaTime);

    // Tank-building collision
    for(auto j = buildings.begin(); j != buildings.end(); ++j)
        CollisionBuilding(&tank, &(*j), deltaTime);

    // Projectile collisions with tanks and buildings
    for(auto i = projectiles.begin(); i != projectiles.end(); ++i) {
        for(auto j = enemies.begin(); j != enemies.end(); ++j) {
            if(glm::distance((*i).pos, (*j).pos) <= (PROJECTILE_RADIUS + TANK_RADIUS)) {
                (*j).health--;
                if((*j).health == 0)
                    ++killedEnemies;
                projectiles.erase(i);
                i--;
                break;
            }
        }
        if(glm::distance((*i).pos, tank.pos) <= (PROJECTILE_RADIUS + TANK_RADIUS)) {
            tank.health--;
            projectiles.erase(i);
            i--;
            break;
        }
        
        for(auto j = buildings.begin(); j != buildings.end(); ++j) {
            if((*i).pos.x + PROJECTILE_RADIUS > (*j).pos.x - BUILDING_SIZE / 2 * (*j).sizeX && 
            (*i).pos.x - PROJECTILE_RADIUS < (*j).pos.x + BUILDING_SIZE / 2 * (*j).sizeX &&
            (*i).pos.z + PROJECTILE_RADIUS > (*j).pos.z - BUILDING_SIZE / 2 * (*j).sizeZ &&
            (*i).pos.z - PROJECTILE_RADIUS < (*j).pos.z + BUILDING_SIZE / 2 * (*j).sizeZ) {
                projectiles.erase(i);
                i--;
                break;
            }
        }
    }

    // World bounds
    for(auto i = enemies.begin(); i != enemies.end(); ++i)
        CollisionBound(&(*i), deltaTime);
    CollisionBound(&tank, deltaTime);
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color, int tankHealth)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Send color
    GLint object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(object_color, 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Send health
    int health_location = glGetUniformLocation(shader->program, "health");
    glUniform1f(health_location, (float)tankHealth);
    
    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::RenderScene() {
    // Ground rendering  
    glm::mat4 groundMat = transform3D::Scale(WORLD_SIZE, 0.01, WORLD_SIZE); 
    RenderSimpleMesh(meshes["box"], shaders["ShaderTema"], groundMat, glm::vec3(0.5, 0.5, 0.5), -1);

    // Tank rendering
    glm::mat4 tankMat = transform3D::Translate(tank.pos.x, tank.pos.y, tank.pos.z) * transform3D::RotateOY(tank.tankRotation) * transform3D::Scale(0.2, 0.2, 0.2);
    glm::mat4 turretMat = tankMat * transform3D::RotateOY(tank.turretRotation);
    RenderSimpleMesh(meshes["senila1"], shaders["ShaderTema"], tankMat, RAIL_COLOR, tank.health);
    RenderSimpleMesh(meshes["senila2"], shaders["ShaderTema"], tankMat, RAIL_COLOR, tank.health);
    RenderSimpleMesh(meshes["corp"], shaders["ShaderTema"], tankMat, BODY_COLOR, tank.health);
    RenderSimpleMesh(meshes["turela"], shaders["ShaderTema"], turretMat, CANNON_COLOR, tank.health);
    RenderSimpleMesh(meshes["tun"], shaders["ShaderTema"], turretMat, RAIL_COLOR, tank.health);

    // Enemy rendering
    for(auto i = enemies.begin(); i != enemies.end(); ++i) {
        Tank t = *i;
        glm::mat4 enemyMat = transform3D::Translate(t.pos.x, t.pos.y, t.pos.z) * transform3D::Scale(0.2, 0.2, 0.2) * transform3D::RotateOY(t.tankRotation);
        glm::mat4 turretMat = enemyMat * transform3D::RotateOY(t.turretRotation);
        RenderSimpleMesh(meshes["senila1"], shaders["ShaderTema"], enemyMat, RAIL_COLOR, t.health);
        RenderSimpleMesh(meshes["senila2"], shaders["ShaderTema"], enemyMat, RAIL_COLOR, t.health);
        RenderSimpleMesh(meshes["corp"], shaders["ShaderTema"], enemyMat, BODY_COLOR, t.health);
        RenderSimpleMesh(meshes["turela"], shaders["ShaderTema"], turretMat, CANNON_COLOR, t.health);
        RenderSimpleMesh(meshes["tun"], shaders["ShaderTema"], turretMat, RAIL_COLOR, t.health);
    }

    // Building rendering
    for(auto i = buildings.begin(); i != buildings.end(); ++i)
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema"], transform3D::Translate((*i).pos.x, 0, (*i).pos.z) * transform3D::Scale((*i).sizeX, (*i).sizeY, (*i).sizeZ), BUILDING_COLOR, -1);

    // Projectile rendering
    for(auto i = projectiles.begin(); i != projectiles.end(); ++i)
        RenderSimpleMesh(meshes["proiectil"], shaders["ShaderTema"], transform3D::Translate((*i).pos.x, (*i).pos.y, (*i).pos.z) * PROJECTILE_SCALE_FACTOR, PROJECTILE_COLOR, -1);
}

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::LoadMeshes() {
    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("senila1");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "senila1.obj");
    meshes[mesh->GetMeshID()] = mesh;
    
    mesh = new Mesh("senila2");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "senila2.obj");
    meshes[mesh->GetMeshID()] = mesh;
    
    mesh = new Mesh("corp");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "corp.obj");
    meshes[mesh->GetMeshID()] = mesh;
    
    mesh = new Mesh("turela");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "turela.obj");
    meshes[mesh->GetMeshID()] = mesh;
    
    mesh = new Mesh("tun");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "tun.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("proiectil");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "proiectil.obj");
    meshes[mesh->GetMeshID()] = mesh;
}

void Tema2::InitTank() {
    tank.health = TANK_HEALTH;
    tank.tankRotation = 0;
    tank.turretRotation = 0;
    tank.shootTimer = SHOOT_TIMER;
    tank.pos = glm::vec3(0, 0, 0);
    tank.forward = glm::vec3(0, 0, -1);
    tank.turretForward = tank.forward;
    tank.state = -1;
}

void Tema2::InitBuildings() {
    int buildingNo = rand() % 3 + 3;
    for(int i = 0; i < buildingNo; ++i) {
        int sizeX = rand() % 4 + 1;
        int sizeY = rand() % 3 + 20;
        int sizeZ = rand() % 4 + 2;
        int posX = rand() % (WORLD_SIZE) - WORLD_SIZE / 2 + TANK_RADIUS + sizeX / 2;
        int posZ = rand() % (WORLD_SIZE) - WORLD_SIZE / 2 + TANK_RADIUS + sizeZ / 2;
        buildings.push_back({glm::vec3(posX, 0, posZ), sizeX, sizeY, sizeZ});
    }
}

void Tema2::InitEnemies() {
    int enemyNo = rand() % 4 + 5;
    for(int i = 0; i < enemyNo; ++i) {
        int posX = rand() % (WORLD_SIZE) - WORLD_SIZE / 2;
        int posZ = rand() % (WORLD_SIZE) - WORLD_SIZE / 2;
        int state = GetNextMovementState(rand() % 4);
        float stateChange = rand() % STATE_TIMER + MIN_STATE_TIMER;
        int startShootTimer = rand() % SHOOT_TIMER + 1;
        enemies.push_back({TANK_HEALTH, 0, 0, (float)startShootTimer, glm::vec3(posX, 0, posZ), glm::vec3(0, 0, -1), glm::vec3(0, 0, -1), state, stateChange, 0});
        for(auto i = buildings.begin(); i != buildings.end(); ++i)
            CollisionBuilding(&(enemies.back()), &(*i), 1);
    }
}

void Tema2::InitShader() {
    Shader *shader = new Shader("ShaderTema");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}

void Tema2::Init()
{
    camera = new m1::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    LoadMeshes();
    InitShader();

    gameTime = GAME_TIMER;
    InitTank();
    InitBuildings();
    InitEnemies();

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(135 / 255.0, 206 / 255.0, 235 / 255.0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x / 2, resolution.y / 2);
}

void Tema2::Update(float deltaTimeSeconds)
{
    if(!gameFinished) {
        if(tank.health == 0) {
            printf("Ai pierdut!\n");
            gameFinished = true;
        }
        else if(gameTime <= 0 || enemies.size() - killedEnemies == 0) {
            printf("Scor: %d\n", killedEnemies);
            gameFinished = true;
        }
    }
    camera->Set(glm::vec3(tank.pos.x + 5 * sin(tank.tankRotation), tank.pos.y + 2, tank.pos.z + 5 * cos(tank.tankRotation)),
            glm::vec3(tank.pos.x, tank.pos.y + 2, tank.pos.z), glm::vec3(0, 1, 0));
    ProjectileMovement(deltaTimeSeconds);

    if(!gameFinished) {
        gameTime -= deltaTimeSeconds;
        tank.shootTimer -= deltaTimeSeconds;
        EnemyMovement(deltaTimeSeconds);
        CollisionDetection(deltaTimeSeconds);
    }
    
    RenderScene();
}

void Tema2::FrameEnd()
{
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if(!gameFinished) {
        glm::vec3 direction = glm::normalize(glm::vec3(tank.forward.x, 0, tank.forward.z));
        if (window->KeyHold(GLFW_KEY_W))
            MoveTank(&tank, direction * TANK_SPEED * deltaTime);
        if (window->KeyHold(GLFW_KEY_A))
            RotateTank(&tank, TANK_ANGULAR_SPEED * deltaTime);
        if (window->KeyHold(GLFW_KEY_S))
            MoveTank(&tank, -direction * TANK_SPEED * deltaTime);
        if (window->KeyHold(GLFW_KEY_D))
            RotateTank(&tank, -TANK_ANGULAR_SPEED * deltaTime);
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event

}

void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 res = window->GetResolution();
    tank.turretRotation = ((float)mouseX * 2.f / ((float)res.x / 2.f / -3.14f) + 3.14f / 2.f);
    glm::vec4 aux = glm::rotate(glm::mat4(1.f), (float)mouseX * 2.f / ((float)res.x / 2.f / -3.14f) + 3.14f / 2.f, glm::vec3(0, 1, 0)) * glm::vec4(tank.forward, 1);
    tank.turretForward = glm::normalize(glm::vec3(aux));
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if(IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && !gameFinished) {
        if(tank.shootTimer <= 0)
            TankShoot(&tank);
    }
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
