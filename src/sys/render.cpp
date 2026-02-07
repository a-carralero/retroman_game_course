#include <memory>
#include <algorithm>
#include <cstring>
#include <cmath>
#include "sys/render.hpp"
#include "man/entitymanager.hpp"
#include "cmp/physics.hpp"
#include "cmp/camera.hpp"
#include "util/constants.hpp"


/* Para librerías en C, 
para asegurarnos que el compilador
no haga mangling de los símbolos*/
extern "C"{ 
   #include "tinyPTC/src/tinyptc.h"
}


RenderSys::RenderSys() 
   : m_screen (std::make_unique<uint32_t[]>(KWIDTH*KHEIGHT))
   { 
   ptc_open("window", KWIDTH, KHEIGHT);
}

RenderSys::~RenderSys(){
   ptc_close();
}

uint32_t* RenderSys::getScreenXY(uint32_t x, uint32_t y) const {
   uint32_t* screen =  m_screen.get() + x + KWIDTH*y;
   return screen;
}

void RenderSys::drawEntity(const RenderCmp& rn, const PhysicsCmp& phy)
{
   uint32_t* screen = getScreenXY(phy.x, phy.y);
   const uint32_t* spr = rn.sprite.data();
   for (uint32_t y=0; y< rn.h; ++y){
      std::memcpy(screen, spr, rn.w*sizeof(uint32_t));
      screen += KWIDTH;
      spr += rn.w;
   }
}

void RenderSys::update(const EntityManager& g)
{
   uint32_t* screen = m_screen.get();
   uint32_t size = KWIDTH*KHEIGHT;
   std::fill(screen, screen+size, kWhite);
   drawAllCameras(g);
   ptc_update(screen);
}

void RenderSys::drawAllEntities(const EntityManager& g) const
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

void RenderSys::drawAllCameras(const EntityManager& g) const 
{
   for(const auto& cam: g.getComponents<CameraCmp>()){
      auto* phy = g.getRequiredCmpFromCmp<PhysicsCmp>(cam);
      if (!phy) return;
      m_currentCam.cam = &cam;
      m_currentCam.phy = phy;
      drawAllEntities(g);
   }
}

