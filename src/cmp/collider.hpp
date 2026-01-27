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
   
   uint8_t mask = 0xFF;
   BoundingBoxNode boxRoot;
};