#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "util/typealiases.hpp"
#include "cmp/component.hpp"

struct CmpVecBase{
   virtual ~CmpVecBase() = default;
};

template <typename Cmp_t>
struct CmpVec : CmpVecBase {
   std::vector<Cmp_t> components;
};

struct ComponentStorage 
{
 private:

   std::unordered_map<uint32_t, std::unique_ptr<CmpVecBase>> cmp_vectors;
   std::size_t init_size = 100;

   template <typename Cmp_t>
   std::vector<Cmp_t>& createCmpVector()
   {
      auto uptr = std::make_unique<CmpVecBase<Cmp_t>>();
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
         auto* aux = dynamic_cast<CmpVec<Cmp_t>*>((it->second).get())
         vptr = &(aux->components);
      } else {
         vptr = &createCmpVector<Cmp_t>();
      }
      return *vptr;
   }

   template <typename Cmp_t>
   Cmp_t& createComponent(uint32_t eid)
   {
      auto& v = getCmpVector<Cmp_t>();
      return v.emplace_back(eid);
   }

   ComponentStorage(const ComponentStorage& ) = delete;
   ComponentStorage(ComponentStorage&& ) = delete;
   ComponentStorage operator=(const ComponentStorage& ) = delete;
   ComponentStorage operator=(ComponentStorage&& ) = delete;
};