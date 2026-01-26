#pragma once
#include <cstdint>

struct EntityManager;

struct CollisionSystem 
{
   explicit CollisionSystem(uint32_t w, uint32_t h)
      : m_w(w), m_h(h) {}

   bool update(EntityManager& g) const;

 private:
 
   uint32_t m_w, m_h;
};

