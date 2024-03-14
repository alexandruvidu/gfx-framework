#include "lab_m1/Tema1/Tema1.h"

#include <iostream>
#include <random>

#include "transform2D.h"
#include "hex/hex.h"
#include "turret/turret.h"
#include "square/square.h"
#include "star/star.h"

#define ENEMY_PURPLE_COLOR glm::vec3(96 / 255.0, 32 / 255.0, 161 / 255.0)
#define ENEMY_YELLOW_COLOR glm::vec3(248 / 255.0, 216 / 255.0, 73 / 255.0)
#define ENEMY_ORANGE_COLOR glm::vec3(236 / 255.0, 113 / 255.0, 46 / 255.0)
#define ENEMY_BLUE_COLOR glm::vec3(32 / 255.0, 112 / 255.0, 243 / 255.0)
#define TURRET_SLOT_COLOR glm::vec3(132 / 255.0, 248 / 255.0, 127 / 255.0)
#define CURRENCY_COLOR glm::vec3(216 / 255.0, 57 / 255.0, 218 / 255.0)

#define SLOT_SIZE 200
#define CURRENCY_RADIUS (SLOT_SIZE / 3)
#define BULLET_RADIUS (SLOT_SIZE / 2 * 0.5)
#define ENEMY_RADIUS (SLOT_SIZE / 2 * 0.7)

#define GRID_STARTING_X (SLOT_SIZE / 2 + 200)
#define GRID_STARTING_Y (SLOT_SIZE / 2 + 50)
#define GRID_OFFSET (SLOT_SIZE * 1.25)

#define SHOP_STARTING_X (SLOT_SIZE / 2 + 100)
#define SHOP_STARTING_Y (SLOT_SIZE / 2 + 100)
#define SHOP_OFFSET (SLOT_SIZE * 1.5)

#define HEALTH_STARTING_X (SLOT_SIZE / 2 + 100)
#define HEALTH_STARTING_Y (SLOT_SIZE / 2 + 100)
#define HEALTH_OFFSET (SLOT_SIZE * .75)

#define STAR_STARTING_X (-SLOT_SIZE / 2 + 10)
#define STAR_OFFSET_X (SLOT_SIZE / 2 - 10)
#define STAR_OFFSET_Y (-SLOT_SIZE / 2 - 50)

#define CURRENCY_OFFSET_X (HEALTH_STARTING_X + 2 * HEALTH_OFFSET * 1.17)
#define CURRENCY_OFFSET_Y (HEALTH_STARTING_Y + 100)

#define STARTING_HEALTH 3
#define ENEMY_SPEED 200
#define BULLET_SPEED 200
#define ENEMY_SPAWN_RATE (rand() % 3 + 4)
#define CURRENCY_SPAWN_RATE (rand() % 3 + 4)
#define BULLET_SHOOT_RATE 1
#define ROTATION_SPEED (3.14 * 2 / 3)

