#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "cmp/entity.hpp"

struct GameContext;


struct RenderSystem {
   RenderSystem(uint32_t w, uint32_t h);
   ~RenderSystem();
   bool update(const GameContext& g);
   void drawAllEntities(const std::vector<Entity>& entities);

private:
   const uint32_t m_w = 0, m_h = 0;
   std::unique_ptr<uint32_t[]> m_screen {nullptr};
   uint32_t* getScreenXY(uint32_t x, uint32_t y);
   void drawEntity(const Entity& e);
   
   static constexpr uint32_t kR = 0x00FF0000;
   static constexpr uint32_t kG = 0x0000FF00;
   static constexpr uint32_t kB = 0x000000FF;
};

