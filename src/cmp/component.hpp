#pragma once
#include <cstdint>

struct Component
{
   explicit Component(uint32_t eid) 
   : entity_id(eid) {}
private:
   uint32_t entity_id = 0;
   uint32_t component_id = nextCmpId++; 
   inline static uint32_t nextCmpId = 0;
};