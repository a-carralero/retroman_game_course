#pragma once
#include <cstdint>
#include <unordered_map>
#include "cmp/component.hpp"


struct RenderComponent;
struct PhysicsComponent;

struct Entity
{
   uint32_t getEntityId() const {return entity_id; }
   void 

private:
   std::unordered_map<uint32_t, Component*> m_components;
   static inline uint32_t nextID = 0;
   uint32_t entity_id  = nextID++;
};
