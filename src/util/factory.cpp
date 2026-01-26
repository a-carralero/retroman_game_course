#include "util/factory.hpp"


void Factory::createPlyer ( uint32_t x, uint32_t y, 
                   const std::string_view filename)
{
   Entity& e = em.createEntity();
   auto& ph = em.addComponent<PhysicsCmp>(e);
   ph.x = x; ph.y = y;
   ph.vx = 0; ph.vy = 0;
   auto& rn = em.addComponent<RenderCmp>(e);
   rn.loadFromFile(filename);
   em.addComponent<InputCmp>(e);
   auto& cl = em.addComponent<ColliderCmp>(e);
   cl.box.xL = 10;
   cl.box.yU = 10;
   cl.box.xR = rn.w - 10;
   cl.box.yD = rn.h - 10;
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
   cl.box.xL = 10;
   cl.box.yU = 10;
   cl.box.xR = rn.w - 10;
   cl.box.yD = rn.h + 10;
}
