#pragma once
#include <cstdint>
#include "util/keyboard.hpp"

struct EntityManager;

struct InputSystem 
{
 private:
   static inline Keyboard keyboard{};
 public:
   InputSystem();
   bool update(EntityManager& g) const;
   static void keypress(KeySym);
   static void keyrelease(KeySym);
};

