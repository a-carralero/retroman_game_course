#pragma once
#include "cmp/component.hpp"
#include <memory>
#include <string_view>
#include <vector>

struct RenderCmp: Component<RenderCmp>
{
   uint32_t w = 0, h = 0;
   std::unique_ptr<uint32_t[]> sprite;

   explicit RenderCmp(uint32_t eid)
      : Component(eid) {}
   
   void loadFromFile(const std::string_view filename);
   auto loadPNGFileIntoVector(const std::string_view filename);
   void initSpriteFromABGRData(const std::vector<unsigned char>& pixels);
};