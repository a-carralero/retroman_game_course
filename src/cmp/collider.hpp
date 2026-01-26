#pragma once
#include "cmp/component.hpp"
#include <cstdint>

struct BoundingBox{
   uint32_t xL = 0, xR = 0;
   uint32_t yU = 0, yD = 0;
};

struct ColliderCmp: Component<ColliderCmp>
{
   explicit ColliderCmp(uint32_t eid)
      : Component(eid) {}
   
   BoundingBox box;
   
};