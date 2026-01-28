#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "util/typealiases.hpp"
#include "cmp/component.hpp"
#include <iostream>

struct CmpVecBase{
   virtual ~CmpVecBase() = default;
   virtual ComponentBase& deleteCmpByEntID(uint32_t eid) = 0;
};

template <typename Cmp_t>
struct CmpVec : CmpVecBase 
{
   std::vector<Cmp_t> components;

   ComponentBase& deleteCmpByEntID(uint32_t eid) override 
   {
      for(auto it = components.begin(); it!=components.end(); ++it)
      {
         auto& cmp = *it;
         if (cmp.getEntityID() == eid)
         {
            if (it != components.end() -1 )  // si no soy el último
            {
               *it = components.back();      // copiamos el último en esa posición
            }
            components.pop_back();          // borramos el último componente que ahora está por duplicado

            std::cout << "Removing Component [EID: "<<eid 
                                        <<", CMPID: "<<cmp.getCmpTypeID()<<"]\n";

            return *it; // devolvemos el componente que copiamos haciendo upcast para actuailzar los punteros de Entity
         }
      }
      std::cerr << "Error deleteCmpByEntID() -> No se encuentra el componente\n";
      std::terminate();
   }
};

struct ComponentStorage 
{
 private:

   std::unordered_map<uint32_t, std::unique_ptr<CmpVecBase>> cmp_vectors;
   std::size_t init_size = 100;

   template <typename Cmp_t>
   std::vector<Cmp_t>& createCmpVector()
   {
      auto uptr = std::make_unique<CmpVec<Cmp_t>>();
      auto &v = uptr->components;
      v.reserve(init_size);
      uint32_t typeID = Cmp_t::getCmpTypeID();
      cmp_vectors[typeID] = std::move(uptr);
      return v;
   }

public:

   explicit ComponentStorage(std::size_t size )
      : init_size(size) {
   }

   template <typename Cmp_t>
   std::vector<Cmp_t>& getCmpVector()
   {
      std::vector<Cmp_t>* vptr = nullptr;
      uint32_t typeID = Cmp_t::getCmpTypeID();
      auto it = cmp_vectors.find(typeID);
      if (it != cmp_vectors.end()){
         auto* aux = dynamic_cast<CmpVec<Cmp_t>*>((it->second).get());
         vptr = &(aux->components);
      } else {
         vptr = &createCmpVector<Cmp_t>();
      }
      return *vptr;
   }

   template <typename Cmp_t>
   const std::vector<Cmp_t>& getCmpVector() const
   {
      std::vector<Cmp_t>* vptr = nullptr;
      uint32_t typeID = Cmp_t::getCmpTypeID();
      auto it = cmp_vectors.find(typeID);
      if (it != cmp_vectors.end()){
         auto* aux = dynamic_cast<CmpVec<Cmp_t>*>((it->second).get());
         vptr = &(aux->components);
      } else {
         std::cerr << "No se encontró el vector de componentes\n";
         std::terminate();
      }
      return *vptr;
   }

   template <typename Cmp_t>
   Cmp_t& createComponent(uint32_t eid)
   {
      auto& v = getCmpVector<Cmp_t>();
      return v.emplace_back(eid);
   }

   ComponentBase& destroyComponent(uint32_t type_id, uint32_t eid)
   {
      auto it = cmp_vectors.find(type_id);
      if (it != cmp_vectors.end())
      {
         CmpVecBase& vec = *(it->second);
         return vec.deleteCmpByEntID(eid);
      } 
      else {
         std::cerr << "No se encontró el vector de componentes a borrar\n";
         std::terminate();
      }
   }

   ComponentStorage(const ComponentStorage& ) = delete;
   ComponentStorage(ComponentStorage&& ) = delete;
   ComponentStorage operator=(const ComponentStorage& ) = delete;
   ComponentStorage operator=(ComponentStorage&& ) = delete;
};