#pragma once
#include <cstdint>

struct EntityManager;

struct SpawnSys 
{
   void update(EntityManager& g) const;
};

