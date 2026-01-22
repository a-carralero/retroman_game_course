#pragma once
#include "cmp/component.hpp"
#include <cstdint>

struct PhysicsCmp: Component<PhysicsCmp>
{
   explicit PhysicsCmp(uint32_t eid)
      : Component(eid) {}
   
   uint32_t x = 0, y = 0;
   int32_t vx = 0, vy = 0;
};