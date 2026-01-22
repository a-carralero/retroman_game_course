#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "util/typealiases.hpp"
#include "cmp/component.hpp"

struct CmpVecBase{
   virtual ~CmpVecBase() = default;
};

template <typename Cmp>
struct CmpVec : CmpVecBase {
   std::vector<Cmp> components;
};

struct ComponentStorage {
   explicit ComponentStorage(std::size_t size ) {
   }
   ComponentStorage(const ComponentStorage& ) = delete;
   ComponentStorage(ComponentStorage&& ) = delete;
   ComponentStorage operator=(const ComponentStorage& ) = delete;
   ComponentStorage operator=(ComponentStorage&& ) = delete;

   PhysicsComponent& createPhysicsComponent(uint32_t eid);

   template <typename Cmp>
   std::vector<Cmp>& getComponents(){

   }

   template <typename Cmp>
   std::vector<Cmp>& createComponentVec(){

   }

   template <typename Cmp>
   Cmp& createComponent(uint32_t eid){

   }



   const std::vector<PhysicsComponent>& getPhysicsComponents() const {return m_physicsComponents;}
         std::vector<PhysicsComponent>& getPhysicsComponents()       {return m_physicsComponents;}

private:
   std::unordered_map<uint32_t, std::unique_ptr<CmpVecBase>> cmp_vectors;
   std::size_t initial_size = 100;
};