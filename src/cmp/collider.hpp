#pragma once
#include <cstdint>
#include <vector>
#include "cmp/component.hpp"


template<typename T>
struct BoundingBox{
   T xL = 0, xR = 0;
   T yU = 0, yD = 0;
};

struct BoundingBoxNode{
    BoundingBox<uint32_t> box;
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
      L_Boundaries= 0x04
   };

   enum{
      P_Nothing  = 0x00,
      P_IsPlayer = 0x01,
      P_IsSolid  = 0x02,
      P_Damages  = 0x04
   };

   uint8_t propierties = P_Nothing;
   uint8_t mask = 0xFF;
   BoundingBoxNode boxRoot;
};