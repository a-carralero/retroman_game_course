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



RenderSys::RenderSys(uint32_t w, uint32_t h) 
   : m_w(w), m_h(h),
     m_screen (std::make_unique<uint32_t[]>(w*h))
   { 
   ptc_open("window", w, h);
}

RenderSys::~RenderSys(){
   ptc_close();
}

uint32_t* RenderSys::getScreenXY(uint32_t x, uint32_t y) const {
   uint32_t* screen =  m_screen.get() + x + m_w*y;
   return screen;
}

void RenderSys::drawEntity(const RenderCmp& rn, const PhysicsCmp& phy)
{
   uint32_t* screen = getScreenXY(phy.x, phy.y);
   const uint32_t* spr = rn.sprite.data();
   for (uint32_t y=0; y< rn.h; ++y){
      std::memcpy(screen, spr, rn.w*sizeof(uint32_t));
      screen += m_w;
      spr += rn.w;
   }
}

void RenderSys::update(const EntityManager& g)
{
   uint32_t* screen = m_screen.get();
   uint32_t size = m_w*m_h;
   std::fill(screen, screen+size, kWhite);
   drawAllEntities(g);
   ptc_update(screen);
}

void RenderSys::drawAllEntities(const EntityManager& g) 
{
   for(const auto& rn: g.getComponents<RenderCmp>()){
      auto* phy = g.getRequiredCmpFromCmp<PhysicsCmp>(rn);
      if (phy){
         drawClippedSprite(rn, *phy);
      }
      if(m_debugDraw){
         const auto* cl = g.getRequiredCmpFromCmp<ColliderCmp>(rn);
         if (!cl) return;
         drawBoxTree(cl->boxRoot, phy->x, phy->y, m_debugColor);
      }
   }
}

void RenderSys::drawClippedSprite(const RenderCmp& rn, const PhysicsCmp& phy)
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
   const uint32_t* spr = rn.sprite.data() + up_off*rn.w + left_off;
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

void RenderSys::drawBoxTree(const BoundingBoxNode& boxNode, 
                          uint32_t x, uint32_t y, 
                          uint32_t color) const
{
    if (boxNode.collided){
        drawFilledBox(boxNode.box, x, y, color);
    }
    else{
        drawBox(boxNode.box, x, y, color);
    }
    for (std::size_t i=0; i<boxNode.childs.size(); ++i)
        drawBoxTree(boxNode.childs[i], x, y, kG);
}


void RenderSys::drawBox(const BoundingBox& box, 
                           uint32_t x, uint32_t y, 
                           uint32_t color) const
{
    uint32_t xL = x + box.xL;
    uint32_t xR = x + box.xR;
    uint32_t yU = y + box.yU;
    uint32_t yD = y + box.yD;

    AlignedLineClipped(xL, xR, yU, false, color);
    AlignedLineClipped(xL, xR, yD, false, color);
    AlignedLineClipped(yU, yD, xL, true , color);
    AlignedLineClipped(yU, yD, xR, true , color);
}




void
RenderSys::AlignedLineClipped(uint32_t xL, uint32_t xR, uint32_t y, 
                                 bool yaxis, uint32_t pixel    ) const
{
    uint32_t infinite = 4*m_w; // beyond infinite, all are considered negative values
    uint32_t maxx = m_w;
    uint32_t maxy = m_h;
    uint32_t stride = 1;       // stride in 1st axis is +1
    uint32_t* screen = nullptr;

    // If the first axis is Y instead of X, swithc axis values
    if (yaxis){
        infinite = 4*m_h;
        maxx = m_h;
        maxy = m_w;
        stride = m_w;
    }

    // Ensure that y coorditate is in screen
    if (y >= maxy) return;

    // Define and dtect start and end of the line
    uint32_t xini{0}, xend{0};
    if (xL > xR) { xini = xR; xend = xL; }
    else         { xini = xL, xend = xR; }

    // Crop line to screen limits
    if      ( xini > infinite ) xini = 0;
    else if ( xini > maxx     ) return;
    if      (xend > infinite  ) xend = 0;
    else if (xend > maxx      ) xend = maxx;
    if      (xini > xend      ) return;

    // Draw the line
    if ( yaxis) screen = getScreenXY(y, xini);
    else        screen = getScreenXY(xini, y);
    renderInScreenLine (screen, xend-xini, stride, pixel);
}


void 
RenderSys::renderInScreenLine(uint32_t* screen, uint32_t len,
                                 uint32_t stride, uint32_t pixel) const
{
    while(len > 0) {
        *screen = pixel;
        --len;
        screen += stride;
    }
}

void
RenderSys::drawFilledBox(BoundingBox box, 
                            uint32_t x, uint32_t y, 
                            uint32_t pixel  ) const
{
    // Crop function
    auto crop = [](uint32_t &val, uint32_t max, uint32_t inf) {
        if (val > inf) val = 0;
        else if (val > max) val = max;
    };
    
    // Infinite values to be considered negatives
    uint32_t xinf = 4*m_w;
    uint32_t yinf = 4*m_h;

    // Convert bounding Box to screen coordinates
    box.xL += x; box.xR += x;
    box.yU += y; box.yD += y;

    // Crop line to screen limits
    crop(box.xL, m_w, xinf);
    crop(box.xR, m_w, xinf);
    crop(box.yU, m_h, yinf);
    crop(box.yD, m_h, yinf);

    if (box.xL == m_w || box.xR == 0 
        || box.yU == m_h || box.yD == 0) return;

    renderFilledBox(getScreenXY(box.xL, box.yU), box, pixel);
}


void
RenderSys::renderFilledBox(uint32_t* screen, const BoundingBox& box, 
                              uint32_t pixel) const
{
    const uint32_t width = box.xR - box.xL;
    uint32_t height = box.yD - box.yU;

    while (height--)
    {
        for (uint32_t i=0; i<width; ++i){
            screen[i] = pixel; 
        }
        screen += m_w;
    }
}

