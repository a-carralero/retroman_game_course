#include <iostream>
#include <memory>
#include <cstdint>
#include "sys/render.hpp"
#include "sys/physics.hpp"
#include "sys/collision.hpp"
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include "cmp/render.hpp"

constexpr uint32_t KWIDTH = 640;
constexpr uint32_t KHEIGHT = 360;

void createPlyer ( EntityManager& em,  
                   uint32_t x, uint32_t y, 
                   const std::string_view filename)
{
   Entity& e = em.createEntity();
   auto& ph = em.createComponent<PhysicsCmp>(e.getEntityID());
   ph.x = x; ph.y = y;
   ph.vx = 1; ph.vy = 1;
   e.linkComponent<PhysicsCmp>(ph);
   auto& rn = em.createComponent<RenderCmp>(e.getEntityID());
   rn.loadFromFile(filename);
   e.linkComponent<RenderCmp>(rn);
}



int main(){
   EntityManager EntityMan;
   createPlyer(EntityMan, 40, 40, "pngs/klipartz4.png");
   RenderSystem Render{KWIDTH, KHEIGHT};
   PhysicsSystem Physics;
   CollisionSystem Collision;

   do{
      Physics.update(EntityMan);
      Collision.update(EntityMan);
   } while (Render.update(EntityMan));

   return 0;
}