#define BULLET_SPAWN_OFFSET (float)(SLOT_SIZE / 2 * 0.8)
#define ENEMY_SPAWN_OFFSET (SLOT_SIZE / 2)
#define DEATH_OFFSET_X 100

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    
    mouseposX = 0;
    mouseposY = 0;
    hasTurret = 0;
    hasTurretType = 9;
    enemySpawnTimer = ENEMY_SPAWN_RATE;
    currencySpawnTimer = CURRENCY_SPAWN_RATE;

    health = STARTING_HEALTH;
    starCount = 4;

    typesByInt[0] = "orange";
    typesByInt[1] = "blue";
    typesByInt[2] = "yellow";
    typesByInt[3] = "purple";

    prices[0] = 1;
    prices[1] = 2;
    prices[2] = 2;
    prices[3] = 3;

    for(int i = 0; i < 9; ++i)
        slots.push_back({false, (float)(GRID_STARTING_X + (i % 3) * GRID_OFFSET), (float)(GRID_STARTING_Y + (i / 3) * GRID_OFFSET), BULLET_SHOOT_RATE, 1});

    for(int i = 0; i < 9; ++i) 
        grid_location.push_back(transform2D::Translate(slots.at(i).posX, slots.at(i).posY));
    grid_location.push_back(transform2D::Translate(100, GRID_STARTING_Y + GRID_OFFSET) * transform2D::Scale(1, 7));

    for(int i = 0; i < 4; ++i)
        shops.push_back({(float)(SHOP_STARTING_X + i * SHOP_OFFSET), (float)(resolution.y / 2 - SHOP_STARTING_Y)});
    for(int i = 0; i < 4; ++i)
        shop_location.push_back(transform2D::Translate(shops.at(i).posX, shops.at(i).posY)); // Pentru vizualizare pe windows fara /2

    shops.at(0).starNo = 1;
    shops.at(1).starNo = 2;
    shops.at(2).starNo = 2;
    shops.at(3).starNo = 3;

    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < shops.at(i).starNo; ++j)
            shops.at(i).stars.push_back(transform2D::Translate(STAR_STARTING_X + j * STAR_OFFSET_X, STAR_OFFSET_Y) * shop_location.at(i));

    health_location.push_back(transform2D::Translate(resolution.x / 2 - SHOP_STARTING_X - 2 * HEALTH_OFFSET, resolution.y / 2 - SHOP_STARTING_Y)); // Pentru vizualizare pe windows fara /2
    health_location.push_back(transform2D::Translate(resolution.x / 2 - SHOP_STARTING_X - HEALTH_OFFSET, resolution.y / 2 - SHOP_STARTING_Y)); // Pentru vizualizare pe windows fara /2
    health_location.push_back(transform2D::Translate(resolution.x / 2 - SHOP_STARTING_X, resolution.y / 2 - SHOP_STARTING_Y)); // Pentru vizualizare pe windows fara /2


    Mesh* red_sq = object2D::CreateSq("red_sq", SLOT_SIZE / 2, glm::vec3(170 / 255.0, 34 / 255.0, 25 / 255.0), true);
    AddMeshToList(red_sq);
    Mesh* grid_square = object2D::CreateSq("grid_square", SLOT_SIZE, TURRET_SLOT_COLOR, true);
    AddMeshToList(grid_square);
    Mesh* shop_square = object2D::CreateSq("shop_square", SLOT_SIZE, glm::vec3(0, 0, 0), false);
    AddMeshToList(shop_square);

    Mesh* orange_turret = object2D::CreateTurret("orange_turret", SLOT_SIZE / 2, ENEMY_ORANGE_COLOR, 1);
    AddMeshToList(orange_turret);
    Mesh* blue_turret = object2D::CreateTurret("blue_turret", SLOT_SIZE / 2, ENEMY_BLUE_COLOR, 1);
    AddMeshToList(blue_turret);
    Mesh* yellow_turret = object2D::CreateTurret("yellow_turret", SLOT_SIZE / 2, ENEMY_YELLOW_COLOR, 1);
    AddMeshToList(yellow_turret);
    Mesh* purple_turret = object2D::CreateTurret("purple_turret", SLOT_SIZE / 2, ENEMY_PURPLE_COLOR, 1);
    AddMeshToList(purple_turret);

    Mesh* orange_star = object2D::CreateStar("orange_star", BULLET_RADIUS, 1, ENEMY_ORANGE_COLOR);
    AddMeshToList(orange_star);
    Mesh* blue_star = object2D::CreateStar("blue_star", BULLET_RADIUS, 1, ENEMY_BLUE_COLOR);
    AddMeshToList(blue_star);
    Mesh* yellow_star = object2D::CreateStar("yellow_star", BULLET_RADIUS, 1, ENEMY_YELLOW_COLOR);
    AddMeshToList(yellow_star);
    Mesh* purple_star = object2D::CreateStar("purple_star", BULLET_RADIUS, 1, ENEMY_PURPLE_COLOR);
    AddMeshToList(purple_star);
    Mesh* gray_star = object2D::CreateStar("gray_star", SLOT_SIZE / 5, 0, glm::vec3(.5, .5, .5));
    AddMeshToList(gray_star);
    Mesh* pink_star = object2D::CreateStar("pink_star", SLOT_SIZE / 3, 4, CURRENCY_COLOR);
    AddMeshToList(pink_star);

    Mesh* orange_hex = object2D::CreateHex("orange_hex", ENEMY_RADIUS, ENEMY_ORANGE_COLOR, 2);
    AddMeshToList(orange_hex);
    Mesh* blue_hex = object2D::CreateHex("blue_hex", ENEMY_RADIUS, ENEMY_BLUE_COLOR, 2);
    AddMeshToList(blue_hex);
    Mesh* yellow_hex = object2D::CreateHex("yellow_hex", ENEMY_RADIUS, ENEMY_YELLOW_COLOR, 2);
    AddMeshToList(yellow_hex);
    Mesh* purple_hex = object2D::CreateHex("purple_hex", ENEMY_RADIUS, ENEMY_PURPLE_COLOR, 2);
    AddMeshToList(purple_hex);
}


