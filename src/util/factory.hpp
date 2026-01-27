#pragma once
#include <cstdint>
#include <string_view>
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include "cmp/render.hpp"
#include "cmp/input.hpp"
#include "cmp/collider.hpp"
#include "cmp/spawner.hpp"
#include "cmp/health.hpp"

struct EntityManager;

struct Factory{
 private:
   EntityManager& em;
 public:
   Factory(EntityManager& eman) : em(eman) {}

   void createPlyer (uint32_t x, uint32_t y, 
             const std::string_view filename);

   void createBlade(uint32_t x, uint32_t y,
                    int32_t vx, int32_t vy);
  
   template<typename Callback_t>
   void createSpawner(uint32_t x, uint32_t y, Callback_t cb)
   {
      Entity& e = em.createEntity();
      auto& rn  = em.addComponent<RenderCmp>(e);
      auto& ph  = em.addComponent<PhysicsCmp>(e);
      auto& cl  = em.addComponent<ColliderCmp>(e);
      auto& spw = em.addComponent<SpawnerCmp>(e);
      
      rn.loadFromFile("pngs/spawner.png");
      ph.x = x; ph.y = y;
      ph.vx = 0; ph.vy = 1;
      cl.boxRoot.box.xR = rn.w;
      cl.boxRoot.box.yD = rn.h;
      cl.mask = 0x00; // Collide with nothing!
      spw.to_be_spawned = 2;
      spw.spawnMethod = cb;
   }

};