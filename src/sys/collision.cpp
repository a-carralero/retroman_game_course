#include "sys/collision.hpp"
#include "man/entitymanager.hpp"

bool CollisionSystem::update(EntityManager& g) const 
{
   // for (auto& e: g.getEntities())
   // {
   //    if (!e.phy || !e.ren) continue;

   //    if (e.phy->x > 640 || e.phy->x + e.w > 640){
   //       e.phy->x -= e.phy->vx; 
   //       e.phy->vx = -e.phy->vx;
   //    }
   //    if (e.phy->y > 360 || e.phy->y + e.h > 360){
   //       e.phy->y -= e.phy->vy;   
   //       e.phy->vy = -e.phy->vy;
   //    }
   // }
   return true;
}