#pragma once
#include <cstdint>

struct ComponentBase{
 protected:
   static inline uint32_t nextTypeID = 0;
};


template <typename T>
struct Component : ComponentBase 
{
 private:
   uint32_t entity_id = 0;

 public:
   Component(uint32_t eid) : entity_id(eid) {}

   uint32_t getEntityID() const {return entity_id;}

   static uint32_t getCmpTypeID(){
      static uint32_t typeID = ++nextTypeID;
      return typeID;
   }
};