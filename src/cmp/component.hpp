#pragma once
#include <cstdint>

struct ComponentBase
{
 private:
   uint32_t entity_id = 0;
 protected:
   static inline uint32_t nextTypeID = 0;
 public:
   uint32_t getEntityID() const {return entity_id;}
   ComponentBase(uint32_t eid) : entity_id(eid) {}
};

template <typename T>
struct Component : ComponentBase 
{
   Component(uint32_t eid) : ComponentBase(eid) {}

   static uint32_t getCmpTypeID(){
      static uint32_t typeID = ++nextTypeID;
      return typeID;
   }
};