#include "sys/physics.hpp"
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include <algorithm>

void PhysicsSys::update(EntityManager& g) const 
{
   for(auto& phy: g.getComponents<PhysicsCmp>())
   {
      // Verify if we are still on Platform
      if (std::abs(phy.vy) > phy.KMinVyForNotOnPlatform)
         phy.onPlatform = false;

      // Jump
      if (phy.jumpIdx < phy.jumpTable.size()){
         phy.vy = phy.jumpTable[phy.jumpIdx];
         phy.jumpIdx++;
      }
      // Gravity
      phy.vy += phy.gravity;
      phy.vy = std::clamp(phy.vy, phy.KMinVy, phy.KMaxVy);

      // X Acceleration
      phy.vx += phy.ax;
      phy.vx = std::clamp(phy.vx, phy.KMinVx, phy.KMaxVx);
      phy.x += phy.vx;
      phy.y += phy.vy;

      // if (phy.gravity > 0 && phy.onPlatform){
      //    std::cout << "Physics: vx = " << phy.vx << "\n";
      // }
      // if (phy.gravity > 0)
      //    std::cout << "ax = " << phy.ax << "\n";
   }
}