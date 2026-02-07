#include <array>
#include <fstream>
#include "util/factory.hpp"
#include "cmp/camera.hpp"
#include "picoJSON/picojson.hpp"


Entity& Factory::createPlyer ( uint32_t x, uint32_t y, 
                   const std::string_view filename)
{
   Entity& e = em.createEntity();
   auto& ph = em.addComponent<PhysicsCmp>(e);
   auto& cl = em.addComponent<ColliderCmp>(e);
              em.addComponent<InputCmp>(e);
   auto& h  = em.addComponent<HealthCmp>(e);
   auto& rn = em.addComponent<RenderCmp>(e);

   h.health = 5;
   ph.x = x; ph.y = y;
   ph.vx = 0; ph.vy = 0;
   ph.gravity = PhysicsCmp::KGravity;
   rn.loadFromFile(filename);
   cl.propierties = ColliderCmp::P_IsPlayer;
   cl.mask ^= ColliderCmp::L_Boundaries;
   cl.boxRoot.box = {0, 83, 1, 133}; 
//     cl.boxRoot.childs = {
//         { {1,  82,   2,  96}, false,
//             { 
//                 {{3, 81, 8,   62}, false, {}}, 
//                 {{4, 77, 63,  83}, false, {}}, 
//                 {{19, 72, 84, 95}, false, {}} 
//             }                                 
//         },
//         { {29, 62,  97, 122}, false, {} }, 
//         { {37, 60, 123, 133}, false, {} }, 
// };
   return e;
}

void Factory::createBlade(uint32_t x, uint32_t y,
                          int32_t vx, int32_t vy)
{
   Entity& e = em.createEntity();
   auto& ph = em.addComponent<PhysicsCmp>(e);
   auto& rn = em.addComponent<RenderCmp>(e);
   auto& cl = em.addComponent<ColliderCmp>(e);
   auto& h  = em.addComponent<HealthCmp>(e);

   ph.x = x; ph.y = y; 
   ph.vx = vx; ph.vy = vy;
   rn.loadFromFile("pngs/blade.png");
   cl.boxRoot.box = {10, rn.w -10, 10, rn.h-10};
   cl.mask = ColliderCmp::L_Blades | ColliderCmp::L_Boundaries;
   cl.propierties = ColliderCmp::P_Damages;
   h.damage_inflicted = 1;
   h.self_damage = 1;
   h.health = 1;
}

void Factory::createPlatform(uint32_t x, uint32_t y)
{
   Entity& e = em.createEntity();
   auto& ph = em.addComponent<PhysicsCmp>(e);
   auto& rn = em.addComponent<RenderCmp>(e);
   auto& cl = em.addComponent<ColliderCmp>(e);

   ph.x = x; ph.y = y; 
   ph.friction = 0.85f;
   rn.loadFromFile("pngs/platform.png");
   cl.boxRoot.box = {0, rn.w, 0, rn.h};
   cl.mask = ColliderCmp::L_Platforms;
   cl.propierties = ColliderCmp::P_IsSolid;
}

void Factory::
createCamera(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t followEID=-1)
{
    Entity& e = em.createEntity();
    auto& cam = em.addComponent<CameraCmp>(e);
    em.addComponent<PhysicsCmp>(e);
    cam.scrx = x; cam.scry = y;
    cam.w = w;    cam.h = h;
    cam.followEntityID = followEID;
}

void Factory::
createCamera(uint32_t x, uint32_t y, uint32_t followEID=-1){
   Entity& e = em.createEntity();
    auto& cam = em.addComponent<CameraCmp>(e);
    em.addComponent<PhysicsCmp>(e);
    cam.scrx = x; cam.scry = y;
    cam.followEntityID = followEID;
}

void Factory::createLevel1()
{
   loadLevelBin("pngs/level1.bin");
   Entity& player = createPlyer(40, 40, "pngs/klipartz4.png");
   createCamera(0,0,player.getEntityID());
   createSpawner(500, 200, 
      [&](uint32_t x, uint32_t y, int32_t vx, int32_t vy){
         createBlade(x,y,vx,vy);
   });

}

