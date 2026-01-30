#include "sys/input.hpp"
#include "cmp/input.hpp"
#include "cmp/physics.hpp"
#include "man/entitymanager.hpp"
extern "C"{ 
   #include "tinyPTC/src/tinyptc.h"
}

void InputSys::keypress(KeySym k){
   keyboard.keyPressed(k);
}

void InputSys::keyrelease(KeySym k){
   keyboard.keyRelease(k);
}

InputSys::InputSys(){
   ptc_set_on_keypress(keypress);
   ptc_set_on_keyrelease(keyrelease);
}

bool InputSys::isKeyPressed(KeySym k){
   return keyboard.isKeyPressed(k);
}


void InputSys::update(EntityManager& g) const
{
   ptc_process_events();

   for (auto& inp : g.getComponents<InputCmp>())
   {
      auto* phy = g.getRequiredCmpFromCmp<PhysicsCmp>(inp);
      if (!phy){
         std::cerr << "Error InputSys::update(), el component de input no tiene asociado un component de físicas\n";
         std::terminate();
      }
      phy->vx = 0;
      if (keyboard.isKeyPressed(inp.key_RIGHT))phy->vx = 1;
      if (keyboard.isKeyPressed(inp.key_LEFT)) phy->vx = -1;
      if (keyboard.isKeyPressed(inp.key_UP)){
         keyboard.keyRelease(inp.key_UP);
         std::cout << "Salto\n";
         phy->a = -20;
      }
         // if (keyboard.isKeyPressed(inp.key_UP))   phy->vy = -1;
   }
}