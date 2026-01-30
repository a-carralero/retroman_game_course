#pragma once
#include "cmp/component.hpp"
#include <cstdint>

struct PhysicsCmp: Component<PhysicsCmp>
{
   explicit PhysicsCmp(uint32_t eid)
      : Component(eid) {}
   
   static constexpr int32_t vmax = 80;
   
   int32_t g = 0, a = 0;
   uint32_t x = 0, y = 0;
   int32_t vx = 0, vy = 0;
};