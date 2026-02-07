#pragma once
#include <cstdint>
#include "cmp/component.hpp"
#include "util/constants.hpp"

struct CameraCmp: Component<CameraCmp>
{
   explicit CameraCmp(uint32_t eid)
      : Component(eid) {}
   
   uint32_t w = KWIDTH, h = KHEIGHT; // si son const ya no son copiables
   uint32_t scrx = 0, scry = 0; // coordenadas de pantalla
   int32_t followEntityID = -1;
};