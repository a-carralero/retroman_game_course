#include <iostream>
#include <memory>
#include <cstdint>
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

int main(){
   EntityManager EntityMan;
   Factory factory(EntityMan);

   factory.createPlyer(40, 40, "pngs/klipartz4.png");
   factory.createBlade(40, 250, 1, -1);
   factory.createSpawner(500, 200, 
      [&factory](uint32_t x, uint32_t y, int32_t vx, int32_t vy){
         factory.createBlade(x,y,vx,vy);
   });
   RenderSys Render{KWIDTH, KHEIGHT};
   PhysicsSys Physics;
   CollisionSys Collision(KWIDTH, KHEIGHT);
   InputSys Input;
   SpawnSys Spawn;
   HealthSys Health;
   Render.setDebugDraw(true);

   while(Input.update(EntityMan)){
      Physics.update(EntityMan);
      Collision.update(EntityMan);
      Health.update(EntityMan);
      Render.update(EntityMan);
      Spawn.update(EntityMan);
   }


   return 0;
}