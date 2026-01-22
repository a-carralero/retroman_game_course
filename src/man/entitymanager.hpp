#pragma once
#include <cstdint>
#include <vector>
#include <string_view>
#include "util/gamecontext.hpp"
#include "man/componentstorage.hpp"

struct EntityManager: public GameContext{
   static constexpr uint32_t KINITIALENTITIES = 1000;
   explicit EntityManager();
   void createEntity(uint32_t x, uint32_t y, const std::string_view filename);
   const std::vector<Entity>& getEntities() const override {return m_Entity; }
         std::vector<Entity>& getEntities()       override {return m_Entity; }
   const std::vector<PhysicsComponent>& getPhysicsComponents() const override {return m_components.getPhysicsComponents();}
         std::vector<PhysicsComponent>& getPhysicsComponents()       override {return m_components.getPhysicsComponents();}


private:
   std::vector<Entity> m_Entity;
   ComponentStorage m_components {KINITIALENTITIES};
};