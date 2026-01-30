#pragma once

#include <cstdint>

struct EntityManager;

struct PhysicsSys 
{
   void update(EntityManager& g) const;
};