void RenderSys::
drawClippedSprite(const RenderCmp& rn, const PhysicsCmp& phy) const
{
   // SPRITE -> WORLD -> CAMERA -> SCREEN
   //      +pos    -poscam    +poscamscr

   if (!m_currentCam.phy || !m_currentCam.cam){
      std::cerr << "Coordenadas de camara no definidas\n";
      std::terminate();
   }
   auto& CamScr = *m_currentCam.cam;
   auto& CamPhy = *m_currentCam.phy;

   struct {
   BoundingBox<float> world {};
   BoundingBox<float> camera {};
   BoundingBox<uint32_t> crop {};
   struct {
      uint32_t x, y, w, h;
      } screen;
   } spr;

   spr.world = {
      phy.x      , 
      phy.x +rn.w , 
      phy.y       , 
      phy.y +rn.h
   }; 

   spr.camera = {
      spr.world.xL - CamPhy.x,
      spr.world.xR - CamPhy.x,
      spr.world.yU - CamPhy.y,
      spr.world.yD - CamPhy.y,
   };

   // SPRITE CLIPPING
   if (  spr.camera.xR < 0 || spr.camera.xL > CamScr.w 
      || spr.camera.yD < 0 || spr.camera.yU > CamScr.h )
      return;
   
   // SPRITE CROPPING
   spr.crop = {
      static_cast<uint32_t>(std::round((spr.camera.xL < 0)         ? -spr.camera.xL : 0 )),
      static_cast<uint32_t>(std::round((spr.camera.xR > CamScr.w ) ? spr.camera.xR - CamScr.w  : 0 )),
      static_cast<uint32_t>(std::round((spr.camera.yU < 0)         ? -spr.camera.yU : 0 )),
      static_cast<uint32_t>(std::round((spr.camera.yD > CamScr.h)  ? spr.camera.yD - CamScr.h : 0)) 
   };
   
   spr.screen = {
      static_cast<uint32_t>(std::round(spr.camera.xL + CamScr.scrx)) + spr.crop.xL,
      static_cast<uint32_t>(std::round(spr.camera.yU + CamScr.scry)) + spr.crop.yU,
      rn.w - spr.crop.xL - spr.crop.xR ,
      rn.h - spr.crop.yU - spr.crop.yD        
   };

   // uint32_t x = static_cast<uint32_t>(std::round(phy.x));
   // uint32_t y = static_cast<uint32_t>(std::round(phy.y));
   // uint32_t w = rn.w;
   // uint32_t h = rn.h;

   // // Clipping
   // uint32_t left_off = 0;
   // uint32_t up_off = 0;

   // // Horizontal Clipping Rules
   // if (x >= KWIDTH){                 // Left Clipping  
   //    left_off = - x;
   //    if (left_off >= w) return;  // Nothing to draw, sprite is out screen (left or right)
   //    x = 0;
   //    w = w - left_off;
   // }
   // else if ( x + w > KWIDTH){        // Right Clipping
   //    w = KWIDTH - x;
   // }
   // // Vertical Clipping Rules
   // if (y >= KHEIGHT) {               // Up Clipping
   //    up_off = -y;
   //    if (up_off >= h) return;   // Nothig to draw
   //    y = 0;
   //    h = h - up_off;
   // }
   // else if (y + h > KHEIGHT){        // Down Clipping
   //    h = KHEIGHT - y;
   // }

   // Draw Sprite
   uint32_t* screen = getScreenXY(spr.screen.x, spr.screen.y);
   const uint32_t* sprite = rn.sprite.data() + static_cast<uint32_t>(spr.crop.yU*rn.w + spr.crop.xL);
   while (spr.screen.h--){
      for (uint32_t i = 0; i < spr.screen.w; ++i) {
            // draw only if transperency != 0
            if ( *sprite & 0xFF000000 ) 
                *screen = *sprite;
            sprite++;
            screen++;
      }
      sprite += rn.w - spr.screen.w;
      screen += KWIDTH - spr.screen.w;
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


void RenderSys::drawBox(const BoundingBox<uint32_t>& box, 
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
    uint32_t infinite = 4*KWIDTH; // beyond infinite, all are considered negative values
    uint32_t maxx = KWIDTH;
    uint32_t maxy = KHEIGHT;
    uint32_t stride = 1;       // stride in 1st axis is +1
    uint32_t* screen = nullptr;

    // If the first axis is Y instead of X, swithc axis values
    if (yaxis){
        infinite = 4*KHEIGHT;
        maxx = KHEIGHT;
        maxy = KWIDTH;
        stride = KWIDTH;
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
RenderSys::drawFilledBox(BoundingBox<uint32_t> box, 
                            uint32_t x, uint32_t y, 
                            uint32_t pixel  ) const
{
    // Crop function
    auto crop = [](uint32_t &val, uint32_t max, uint32_t inf) {
        if (val > inf) val = 0;
        else if (val > max) val = max;
    };
    
    // Infinite values to be considered negatives
    uint32_t xinf = 4*KWIDTH;
    uint32_t yinf = 4*KHEIGHT;

    // Convert bounding Box to screen coordinates
    box.xL += x; box.xR += x;
    box.yU += y; box.yD += y;

    // Crop line to screen limits
    crop(box.xL, KWIDTH, xinf);
    crop(box.xR, KWIDTH, xinf);
    crop(box.yU, KHEIGHT, yinf);
    crop(box.yD, KHEIGHT, yinf);

    if (box.xL == KWIDTH || box.xR == 0 
        || box.yU == KHEIGHT || box.yD == 0) return;

    renderFilledBox(getScreenXY(box.xL, box.yU), box, pixel);
}


void RenderSys::
renderFilledBox(uint32_t* screen, const BoundingBox<uint32_t>& box, 
                                             uint32_t pixel) const
{
    const uint32_t width = box.xR - box.xL;
    uint32_t height = box.yD - box.yU;

    while (height--)
    {
        for (uint32_t i=0; i<width; ++i){
            screen[i] = pixel; 
        }
        screen += KWIDTH;
    }
}

