#pragma once
#include <cstdint>

struct EntityManager;

struct SpawnSystem 
{
   void update(EntityManager& g) const;
};

