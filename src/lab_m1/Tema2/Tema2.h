#pragma once

#include <random>
#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
#include "lab_m1/Tema2/transform3D.h"


namespace m1
{
   class Tema2 : public gfxc::SimpleScene
   {
      public:
         Tema2();
         ~Tema2();

         void Init() override;

      private:
         void FrameStart() override;
         void Update(float deltaTimeSeconds) override;
         void FrameEnd() override;

         void LoadMeshes();
         void InitShader();
         void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color, int tankHealth);
         void RenderScene();

         void CollisionDetection(float deltaTime);
         void EnemyMovement(float deltaTime);
         void ProjectileMovement(float deltaTime);

         void OnInputUpdate(float deltaTime, int mods) override;
         void OnKeyPress(int key, int mods) override;
         void OnKeyRelease(int key, int mods) override;
         void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
         void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
         void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
         void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
         void OnWindowResize(int width, int height) override;

      protected:
         m1::Camera *camera;
         glm::mat4 projectionMatrix;

         typedef struct {
            int health;
            float tankRotation;
            float turretRotation;
            float shootTimer;
            glm::vec3 pos;
            glm::vec3 forward;
            glm::vec3 turretForward;
            int state;
            float stateTimer;
            bool locked;
         } Tank;

         typedef struct {
            glm::vec3 pos;
            int sizeX, sizeY, sizeZ;
         } Building;

         typedef struct {
            glm::vec3 pos;
            glm::vec3 forward;
            float timer;
         } Projectile;

         void InitTank();
         void TankShoot(Tank* tank);
         void MoveTank(Tank* tank, glm::vec3 amount);
         void RotateTank(Tank* tank, float angle);
         void CollisionTank(Tank* tank1, Tank* tank2);
         void CollisionBuilding(Tank* tank, Building* building, float deltaTime);
         void CollisionBound(Tank* tank, float deltaTime);

         void InitBuildings();
         void InitEnemies();

         float gameTime;
         int killedEnemies = 0;
         bool gameFinished = false;

         std::vector<Building> buildings;
         std::vector<Tank> enemies;
         std::vector<Projectile> projectiles;

         Tank tank;
      };
}  // namespace m1