bool Tema1::hasEnemyOfType(int line, int type) {
    for(auto i = enemies[line].begin(); i != enemies[line].end(); ++i)
        if((*i).type == type)
            return true;
    return false;
}


void Tema1::addTurret(int slot, int type) {
    if(starCount >= prices[type]) {
        slots.at(slot).isOccupied = true;
        slots.at(slot).shootTimer = BULLET_SHOOT_RATE;
        slots.at(slot).scaleFactor = 1;
        slots.at(slot).turretType = type;
        starCount -= prices[type];
    }
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(.23, .23, .23, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    if (health <= 0)
        exit(1);

    glm::ivec2 resolution = window->GetResolution();
    enemySpawnTimer -= deltaTimeSeconds;
    currencySpawnTimer -= deltaTimeSeconds;

    // Spawn more enemies
    if(enemySpawnTimer < 0) {
        int line = rand() % 3;
        int type = rand() % 4;
        enemies[line].push_back({(float)(resolution.x / 2 + ENEMY_SPAWN_OFFSET), (float)(GRID_STARTING_Y + line * GRID_OFFSET), 1, 3, type}); // Pentru windows fara *2 si /2
        enemySpawnTimer = ENEMY_SPAWN_RATE;
    }
    
    // Spawn more currency
    if(currencySpawnTimer < 0) {
        int count = rand() % 2 + 3;
        for (int i = 0; i < count; ++i)
            spawned_currency.push_back({(float)(rand() % (resolution.x / 2)), (float)(rand() % (resolution.y / 2))}); // Pentru windows fara *2 si /2
        currencySpawnTimer = CURRENCY_SPAWN_RATE;
    }

    // Spawn more bullets
    for(int i = 0; i < 9; ++i) {
        if(slots.at(i).isOccupied && hasEnemyOfType(i / 3, slots.at(i).turretType)) {
            slots.at(i).shootTimer -= deltaTimeSeconds;
            if(slots.at(i).shootTimer < 0 && slots.at(i).scaleFactor == 1) {
                bullets[i / 3].push_back({slots.at(i).posX + BULLET_SPAWN_OFFSET, slots.at(i).posY, 0, slots.at(i).turretType});
                slots.at(i).shootTimer = BULLET_SHOOT_RATE;
            }
        }
    }

    glm::mat3 mouseMat = transform2D::Translate(mouseposX, mouseposY);

    // Calculate turrets scaling
    for(int i = 0; i < 9; ++i) {
        if(slots.at(i).isOccupied) {
            if(slots.at(i).scaleFactor < 1) {
                if (slots.at(i).scaleFactor - deltaTimeSeconds > 0)
                    slots.at(i).scaleFactor -= deltaTimeSeconds;
                else
                    slots.at(i).isOccupied = false;
            }
        }
    }

    // Calculate enemy scaling
    for(int i = 0; i < 3; ++i) {
        for(auto j = enemies[i].begin(); j != enemies[i].end(); ++j) {
            if((*j).scaleFactor < 1) {
                if((*j).scaleFactor - deltaTimeSeconds > 0)
                    (*j).scaleFactor -= deltaTimeSeconds;
                else {
                    enemies[i].erase(j);
                    --j;
                }
            }
        }
    }

    // Calculate collisions
    for(int i = 0; i < 3; ++i) {
        for(auto k = enemies[i].begin(); k != enemies[i].end(); ++k) {
            // Enemy-bullet collision
            for(auto j = bullets[i].begin(); j != bullets[i].end(); ++j) {
                if(abs((*k).posX - (*j).posX) <= BULLET_RADIUS + ENEMY_RADIUS && (*k).type == (*j).type) {
                    if((*k).scaleFactor == 1) {
                        bullets[i].erase(j);
                        j--;
                    }
                    if((*k).health == 1)
                        (*k).scaleFactor -= 0.0001;
                    (*k).health--;
                }
            }
            // Enemy-turret collision
            for(int j = 3 * i; j < (i + 1) * 3; ++j)
                if(abs((*k).posX - slots.at(j).posX) <= SLOT_SIZE / 2 + ENEMY_RADIUS)
                    slots.at(j).scaleFactor -= deltaTimeSeconds;
        }
    }

    // Calculate enemy positions
    for(int i = 0; i < 3; ++i) {
        for(auto j = enemies[i].begin(); j != enemies[i].end(); ++j) {
            if((*j).scaleFactor == 1)
                (*j).posX -= ENEMY_SPEED * deltaTimeSeconds;
            if((*j).posX <= DEATH_OFFSET_X) {
                enemies[i].erase(j);
                j--;
                health -= 1;
                health_location.pop_back();
            }
        }
    }

    // Calculate bullets position and rotation
    for(int i = 0; i < 3; ++i) {
        for(auto j = bullets[i].begin(); j != bullets[i].end(); ++j) {
            if((*j).posX > resolution.x / 2) {
                bullets[i].erase(j);
                j--;
            }
            else {
                (*j).posX += BULLET_SPEED * deltaTimeSeconds;
                (*j).rotation -= ROTATION_SPEED * deltaTimeSeconds;
            }
        }
    }

    // Grid rendering
    for(int i = 0; i < 9; ++i)
        RenderMesh2D(meshes["grid_square"], shaders["VertexColor"], grid_location.at(i));
    RenderMesh2D(meshes["red_sq"], shaders["VertexColor"], grid_location.at(9));
    
    // Shop rendering
    for(int i = 0; i < 4; ++i) {
        RenderMesh2D(meshes["shop_square"], shaders["VertexColor"], shop_location.at(i));
        RenderMesh2D(meshes[typesByInt[i] + "_turret"], shaders["VertexColor"], shop_location.at(i));
    }
    for(auto i = shops.begin(); i != shops.end(); ++i)
        for(int j = 0; j < (*i).starNo; ++j)
            RenderMesh2D(meshes["gray_star"], shaders["VertexColor"], (*i).stars.at(j));

    // Render health
    for(auto i = health_location.begin(); i != health_location.end(); ++i)
        RenderMesh2D(meshes["red_sq"], shaders["VertexColor"], (*i));

    // Render turret at mouse if needed
    if(hasTurret)
        RenderMesh2D(meshes[typesByInt[hasTurretType] + "_turret"], shaders["VertexColor"], mouseMat);

    // Render turrets
    for(int i = 0; i < 9; ++i)
        if(slots.at(i).isOccupied)
            RenderMesh2D(meshes[typesByInt[slots.at(i).turretType] + "_turret"], shaders["VertexColor"], grid_location.at(i) * transform2D::Scale(slots.at(i).scaleFactor, slots.at(i).scaleFactor));

    // Render bullets
    for(int i = 0; i < 3; ++i)
        for(auto j = bullets[i].begin(); j != bullets[i].end(); ++j)
            RenderMesh2D(meshes[typesByInt[(*j).type] + "_star"], shaders["VertexColor"], transform2D::Translate((*j).posX, (*j).posY) * transform2D::Rotate((*j).rotation));

    // Render enemies
    for(int i = 0; i < 3; ++i)
        for(auto j = enemies[i].begin(); j != enemies[i].end(); ++j)
            RenderMesh2D(meshes[typesByInt[(*j).type] + "_hex"], shaders["VertexColor"], transform2D::Translate((*j).posX, (*j).posY) * transform2D::Scale((*j).scaleFactor, (*j).scaleFactor));

    // Render spawned currency
    for(auto i = spawned_currency.begin(); i != spawned_currency.end(); ++i)
        RenderMesh2D(meshes["pink_star"], shaders["VertexColor"], transform2D::Translate((*i).posX, (*i).posY));

    // Render currency
    for(auto i = 0; i < starCount && i < 24; ++i)
        RenderMesh2D(meshes["gray_star"], shaders["VertexColor"], transform2D::Translate(resolution.x / 2 - CURRENCY_OFFSET_X + (i % 6) * (SLOT_SIZE / 2.5), resolution.y / 2 - CURRENCY_OFFSET_Y - (100 * (i / 6))));
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    glm::ivec2 res = window->GetResolution();
    // Pentru windows fara *2 si /2
    mouseposX = mouseX * 2;
    mouseposY = res.y / 2 - mouseY * 2;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if(IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        glm::ivec2 res = window->GetResolution();
        // Pentru windows fara *2 si /2
        int mouseX_r = mouseX * 2;
        int mouseY_r = res.y / 2 - mouseY * 2;
        for(auto i = spawned_currency.begin(); i != spawned_currency.end(); ++i) {
            if(sqrt((mouseX_r - (*i).posX) * (mouseX_r - (*i).posX) + (mouseY_r - (*i).posY) * (mouseY_r - (*i).posY)) <= (float)CURRENCY_RADIUS) {
                starCount++;
                spawned_currency.erase(i);
                i--;
            }
        }

        for(int i = 0; i < 4; ++i) {
            if(mouseX_r <= shops.at(i).posX + SLOT_SIZE / 2 && mouseX_r >= shops.at(i).posX - SLOT_SIZE / 2
                && mouseY_r <= shops.at(i).posY + SLOT_SIZE / 2 && mouseY_r >= shops.at(i).posY - SLOT_SIZE / 2) {
                hasTurret = true;
                hasTurretType = i;
            }
        }
    }

    if(IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        glm::ivec2 res = window->GetResolution();
        // Pentru windows fara *2 si /2
        int mouseX_r = mouseX * 2;
        int mouseY_r = res.y / 2 - mouseY * 2;
        for(int i = 0; i < 9; ++i) {
            if(mouseX_r <= slots.at(i).posX + SLOT_SIZE / 2 && mouseX_r >= slots.at(i).posX - SLOT_SIZE / 2
                && mouseY_r <= slots.at(i).posY + SLOT_SIZE / 2 && mouseY_r >= slots.at(i).posY - SLOT_SIZE / 2) {
                // slots.at(i).isOccupied = false;
                slots.at(i).scaleFactor -= 0.0001;
            }
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if(IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        glm::ivec2 res = window->GetResolution();
        // Pentru windows fara *2 si /2
        if(hasTurret) {
            int mouseX_r = mouseX * 2;
            int mouseY_r = res.y / 2 - mouseY * 2;
            for(int i = 0; i < 9; ++i) {
                if(mouseX_r <= slots.at(i).posX + SLOT_SIZE / 2 && mouseX_r >= slots.at(i).posX - SLOT_SIZE / 2
                    && mouseY_r <= slots.at(i).posY + SLOT_SIZE / 2 && mouseY_r >= slots.at(i).posY - SLOT_SIZE / 2
                    && !slots.at(i).isOccupied) {
                    addTurret(i, hasTurretType);
                }
            }
            hasTurret = false;
        }
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
