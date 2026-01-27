#pragma once

#include <cstdint>

struct EntityManager;

struct PhysicsSys 
{
   bool update(EntityManager& g) const;
};

