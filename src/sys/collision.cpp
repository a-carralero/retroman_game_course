#include "sys/collision.hpp"
#include "man/entitymanager.hpp"
#include "cmp/collider.hpp"
#include "cmp/physics.hpp"

bool CollisionSystem::update(EntityManager& g) const 
{
   for (auto& cl: g.getComponents<ColliderCmp>())
   {
      auto* phy = g.getRequiredCmpFromCmp<PhysicsCmp>(cl);
      if (!phy) continue;

      uint32_t xl = phy->x + cl.box.xL;
      uint32_t xr = phy->x + cl.box.xR;
      uint32_t yu = phy->y + cl.box.yU;
      uint32_t yd = phy->y + cl.box.yD;

      // left collision
      if (xl > m_w || xr > m_w) {
         phy-> x -= phy->vx;
         phy->vx = -phy->vx;
      }
      if (yu > m_h || yd > m_h) {
         phy-> y -= phy->vy;
         phy->vy = -phy->vy;
      }
   }

   return true;
}