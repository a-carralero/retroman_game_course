#include "sys/health.hpp"
#include "man/entitymanager.hpp"
#include "cmp/health.hpp"
#include "cmp/collider.hpp"

bool HealthSys::update(EntityManager& g) const 
{
   for(auto& h: g.getComponents<HealthCmp>())
   {
      const auto* cl = g.getRequiredCmpFromCmp<ColliderCmp>(h);
      if (!cl) continue;
      if (h.health && leafNodeCollided(cl->boxRoot)){
         if (--h.health == 0){
            std::cout << "Entity " << h.getEntityID() << " is dead\n";
            g.destroyEntityByID(h.getEntityID());
         }
      }
   }
   return true;
}

bool 
HealthSys::leafNodeCollided(const BoundingBoxNode& bn) const
{
   if (bn.childs.empty()) {
      return bn.collided;
   }
   else if (bn.collided)
   {
      for (auto& b: bn.childs){
         if (leafNodeCollided(b))
            return true;
      }
   }
   return false;
}