void Factory::loadLevelJson(std::string_view filepath)
{
   std::ifstream file(filepath.data());
   if (!file){
      std::cerr << "El fichero JSON no se pudo abrir\n";
      std::terminate();
   }

   // Read JSON into Memory
   namespace pj = picojson;
   pj::value json;
   file >> json;
   std::string err {pj::get_last_error()};
   if (!err.empty()) {
      std::cerr << "Error al leer el JSON\n";
      std::cerr << err << "\n";
      std::terminate();
   }
   const auto& root = json.get<pj::object>();
   const auto& h    = root.at("height").get<double>();
   const auto& w    = root.at("width").get<double>();
   const auto& map  = root.at("map").get<pj::array>();
   if (map.size() != w*h){
      std::cerr << "Map size error";
      std::terminate();
   }

   uint32_t x = 0, y = 0;
   for (auto& elem : map){
      const auto& tile = static_cast<bool>(elem.get<double>());
      if (tile) createPlatform(78*x, 77*y);
      if (++x == w) { x=0; ++y; }
   }
}


void Factory::
json2Bin(std::string_view jsonpath, std::string_view binpath)
{
   // Open Json file
   std::ifstream filejson(jsonpath.data());
   if (!filejson){
      std::cerr << "El fichero JSON no se pudo abrir\n"; 
      std::terminate();
   }

   // Open Bin file
   std::ofstream filebin (binpath.data(), std::ofstream::binary | std::ofstream::trunc);
   if (!filebin){
      std::cerr << "El fichero BIN no se pudo abrir para escritura\n"; 
      std::terminate();
   }

   // Read JSON into Memory
   namespace pj = picojson;
   pj::value json;
   filejson >> json;
   std::string err {pj::get_last_error()};
   if (!err.empty()) {
      std::cerr << "Error al leer el JSON\n" << err << "\n";
      std::terminate();
   }

   // Write to binary
   const auto& root = json.get<pj::object>();
   const auto& h    = static_cast<uint32_t>(root.at("height").get<double>());
   const auto& w    = static_cast<uint32_t>(root.at("width").get<double>());
   const auto& map  = root.at("map").get<pj::array>();

   filebin.write(reinterpret_cast<const char*>(&w), sizeof w);
   filebin.write(reinterpret_cast<const char*>(&h), sizeof h);
   
   for (auto & elem : map){
      const auto& tile = static_cast<uint8_t>(elem.get<double>());
      filebin.write(reinterpret_cast<const char*>(&tile), sizeof tile);
   }
}

void Factory::loadLevelBin(std::string_view filepath)
{
// Open BIN file
   std::ifstream file(filepath.data(), std::ifstream::binary);
   if (!file){
      std::cerr << "El fichero JSON no se pudo abrir\n"; 
      std::terminate();
   }

   // Calculate file length
   file.seekg(0, std::ifstream::end);
   std::size_t length = file.tellg();
   file.seekg(0, std::ifstream::beg);

   // check minimal file size
   if (length < 8){
      std::cerr << " Binary file corrupt, Size to small\n";
      std::terminate();
   }

   // Read whole file at once
   std::vector<char> filemem(length);
   char* pfilemem = filemem.data();
   file.read(pfilemem, length);

   uint32_t w = 0, h = 0;
   std::memcpy(&w, pfilemem, sizeof w);
   std::memcpy(&h, pfilemem+4, sizeof h);

   if (w*h != (length - 8)){
      std::cerr << "Baad size in BIN file level\n";
      std::terminate();
   }

   pfilemem += 8;
   for (uint32_t y = 0; y < h; ++y){
      for (uint32_t x = 0; x < w; ++x){
         if (*pfilemem) createPlatform(x*78, y*77);
         pfilemem++;
      }
   }
}






