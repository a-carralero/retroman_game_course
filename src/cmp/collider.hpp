#pragma once
#include <cstdint>
#include <vector>
#include "cmp/component.hpp"


struct BoundingBox{
   uint32_t xL = 0, xR = 0;
   uint32_t yU = 0, yD = 0;
};

struct BoundingBoxNode{
    BoundingBox box;
    bool collided {false};
    std::vector<BoundingBoxNode> childs;
};

struct ColliderCmp: Component<ColliderCmp>
{
   explicit ColliderCmp(uint32_t eid)
      : Component(eid) {}
   
   enum{
      L_NoLayer   = 0x00,   // 000
      L_Blades    = 0x01,   // 001
      L_Platforms = 0x02,   // 010
   };
   uint8_t mask = 0xFF;
   BoundingBoxNode boxRoot;
};