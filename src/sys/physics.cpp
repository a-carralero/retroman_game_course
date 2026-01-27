#include "sys/physics.hpp"
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"

bool PhysicsSys::update(EntityManager& g) const {

   for(auto& phy: g.getComponents<PhysicsCmp>()){
      phy.x += phy.vx;
      phy.y += phy.vy;
   }
   return true;
}