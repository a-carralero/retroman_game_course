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
   keyboard.resetKeyboard();
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
      phy->ax = 0;
      if (keyboard.isKeyPressed(inp.key_RIGHT)) phy->ax = phy->KStdAx;
      if (keyboard.isKeyPressed(inp.key_LEFT)) phy->ax = -phy->KStdAx;

      if (keyboard.isKeyPressed(inp.key_UP))
      {
         if (phy->onPlatform &&phy->jumpIdx == phy->jumpTable.size())
            phy->jumpIdx = 0;
      }

   }
}