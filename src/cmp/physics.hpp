#pragma once
#include <cstdint>
#include <array>
#include "cmp/component.hpp"

struct PhysicsCmp: Component<PhysicsCmp>
{
   explicit PhysicsCmp(uint32_t eid)
      : Component(eid) {}
   
   static constexpr float KGravity = 0.5f;
   static constexpr float KMinVy = -9.0f;
   static constexpr float KMaxVy =  9.0f;
   static constexpr float KMinVx = -4.0f;
   static constexpr float KMaxVx =  4.0f;
   static constexpr float KStdAx = 0.5f;
   static constexpr float KMinVyForNotOnPlatform = 2.5f*KGravity;

   
   float x = 0, y = 0;
   float vx = 0, vy = 0;
   float ax = 0;
   float gravity = 0;
   float friction = 1.0f;

   bool onPlatform = false;
   static constexpr std::array jumpTable {
      -9, -9, -9, -9, -9, -9, -9, -9, -7, -7, -7, -7, -7, -6, -6, -6, -6, -6, -6,  -4, -4, -4, -4, -4
   };
   uint8_t jumpIdx = jumpTable.size();
};