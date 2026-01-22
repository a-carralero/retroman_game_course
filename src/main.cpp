#include <iostream>
#include <memory>
#include <cstdint>
#include "sys/render.hpp"
#include "sys/physics.hpp"
#include "sys/collision.hpp"
#include "man/entitymanager.hpp"

constexpr uint32_t KWIDTH = 640;
constexpr uint32_t KHEIGHT = 360;


int main(){
   EntityManager EntityMan;
   EntityMan.createEntity(40,40, "pngs/klipartz4.png");
   RenderSystem Render{KWIDTH, KHEIGHT};
   PhysicsSystem Physics;
   CollisionSystem Collision;

   do{
      Physics.update(EntityMan);
      Collision.update(EntityMan);
   } while (Render.update(EntityMan));

   return 0;
}