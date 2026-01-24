#include "util/keyboard.hpp"
#include <iostream>

bool& Keyboard::getMapElement(KeySym k){
   auto it = m_pressedKeys.find(k);
   if (it != m_pressedKeys.end()){
      return it->second;
   } else{
      std::cerr << "getMapElement: la tecla no existe en el mapa\n";
      std::terminate();
   }
}

bool Keyboard::isKeyPressed(KeySym k){
   return getMapElement(k);
}

void Keyboard::keyPressed(KeySym k){
   getMapElement(k) = true;
}

void Keyboard::keyRelease(KeySym k){
   getMapElement(k) = false;
}