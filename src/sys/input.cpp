#include "sys/input.hpp"
#include "cmp/input.hpp"
#include "cmp/physics.hpp"
#include "man/entitymanager.hpp"
extern "C"{ 
   #include "tinyPTC/src/tinyptc.h"
}

void InputSystem::keypress(KeySym k){
   keyboard.keyPressed(k);
}

void InputSystem::keyrelease(KeySym k){
   keyboard.keyRelease(k);
}

InputSystem::InputSystem(){
   ptc_set_on_keypress(keypress);
   ptc_set_on_keyrelease(keyrelease);
}

bool InputSystem::update(EntityManager& g) const
{
   if (ptc_process_events()) return false;

   for (auto& inp : g.getComponents<InputCmp>())
   {
      auto* phy = g.getRequiredCmpFromCmp<PhysicsCmp>(inp);
      if (phy){
         phy->vx = 0; phy->vy = 0;
         if (keyboard.isKeyPressed(inp.key_RIGHT))phy->vx = 1;
         if (keyboard.isKeyPressed(inp.key_LEFT)) phy->vx = -1;
         if (keyboard.isKeyPressed(inp.key_UP))   phy->vy = -1;
         if (keyboard.isKeyPressed(inp.key_DOWN)) phy->vy = 1;
      }
   }
   return true;
}