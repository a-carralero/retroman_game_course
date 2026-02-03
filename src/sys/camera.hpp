#pragma once

#include <cstdint>

struct EntityManager;

struct CameraSys 
{
   void update(EntityManager& g) const;
};

