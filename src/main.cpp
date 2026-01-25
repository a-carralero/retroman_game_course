#include <iostream>
#include <memory>
#include <cstdint>
#include "sys/render.hpp"
#include "sys/physics.hpp"
#include "sys/collision.hpp"
#include "sys/input.hpp"
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include "cmp/render.hpp"
#include "cmp/input.hpp"

constexpr uint32_t KWIDTH = 640;
constexpr uint32_t KHEIGHT = 360;

void createPlyer ( EntityManager& em,  
                   uint32_t x, uint32_t y, 
                   const std::string_view filename)
{
   Entity& e = em.createEntity();
   uint32_t eid = e.getEntityID();
   auto& ph = em.createComponent<PhysicsCmp>(eid);
   ph.x = x; ph.y = y;
   ph.vx = 0; ph.vy = 0;
   e.linkComponent<PhysicsCmp>(ph);
   auto& rn = em.createComponent<RenderCmp>(eid);
   rn.loadFromFile(filename);
   e.linkComponent<RenderCmp>(rn);
   auto& inp = em.createComponent<InputCmp>(eid);
   e.linkComponent<InputCmp>(inp);
}



int main(){
   EntityManager EntityMan;
   createPlyer(EntityMan, 40, 40, "pngs/klipartz4.png");
   RenderSystem Render{KWIDTH, KHEIGHT};
   PhysicsSystem Physics;
   CollisionSystem Collision;
   InputSystem Input;

   while(Input.update(EntityMan)){
      Physics.update(EntityMan);
      Collision.update(EntityMan);
      Render.update(EntityMan);
   }


   return 0;
}