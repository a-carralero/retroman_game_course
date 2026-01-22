#pragma once
#include "cmp/component.hpp"
#include <cstdint>

struct InputCmp: Component<InputCmp>
{
   explicit InputCmp(uint32_t eid)
      : Component(eid) {}
   
};