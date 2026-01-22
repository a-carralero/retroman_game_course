
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include <algorithm>

EntityManager::EntityManager(){
   m_Entity.reserve(KINITIALENTITIES);
}

void EntityManager::createEntity(uint32_t x, uint32_t y, const std::string_view filename)
{
   Entity& e = m_Entity.emplace_back();
   auto& ph = m_components.createPhysicsComponent(e.entity_id);
   e.phy = &ph;
   ph.x = x; ph.y = y;
   ph.vx = 1; ph.vy = 1;
   auto& rn = 
   rn.loadFronFile(filename);
   e.ren = &rn;
   
}
