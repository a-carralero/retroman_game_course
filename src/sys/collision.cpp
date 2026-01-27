#include "sys/collision.hpp"
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include <iostream>


bool CollisionSys::update(EntityManager& g) const 
{
   auto& collider_vec = g.getComponents<ColliderCmp>();

   for(auto&c : collider_vec) setFalseCollideds(c.boxRoot);

   std::size_t size = collider_vec.size();
   for (std::size_t i=0; i<size; ++i)
   {
      auto& cl = collider_vec[i];
      auto* phy = g.getRequiredCmpFromCmp<PhysicsCmp>(cl);
      if (!phy) continue;
      checkBoundaries(cl, *phy);

      for (std::size_t j=i+1; j<size; ++j){
         auto& cl2 = collider_vec[j];
         if ((cl.mask & cl2.mask) == 0) continue;
         auto* phy2 = g.getRequiredCmpFromCmp<PhysicsCmp>(cl2);
         if (!phy) continue;
         checkObjCollision(cl.boxRoot, *phy, cl2.boxRoot, *phy2);
      }
   }
   return true;
}

void 
CollisionSys::checkBoundaries(const ColliderCmp& cl, 
                                    PhysicsCmp& phy) const
{
   auto [xl, xr, yu, yd] = move2ScreenFrame(cl.boxRoot.box, phy.x, phy.y);
   if (xl > m_w || xr > m_w) {
      phy. x -=  phy.vx;
      phy.vx  = -phy.vx;
   }
   if (yu > m_h || yd > m_h) {
      phy. y -=  phy.vy;
      phy.vy  = -phy.vy;
   }
}


BoundingBox
CollisionSys::move2ScreenFrame(const BoundingBox& box, 
                               uint32_t x, uint32_t y) const
{
   return BoundingBox {
      x + box.xL,
      x + box.xR,
      y + box.yU,
      y + box.yD,
   };
}

bool 
CollisionSys::checkObjCollision(BoundingBoxNode& bn1, PhysicsCmp& phy1,
                                BoundingBoxNode& bn2, PhysicsCmp& phy2) const
{
   BoundingBox b1 = move2ScreenFrame(bn1.box, phy1.x, phy1.y);
   BoundingBox b2 = move2ScreenFrame(bn2.box, phy2.x, phy2.y);

   // Check generic intervals
   auto checkIntervals = [](uint32_t L1, uint32_t R1, uint32_t L2, uint32_t R2)
   {
      if (L1 > R2 || L2 > R1)
         return false;
      else 
         return true;
   };
   
   // Check collision in both axes
   if (checkIntervals(b1.xL, b1.xR, b2.xL, b2.xR) ){    // evaluamos choque horizontal
      if (checkIntervals(b1.yU, b1.yD, b2.yU, b2.yD))   // evaluamos choque vertical
      { 
         bn1.collided = true;
         bn2.collided = true;
         // BN1 BN2
         // 1) Los dos sean hoja
         // 2) BN1 tenga hijos
         // 3) BN2 tenga hijos
         // 4) BN1 y BN2 tengan hijos

         
         if (!bn1.childs.empty()) 
         {
            for(auto& b: bn1.childs)
               checkObjCollision(b, phy1, bn2, phy2);
         }
         else if (!bn2.childs.empty())
         {
            for(auto& b: bn2.childs)
               checkObjCollision(bn1, phy1, b, phy2);
         }
         else // activamos collided = true cuando hemos encontrado los nodos hoja
         {
            // bn1.collided = true;
            // bn2.collided = true;
         }
      }
   }
   return false;
}

void 
CollisionSys::setFalseCollideds(BoundingBoxNode& bn) const
{
   bn.collided = false;
   for (auto& child : bn.childs)
      setFalseCollideds(child);
}