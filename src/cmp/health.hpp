#pragma once
#include "cmp/component.hpp"
#include <cstdint>

struct HealthCmp: Component<HealthCmp>
{
   explicit HealthCmp(uint32_t eid)
      : Component(eid) {}
   
   uint64_t health = 1;
};