#pragma once

#include <cstdint>

struct EntityManager;
struct BoundingBoxNode;

struct HealthSys 
{
   bool update(EntityManager& g) const;

 private:
   bool leafNodeCollided(const BoundingBoxNode& bn) const;
};

