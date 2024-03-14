#pragma once

#include <map>
#include <random>
#include "components/simple_scene.h"


namespace m1
{
   class Tema1 : public gfxc::SimpleScene
   {
      public:
         Tema1();
         ~Tema1();

         void Init() override;

      private:
         void FrameStart() override;
         void Update(float deltaTimeSeconds) override;
         void FrameEnd() override;

         void OnInputUpdate(float deltaTime, int mods) override;
         void OnKeyPress(int key, int mods) override;
         void OnKeyRelease(int key, int mods) override;
         void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
         void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
         void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
         void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
         void OnWindowResize(int width, int height) override;

         bool hasEnemyOfType(int line, int type);
         void addTurret (int slot, int type);

      protected:
         typedef struct {
            bool isOccupied;
            float posX;
            float posY;
            float shootTimer;
            float scaleFactor;
            int turretType;
         } TurretSlot;

         typedef struct {
            float posX;
            float posY;
            float starNo;
            std::vector<glm::mat3> stars;
         } ShopSlot;

         typedef struct {
            float posX;
            float posY;
            float scaleFactor;
            int health;
            int type;
         } Enemy;

         typedef struct {
            float posX;
            float posY;
            float rotation;
            int type;
         } Bullet;

         typedef struct {
            float posX;
            float posY;
         } Currency;

         int health;
         int starCount;
         float enemySpawnTimer;
         float currencySpawnTimer;

         bool hasTurret;
         int hasTurretType;
         float mouseposX;
         float mouseposY;

         std::map<int, std::string> typesByInt;
         std::map<int, int> prices;

         std::vector<glm::mat3> grid_location;
         std::vector<glm::mat3> shop_location;
         std::vector<glm::mat3> health_location;

         std::vector<TurretSlot> slots;
         std::vector<ShopSlot> shops;

         std::vector<Enemy> enemies[3];
         std::vector<Bullet> bullets[3];
         std::vector<Currency> spawned_currency;
   };
}   // namespace m1
