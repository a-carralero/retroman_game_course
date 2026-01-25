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

void RenderSystem::drawAllEntities(const std::vector<RenderCmp>& render_cmps,
                                   const EntityManager& g) 
{
   for(const auto& rn: render_cmps){
      auto* phy = g.getRequiredCmpFromCmp<PhysicsCmp>(rn);
      if (phy){
         drawEntity(rn, *phy);
      }
   }
}

void RenderSystem::update(const EntityManager& g)
{
   uint32_t* screen = m_screen.get();
   uint32_t size = m_w*m_h;
   std::fill(screen, screen+size, kR);
   drawAllEntities(g.getComponents<RenderCmp>(), g);
   ptc_update(screen);
}