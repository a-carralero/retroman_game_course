#pragma once
#include <cstdint>
#include "util/keyboard.hpp"

struct EntityManager;

struct InputSys 
{
 private:
   static inline Keyboard keyboard{};
 public:
   InputSys();
   void update(EntityManager& g) const;
   bool isKeyPressed(KeySym);
   static void keypress(KeySym);
   static void keyrelease(KeySym);
};

