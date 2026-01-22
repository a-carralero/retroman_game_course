#include "man/componentstorage.hpp"


PhysicsComponent& ComponentStorage::createPhysicsComponent(uint32_t eid) {
   PhysicsComponent& cmp =  m_physicsComponents.emplace_back(eid);
   return cmp;
}
