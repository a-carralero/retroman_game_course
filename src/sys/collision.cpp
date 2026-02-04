#include "sys/collision.hpp"
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include "cmp/health.hpp"
#include <cmath>
#include <iostream>


void CollisionSys::update(EntityManager& g) const 
{
   auto& collider_vec = g.getComponents<ColliderCmp>();

   for(auto&c : collider_vec) setFalseCollideds(c.boxRoot);

   std::size_t size = collider_vec.size();
   for (std::size_t i=0; i<size; ++i)
   {
      auto& cl = collider_vec[i];
      auto* phy = g.getRequiredCmpFromCmp<PhysicsCmp>(cl);
      if (!phy) continue;
      if (cl.mask & ColliderCmp::L_Boundaries)
         checkBoundaries(cl, *phy);

      for (std::size_t j=i+1; j<size; ++j){
         auto& cl2 = collider_vec[j];
         if ((cl.mask & cl2.mask) == 0) continue;
         auto* phy2 = g.getRequiredCmpFromCmp<PhysicsCmp>(cl2);
         if (!phy) continue;
         if (checkObjCollision(cl.boxRoot, *phy, cl2.boxRoot, *phy2)){
            react2Collision(g, cl, cl2);
         }
      }

      // if (phy->gravity > 0){
      //    std::cout << "Collision: vx = " << phy->vx << "\n";
      // }
   }
   
}

void 
CollisionSys::checkBoundaries(const ColliderCmp& cl, 
                                    PhysicsCmp& phy) const
{
   auto [xl, xr, yu, yd] = move2WorldFrame(cl.boxRoot.box, phy.x, phy.y);
   if (xl > m_w || xr < 0) {
      phy. x -=  phy.vx;
      phy.vx  = -phy.vx;
   }
   if (yu > m_h || yd < 0) {
      phy. y -=  phy.vy; // recuperar la posición de antes de que nos saliésemos de la pantalla
      if (phy.gravity == 0)
         phy.vy  = -phy.vy;
      else
         phy.vy = 0;
   }
}


BoundingBox<float> CollisionSys::
move2WorldFrame(const BoundingBox<uint32_t>& box, float x, float y) const
{
   return BoundingBox<float> {
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
   BoundingBox<float> b1 = move2WorldFrame(bn1.box, phy1.x, phy1.y);
   BoundingBox<float> b2 = move2WorldFrame(bn2.box, phy2.x, phy2.y);

   // Check generic intervals
   auto checkIntervals = [](float L1, float R1, float L2, float R2)
   {
      if (L1 >= R2 || L2 >= R1)
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
               if (checkObjCollision(b, phy1, bn2, phy2)) return true;
         }
         else if (!bn2.childs.empty())
         {
            for(auto& b: bn2.childs)
               if (checkObjCollision(bn1, phy1, b, phy2)) return true;
         }
         else // activamos collided = true cuando hemos encontrado los nodos hoja
         {
            return true;
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

void
CollisionSys::react2Collision(EntityManager& g,
                              ColliderCmp& c1, ColliderCmp& c2) const
{
   using CP = ColliderCmp;
   CP* player = nullptr;
   CP* other = nullptr;
   if (c1.propierties & CP::P_IsPlayer){
      player = &c1;
      other = &c2;
   } else if (c2.propierties & CP::P_IsPlayer){
      player = &c2;
      other = &c1;
   }
   else return;

   

   // Inflingir daño
   if (other->propierties & CP::P_Damages){
      inflictDamage(g, *player, *other);
   }
   // Undo
   else if (other->propierties & CP::P_IsSolid){
      undoCollision(g, *player, *other);
   }
}

void CollisionSys::
inflictDamage(EntityManager& g, const ColliderCmp& receiver, 
                                const ColliderCmp& damager) const
{
   auto* reciver_health = g.getRequiredCmpFromCmp<HealthCmp>(receiver);
   auto* damager_health = g.getRequiredCmpFromCmp<HealthCmp>(damager);
   if (!reciver_health || !damager_health) return;
   reciver_health->damage += damager_health->damage_inflicted;
   damager_health->damage += damager_health->self_damage;
}


void CollisionSys::
undoCollision(EntityManager& g, const ColliderCmp& mobile, 
                                const ColliderCmp& solid) const
{
   auto* mobilePhy = g.getRequiredCmpFromCmp<PhysicsCmp>(mobile);
   auto* solidPhy = g.getRequiredCmpFromCmp<PhysicsCmp>(solid);
   if (!mobilePhy || !solidPhy) return;

   BoundingBox<float> m = move2WorldFrame(mobile.boxRoot.box, mobilePhy->x, mobilePhy->y);
   BoundingBox<float> s = move2WorldFrame(solid.boxRoot.box, solidPhy->x, solidPhy->y);

   //   mL     mR        |------|      // Right
    //   |------|                       // Left 
    //   |-----------------------|      // Center
    //          |--------|              // Center
    //      |----------------|
    //      sL               sR

   // Calculate intersection in 1-D
   auto checkIntersection = 
      [](float mL, float mR, float sL, float sR)
   {
      if (mL < sL){
         if (mR < sR)   return sL - mR;
      }
      else if (mR > sR) return sR - mL;
      return 0.0f;
   };

   // Calculate intersection
   float overlapX = checkIntersection(m.xL, m.xR, s.xL, s.xR);
   float overlapY = checkIntersection(m.yU, m.yD, s.yU, s.yD);

   // Undo Overlap
   if      (overlapX == 0)                             goto undoY;
   else if (overlapY == 0)                             goto undoX;
   else if  (std::abs(overlapX) >= std::abs(overlapY)) goto undoY;
   else                                                goto undoX;

   undoX:
   // std::cout << "UndoX  --> Overlap: "<<overlapX<< " , Vx: "<<mobilePhy->vx<< "\n";
   mobilePhy->x += overlapX;
   mobilePhy->vx = 0;

   return;

   undoY:
   // std::cout << "UndoY  --> Overlap: "<<overlapY<< " , Vy: "<<mobilePhy->vy<< "\n";
   mobilePhy->y += overlapY;
   mobilePhy->vy = 0;
   mobilePhy->vx *= solidPhy->friction;
   mobilePhy->onPlatform = (overlapY < 0); // sólo cuando choco con plataforma de abajo
   return;
}


const BoundingBoxNode&
CollisionSys::leafNodeCollided(const BoundingBoxNode& bn) const
{
   if(!bn.childs.empty())
   {
      for (auto& child : bn.childs){
         if (child.collided){
            return leafNodeCollided(child);
         }
      }
      std::cerr << "Error lógico, no se encontró ningún hijo que colisione";
      std::terminate();
   }
   else{
      return bn;
   }
}