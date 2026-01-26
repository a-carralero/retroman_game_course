#include <iostream>
#include <memory>
#include <cstdint>
#include "sys/render.hpp"
#include "sys/physics.hpp"
#include "sys/collision.hpp"
#include "sys/input.hpp"
#include "sys/spawn.hpp"
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include "cmp/render.hpp"
#include "cmp/input.hpp"
#include "cmp/collider.hpp"
#include "cmp/spawner.hpp"

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
   auto& cl = em.createComponent<ColliderCmp>(eid);
   cl.box.xL = 10;
   cl.box.yU = 10;
   cl.box.xR = rn.w - 10;
   cl.box.yD = rn.h - 10;
   e.linkComponent<ColliderCmp>(cl);
}

void createBlade(EntityManager& em, uint32_t x, uint32_t y,
                                    int32_t vx, int32_t vy)
{
   Entity& e = em.createEntity();
   uint32_t eid = e.getEntityID();
   auto& ph = em.createComponent<PhysicsCmp>(eid);
   ph.x = x; ph.y = y; 
   ph.vx = vx; ph.vy = vy;
   e.linkComponent<PhysicsCmp>(ph);
   auto& rn = em.createComponent<RenderCmp>(eid);
   rn.loadFromFile("pngs/blade.png");
   e.linkComponent<RenderCmp>(rn);
   auto& cl = em.createComponent<ColliderCmp>(eid);
   cl.box.xL = 10;
   cl.box.yU = 10;
   cl.box.xR = rn.w - 10;
   cl.box.yD = rn.h + 10;
   e.linkComponent<ColliderCmp>(cl);
}

void createSpawner( EntityManager& em,  
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
   auto& spw = em.createComponent<SpawnerCmp>(eid);
   e.linkComponent<SpawnerCmp>(spw);
}


int main(){
   EntityManager EntityMan;
   createPlyer(EntityMan, 40, 40, "pngs/klipartz4.png");
   // createBlade(EntityMan, 40, 250, 1, -1, "pngs/blade.png");
   createSpawner(EntityMan, 500, 200, "pngs/spawner.png");
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