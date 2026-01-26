#include <iostream>
#include <memory>
#include <cstdint>
#include "sys/render.hpp"
#include "sys/physics.hpp"
#include "sys/collision.hpp"
#include "sys/input.hpp"
#include "sys/spawn.hpp"
#include "man/entitymanager.hpp"
#include "util/factory.hpp"

constexpr uint32_t KWIDTH = 640;
constexpr uint32_t KHEIGHT = 360;



int main(){
   EntityManager EntityMan;
   Factory factory(EntityMan);

   factory.createPlyer(40, 40, "pngs/klipartz4.png");
   // createBlade(EntityMan, 40, 250, 1, -1, "pngs/blade.png");
   factory.createSpawner(500, 200, 
      [&factory](uint32_t x, uint32_t y, int32_t vx, int32_t vy){
         factory.createBlade(x,y,vx,vy);
   });
   RenderSystem Render{KWIDTH, KHEIGHT};
   PhysicsSystem Physics;
   CollisionSystem Collision(KWIDTH, KHEIGHT);
   InputSystem Input;
   SpawnSystem Spawn;

   while(Input.update(EntityMan)){
      Physics.update(EntityMan);
      Collision.update(EntityMan);
      Render.update(EntityMan);
      Spawn.update(EntityMan);
   }


   return 0;
}