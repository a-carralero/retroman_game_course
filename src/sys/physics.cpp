#include "sys/physics.hpp"
#include "util/gamecontext.hpp"

bool PhysicsSystem::update(GameContext& g) const {

   for(auto& phy: g.getPhysicsComponents()){
      phy.x += phy.vx;
      phy.y += phy.vy;
   }
   return true;
}