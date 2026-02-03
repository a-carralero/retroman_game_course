#pragma once
#include "cmp/component.hpp"
#include <cstdint>

struct HealthCmp: Component<HealthCmp>
{
   explicit HealthCmp(uint32_t eid)
      : Component(eid) {}
   
   uint32_t self_damage = 0;
   uint32_t damage_inflicted = 0;
   uint32_t damage = 0;
   uint32_t health = 0;
};