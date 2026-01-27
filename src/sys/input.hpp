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
   bool update(EntityManager& g) const;
   static void keypress(KeySym);
   static void keyrelease(KeySym);
};

