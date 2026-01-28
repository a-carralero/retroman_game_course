#pragma once
#include <cstdint>
#include <unordered_map>
#include "cmp/component.hpp"


struct Entity
{
 private:
   std::unordered_map<uint32_t, ComponentBase*> m_components;
   static inline uint32_t nextID = 0;
   uint32_t entity_id  = nextID++;

 public:
   uint32_t getEntityID() const {return entity_id; }
   
   auto cmpsBegin(){
      return m_components.begin();
   }
   auto cmpsEnd(){
      return m_components.end();
   }

   template <typename Cmp_t>
   const Cmp_t* getComponent() const {
      uint32_t typeID = Cmp_t::getCmpTypeID();
      auto it = m_components.find(typeID);
      if (it != m_components.end())
         return static_cast<Cmp_t*>(it->second);
      else
         return nullptr;
   }

   template<typename Cmp_t>
   void linkComponent(Cmp_t& cmp){
      uint32_t typeID = Cmp_t::getCmpTypeID();
      m_components[typeID] =  &cmp;
   }

   void updateCmpPtr(uint32_t cid, ComponentBase* cmp)
   {
      auto it = m_components.find(cid);
      if (it != m_components.end()){
         it->second = cmp;
      }
   }

};
