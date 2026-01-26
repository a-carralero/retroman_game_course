#include "sys/spawn.hpp"
#include "man/entitymanager.hpp"
#include "cmp/spawner.hpp"
#include "cmp/physics.hpp"
#include <iostream>

void SpawnSystem::update(EntityManager& g) const 
{
   using namespace std::chrono;
   auto now = steady_clock::now();
   for (auto& spw: g.getComponents<SpawnerCmp>()) 
   {
      auto *phy = g.getRequiredCmpFromCmp<PhysicsCmp>(spw);
      if (!phy) continue;

      if (spw.to_be_spawned > 0) {
         auto passed = now - spw.last_spawn_time;
         if (passed > spw.spawn_interval){
            spw.spawnMethod(phy->x, phy->y, -1, 0);
            spw.last_spawn_time = now;
            --spw.to_be_spawned;
         }
      }
   }
}