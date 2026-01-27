#pragma once
#include <cstdint>
#include "cmp/collider.hpp"

struct EntityManager;
struct PhysicsCmp;

struct CollisionSys 
{
   explicit CollisionSys(uint32_t w, uint32_t h)
      : m_w(w), m_h(h) {}

   bool update(EntityManager& g) const;

 private:

   uint32_t m_w, m_h;

   BoundingBox move2ScreenFrame(const BoundingBox& box, 
                              uint32_t x, uint32_t y) const;
   
   void checkBoundaries(const ColliderCmp& cl, PhysicsCmp& phy) const;

   bool checkObjCollision(BoundingBoxNode& bn1, PhysicsCmp& phy1,
                          BoundingBoxNode& bn2, PhysicsCmp& phy2) const;

   void setFalseCollideds(BoundingBoxNode& bn) const;
   
};

