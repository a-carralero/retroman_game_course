#pragma once

#include <cstdint>

struct EntityManager;

struct PhysicsSystem 
{
   bool update(EntityManager& g) const;
};

