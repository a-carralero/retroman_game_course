#pragma once
#include "cmp/component.hpp"
#include <cstdint>

struct CameraCmp: Component<CameraCmp>
{
   explicit CameraCmp(uint32_t eid)
      : Component(eid) {}
   
   static constexpr uint32_t KDefaultWdith = 640;
   static constexpr uint32_t KDefaultHeight = 360;   

   uint32_t w = KDefaultWdith, h = KDefaultHeight;
   uint32_t scrx = 0, scry = 0; // coordenadas de pantalla
   int32_t followEntityID = -1;
};