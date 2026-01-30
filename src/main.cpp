#include <iostream>
#include <memory>
#include <cstdint>
#include <chrono>
#include "sys/render.hpp"
#include "sys/physics.hpp"
#include "sys/collision.hpp"
#include "sys/input.hpp"
#include "sys/spawn.hpp"
#include "sys/health.hpp"
#include "man/entitymanager.hpp"
#include "util/factory.hpp"

constexpr uint32_t KWIDTH = 640;
constexpr uint32_t KHEIGHT = 360;
constexpr uint32_t FPS = 30;
constexpr uint32_t MFPS = 1000/FPS; // Los MS que dura un FPS

int main(){
   EntityManager EntityMan;
   Factory factory(EntityMan);

   factory.createPlyer(40, 40, "pngs/klipartz4.png");
   factory.createBlade(40, 250, 1, -1);
   factory.createSpawner(500, 200, 
      [&factory](uint32_t x, uint32_t y, int32_t vx, int32_t vy){
         factory.createBlade(x,y,vx,vy);
   });
   factory.createPlatform(300, 300);
   RenderSys Render{KWIDTH, KHEIGHT};
   PhysicsSys Physics;
   CollisionSys Collision(KWIDTH, KHEIGHT);
   InputSys Input;
   SpawnSys Spawn;
   HealthSys Health;
   Render.setDebugDraw(true);

   while( !Input.isKeyPressed(XK_Escape))
   {
      Input.update(EntityMan);
      Physics.update(EntityMan);
      Collision.update(EntityMan);
      Health.update(EntityMan);
      Render.update(EntityMan);
      Spawn.update(EntityMan);
   }


   return 0;
}