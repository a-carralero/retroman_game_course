#pragma once
#include <cstdint>
#include <string_view>
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include "cmp/render.hpp"
#include "cmp/input.hpp"
#include "cmp/collider.hpp"
#include "cmp/spawner.hpp"

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
      auto& ph = em.addComponent<PhysicsCmp>(e);
      ph.x = x; ph.y = y;
      ph.vx = 0; ph.vy = 1;
      auto& rn = em.addComponent<RenderCmp>(e);
      rn.loadFromFile("pngs/spawner.png");
      auto& spw = em.addComponent<SpawnerCmp>(e);
      spw.to_be_spawned = 2;
      spw.spawnMethod = cb;
   }

};