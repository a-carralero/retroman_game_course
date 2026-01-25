#pragma once
#include <cstdint>
#include "cmp/component.hpp"
#include <X11/X.h>
#include <X11/keysym.h>

struct InputCmp: Component<InputCmp>
{
   explicit InputCmp(uint32_t eid)
      : Component(eid) {}
   
   KeySym key_LEFT  {XK_Left };
   KeySym key_RIGHT {XK_Right};
   KeySym key_UP    {XK_Up   };
   KeySym key_DOWN  {XK_Down };
};