#pragma once
#include <cstdint>
#include <vector>
#include <string_view>
#include <iostream>
#include <utility>
#include "man/componentstorage.hpp"
#include "cmp/entity.hpp"

struct EntityManager
{
 private:
   std::vector<Entity> entities;
   ComponentStorage cmp_storage {KINITIALENTITIES};
   static constexpr uint32_t KINITIALENTITIES = 1000;

 public:
   explicit EntityManager() {
      entities.reserve(KINITIALENTITIES);
   }

   Entity& createEntity(){
      return entities.emplace_back();
   }

   const std::vector<Entity>& getEntities() const {return entities; }
         std::vector<Entity>& getEntities()       {return entities; }

   template <typename Cmp_t>
   const std::vector<Cmp_t>& getComponents() const {
      return cmp_storage.getCmpVector<Cmp_t>();
   }

   template <typename Cmp_t>
   std::vector<Cmp_t>& getComponents() {
      return cmp_storage.getCmpVector<Cmp_t>();
   }
         
   template <typename Cmp_t>
   Cmp_t& createComponent(uint32_t eid){
      return cmp_storage.createComponent<Cmp_t>(eid);
   }

   template <typename Cmp_t>
   Cmp_t& addComponent(Entity& e){
      uint32_t eid = e.getEntityID();
      const Cmp_t* cmp_ptr = e.getComponent<Cmp_t>();
      if (cmp_ptr){
         std::cerr << "EntityManager::addComponent() : El componente a añadir ya existe\n";
         std::terminate();      
      }
      Cmp_t& cmp = createComponent<Cmp_t>(eid);
      e.linkComponent(cmp);
      return cmp;
   }


   const Entity& getEntityByID(uint32_t eid) const
   {
      for (const Entity& e : entities){
         if (e.getEntityID() == eid)
            return e;      
      }
      std::cerr << "EntityManager::getEntityByID() : Ese ID no corresponde con ningún Entity\n";
      std::terminate();      
   }

   Entity& getEntityByID(uint32_t eid){
      return const_cast<Entity&>(
         std::as_const(*this).getEntityByID(eid)
      );
   }

   template <typename C1_t, typename C2_t>
   const C1_t* getRequiredCmpFromCmp(const C2_t& c) const {
      const Entity& e = getEntityByID(c.getEntityID());
      return e.getComponent<C1_t>();
   }

   template <typename C1_t, typename C2_t>
   C1_t* getRequiredCmpFromCmp(const C2_t& c) {
      return const_cast<C1_t*>(
         std::as_const(*this).getRequiredCmpFromCmp<C1_t, C2_t>(c)
      );
   }

   auto getEntityIteratorByID(uint32_t eid){
      for (auto it = entities.begin(); it!=entities.end(); ++it){
         if (it->getEntityID() == eid)
            return it;
      }
      std::cerr << "EntityManager::getEntityIteratorByID() : Ese ID no corresponde con ningún Entity\n";
      std::terminate(); 

   }

   void destroyEntityByID(uint32_t eid)
   {
      auto e_it = getEntityIteratorByID(eid);
      Entity& e = *e_it;
      std::cout << "Deleting Entity: "<<eid<<"\n";

      for(auto it = e.cmpsBegin(); it!=e.cmpsEnd(); ++it)
      {
         uint32_t type_id = it->first;
         ComponentBase& cmp = cmp_storage.destroyComponent(type_id, eid);
         auto& entity_to_move = getEntityByID(cmp.getEntityID());
         entity_to_move.updateCmpPtr(type_id, &cmp);
      }
      entities.erase(e_it);
   }

};