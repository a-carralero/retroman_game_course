#pragma once
#include <cstdint>
#include "cmp/collider.hpp"

struct EntityManager;
struct PhysicsCmp;

struct CollisionSys 
{
   void update(EntityManager& g) const;

 private:

   BoundingBox<float> move2WorldFrame(const BoundingBox<uint32_t>& box, 
                              float x, float y) const;
   
   void checkBoundaries(const ColliderCmp& cl, PhysicsCmp& phy) const;

   bool checkObjCollision(BoundingBoxNode& bn1, PhysicsCmp& phy1,
                          BoundingBoxNode& bn2, PhysicsCmp& phy2) const;

   void setFalseCollideds(BoundingBoxNode& bn) const;

   void react2Collision(EntityManager& g, ColliderCmp& c1, ColliderCmp& c2) const;
   
   void inflictDamage(EntityManager& g, const ColliderCmp& receiver, 
                                        const ColliderCmp& damager) const;

   void undoCollision(EntityManager& g, const ColliderCmp& mobile, 
                                        const ColliderCmp& solid) const;

   const BoundingBoxNode& leafNodeCollided(const BoundingBoxNode& bn) const;
   
};

