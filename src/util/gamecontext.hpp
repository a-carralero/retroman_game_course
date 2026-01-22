#pragma once
#include <vector>
#include "cmp/entity.hpp"
#include "com/component.hpp"

struct GameContext{
   virtual ~GameContext() = default;
   virtual const std::vector<Entity>& getEntities() const = 0;
   virtual       std::vector<Entity>& getEntities()       = 0;

   virtual       std::vector<PhysicsComponent>& getPhysicsComponents()       = 0;
   virtual const std::vector<PhysicsComponent>& getPhysicsComponents() const = 0;

   
};