#include <memory>
#include "sys/render.hpp"
#include "man/entitymanager.hpp"
#include <algorithm>
#include <cstring>
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

void RenderSystem::drawEntity(const Entity& e){
   if (!e.phy || !e.ren) return;
   uint32_t* screen = getScreenXY(e.phy->x, e.phy->y);
   const uint32_t* spr = e.ren->sprite.get();
   for (uint32_t y=0; y< e.ren->h; ++y){
      std::memcpy(screen, spr, e.ren->w*sizeof(uint32_t));
      screen += m_w;
      spr += e.ren->w;
   }
}

void RenderSystem::drawAllEntities(const std::vector<Entity>& entities) {
   for(auto& e: entities){
      drawEntity(e);
   }
}

bool RenderSystem::update(const GameContext& g)
{
   uint32_t* screen = m_screen.get();
   uint32_t size = m_w*m_h;
   std::fill(screen, screen+size, kR);
   drawAllEntities(g.getEntities());
   ptc_update(screen);

   return !ptc_process_events();
}