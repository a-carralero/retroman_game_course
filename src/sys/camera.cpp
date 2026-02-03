#include "sys/camera.hpp"
#include "man/entitymanager.hpp"
#include "cmp/camera.hpp"
#include "cmp/physics.hpp"
#include "cmp/render.hpp"


void CameraSys::update(EntityManager& g) const 
{
   for(auto& cam: g.getComponents<CameraCmp>())
   {
      if (cam.followEntityID < 0) continue;
      auto& entity = g.getEntityByID(cam.followEntityID);
      auto* entphy = entity.getComponent<PhysicsCmp>();
      auto* camphy = g.getRequiredCmpFromCmp<PhysicsCmp>(cam);
      if (!entphy || !camphy) {
         std::cerr << "No encontramos los componentes físicos de la cámara o el entity a seguir\n";
         std::terminate();
      }
      auto* renent = entity.getComponent<RenderCmp>();
      uint32_t rw = 0, rh = 0;
      if (renent){
         rw = renent->w;
         rh = renent->h;
      }
      camphy->x = entphy->x - ( cam.w - rw) / 2;
      camphy->y = entphy->y - ( cam.h - rh) / 2;
   }
}
