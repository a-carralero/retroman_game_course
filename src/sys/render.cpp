#include <memory>
#include <algorithm>
#include <cstring>
#include "sys/render.hpp"
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"

/* Para librerías en C, 
para asegurarnos que el compilador
no haga mangling de los símbolos*/
extern "C"{ 
   #include "tinyPTC/src/tinyptc.h"
}



RenderSystem::RenderSystem(uint32_t w, uint32_t h) 
   : m_w(w), m_h(h),
     m_screen (std::make_unique<uint32_t[]>(w*h))
   { 
   ptc_open("window", w, h);
}

RenderSystem::~RenderSystem(){
   ptc_close();
}

uint32_t* RenderSystem::getScreenXY(uint32_t x, uint32_t y) {
   uint32_t* screen =  m_screen.get() + x + m_w*y;
   return screen;
}

void RenderSystem::drawEntity(const RenderCmp& rn, const PhysicsCmp& phy)
{
   uint32_t* screen = getScreenXY(phy.x, phy.y);
   const uint32_t* spr = rn.sprite.get();
   for (uint32_t y=0; y< rn.h; ++y){
      std::memcpy(screen, spr, rn.w*sizeof(uint32_t));
      screen += m_w;
      spr += rn.w;
   }
}

void RenderSystem::drawAllEntities(const EntityManager& g) 
{
   for(const auto& rn: g.getComponents<RenderCmp>()){
      auto* phy = g.getRequiredCmpFromCmp<PhysicsCmp>(rn);
      if (phy){
         drawClippedSprite(rn, *phy);
      }
   }
}

void RenderSystem::update(const EntityManager& g)
{
   uint32_t* screen = m_screen.get();
   uint32_t size = m_w*m_h;
   std::fill(screen, screen+size, kR);
   drawAllEntities(g);
   ptc_update(screen);
}

void RenderSystem::drawClippedSprite(const RenderCmp& rn, const PhysicsCmp& phy)
{
   uint32_t x = phy.x;
   uint32_t y = phy.y;
   uint32_t w = rn.w;
   uint32_t h = rn.h;

   // Clipping
   uint32_t left_off = 0;
   uint32_t up_off = 0;

   // Horizontal Clipping Rules
   if (x >= m_w){                 // Left Clipping  
      left_off = - x;
      if (left_off >= w) return;  // Nothing to draw, sprite is out screen (left or right)
      x = 0;
      w = w - left_off;
   }
   else if ( x + w > m_w){        // Right Clipping
      w = m_w - x;
   }
   // Vertical Clipping Rules
   if (y >= m_h) {               // Up Clipping
      up_off = -y;
      if (up_off >= h) return;   // Nothig to draw
      y = 0;
      h = h - up_off;
   }
   else if (y + h > m_h){        // Down Clipping
      h = m_h - y;
   }

   // Draw Sprite
   uint32_t* screen = getScreenXY(x, y);
   const uint32_t* spr = rn.sprite.get() + up_off*rn.w + left_off;
   while (h--){
      for (uint32_t i = 0; i < w; ++i) {
            // draw only if transperency != 0
            if ( spr[i] & 0xFF000000 ) 
                screen[i] = spr[i];
      }
      spr += rn.w;
      screen += m_w;
   }
}
