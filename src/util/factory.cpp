#include "util/factory.hpp"


void Factory::createPlyer ( uint32_t x, uint32_t y, 
                   const std::string_view filename)
{
   Entity& e = em.createEntity();
   auto& ph = em.addComponent<PhysicsCmp>(e);
   auto& cl = em.addComponent<ColliderCmp>(e);
              em.addComponent<InputCmp>(e);
   auto& h  = em.addComponent<HealthCmp>(e);
   auto& rn = em.addComponent<RenderCmp>(e);

   ph.x = x; ph.y = y;
   ph.vx = 0; ph.vy = 0;
   rn.loadFromFile(filename);
   cl.boxRoot.box = {0, 83, 1, 135}; 
    cl.boxRoot.childs = {
        { {1,  82,   2,  96}, false,
            { 
                {{3, 81, 8,   62}, false, {}}, 
                {{4, 77, 63,  83}, false, {}}, 
                {{19, 72, 84, 95}, false, {}} 
            }                                 
        },
        { {29, 62,  97, 122}, false, {} }, 
        { {37, 60, 123, 133}, false, {} }, 
    };
}

void Factory::createBlade(uint32_t x, uint32_t y,
                          int32_t vx, int32_t vy)
{
   Entity& e = em.createEntity();
   auto& ph = em.addComponent<PhysicsCmp>(e);
   ph.x = x; ph.y = y; 
   ph.vx = vx; ph.vy = vy;
   auto& rn = em.addComponent<RenderCmp>(e);
   rn.loadFromFile("pngs/blade.png");
   auto& cl = em.addComponent<ColliderCmp>(e);
   cl.boxRoot.box = {10, rn.w -10, 10, rn.h-10};
}
