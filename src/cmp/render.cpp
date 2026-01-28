
#include <fstream>
#include <tuple>
#include "cmp/render.hpp"
#include "picoPNG/src/picopng.hpp"

// string_view es como un puntero a un string
auto RenderCmp::loadPNGFileIntoVector(const std::string_view filename)
{
   std::tuple<std::vector<unsigned char>, unsigned long, unsigned long> tupl;
   auto& [pixels, width, height] = tupl;
   std::ifstream file(filename.data(), std::ios::binary);
   std::vector<unsigned char> filevec (
      std::istreambuf_iterator<char>{file}, 
      std::istreambuf_iterator<char>{}
   );
   decodePNG(pixels, width, height, filevec.data(), filevec.size());
   return tupl;  // NRVO
}

void RenderCmp::initSpriteFromABGRData(const std::vector<unsigned char>& pixels)
{
   sprite.resize(pixels.size() / sizeof(uint32_t));
   uint32_t* spr = sprite.data();
   for (auto p = pixels.begin(); p != pixels.end(); p+=4){
      uint32_t pixel = 
         static_cast<uint32_t>(*(p  )) << 16 |
         static_cast<uint32_t>(*(p+1)) << 8  |
         static_cast<uint32_t>(*(p+2))       |
         static_cast<uint32_t>(*(p+3)) << 24;
      *spr = pixel;
      ++spr;
   }
}

void RenderCmp::loadFromFile(const std::string_view filename)
{
   auto [pixels, width, height] = loadPNGFileIntoVector(filename.data());
   w = width; h = height;
   initSpriteFromABGRData(pixels);
}

