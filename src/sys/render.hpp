#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "cmp/render.hpp"
#include "cmp/collider.hpp"


struct EntityManager;
struct PhysicsCmp;
struct CameraCmp;

struct CameraWithPhysics{
  const CameraCmp* cam = nullptr;
  const PhysicsCmp* phy = nullptr;
};

struct RenderSys 
{   
 private:
   std::unique_ptr<uint32_t[]> m_screen {nullptr};
   mutable bool m_debugDraw {false};
   mutable uint32_t m_debugColor { 0xFFFF0000 };
   mutable CameraWithPhysics m_currentCam;

   uint32_t* getScreenXY(uint32_t x, uint32_t y) const;
   void drawEntity(const RenderCmp& rn, const PhysicsCmp& phy);
   void drawAllEntities(const EntityManager& g) const;
   void drawClippedSprite(const RenderCmp& rn, const PhysicsCmp& phy) const;
   void renderFilledBox(uint32_t* screen, const BoundingBox<uint32_t>& box, uint32_t pixel) const;
   void renderInScreenLine(uint32_t* screen, uint32_t len, uint32_t stride, uint32_t pixel) const;
   void AlignedLineClipped(uint32_t x1, uint32_t x2, uint32_t y, bool yaxis, uint32_t pixel) const;
   void drawFilledBox(BoundingBox<uint32_t> box, uint32_t x, uint32_t y, uint32_t pixel) const;
   void drawBox (const BoundingBox<uint32_t>& box, uint32_t x, uint32_t y, uint32_t color) const;
   void drawBoxTree (const BoundingBoxNode& boxNode, uint32_t x, uint32_t y, uint32_t color) const;
   void drawAllCameras(const EntityManager& g) const;

   static constexpr uint32_t kR = 0x00FF0000;
   static constexpr uint32_t kG = 0x0000FF00;
   static constexpr uint32_t kB = 0x000000FF;
   static constexpr uint32_t kWhite = 0x00FFFFFF;

 public:
   RenderSys();
   ~RenderSys();
   void update(const EntityManager& g);
   void setDebugDraw(bool v) const      {m_debugDraw  = v;  }
   void setDebugColor(uint32_t c) const {m_debugColor = c; }

};

