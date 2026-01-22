#pragma once
#include "cmp/component.hpp"
#include <cstdint>

struct InputComponent: Component
{
   explicit InputComponent(uint32_t eid)
      : Component(eid) {}
   
};