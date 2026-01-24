#pragma once
#include <cstdint>

struct EntityManager;

struct CollisionSystem 
{
   bool update(EntityManager& g) const;
};

