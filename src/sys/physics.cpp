#include "sys/physics.hpp"
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include <algorithm>

void PhysicsSys::update(EntityManager& g) const {

   for(auto& phy: g.getComponents<PhysicsCmp>()){

      phy.vy = phy.vy + phy.g + phy.a;
      phy.vy = std::clamp(phy.vy, -phy.vmax, phy.vmax);

      phy.x = phy.x + phy.vx;
      phy.y = phy.y + phy.vy;

      if (phy.vy < -1){
         std::cout << "vy = " << phy.vy << "\n";
      }
      // sólo queremos que se active durante un frame para el salto
      phy.a = 0;  
   }
}