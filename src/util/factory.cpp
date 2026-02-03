#include "util/factory.hpp"
#include "cmp/camera.hpp"
#include <array>

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

void Factory::createLevel1()
{
    constexpr std::array level{
        0b00000000 ,
        0b00000000 ,
        0b00000011 ,
        0b00000111 ,
        0b11111111 
    };

    uint32_t y = 0;
    for (auto row: level){
        for (uint32_t x=0; x<78*8; x+=78){
            if (row & 0x80) createPlatform(x, y);
            row <<= 1;
        }
        y +=77 + 1;
    }
}

void Factory::
createCamera(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t followEID=-1)
{
    Entity& e = em.createEntity();
    auto& cam = em.addComponent<CameraCmp>(e);
    auto& ph = em.addComponent<PhysicsCmp>(e);
    cam.scrx = x; cam.scry = y;
    cam.w = w;    cam.h = h;
    cam.followEntityID = followEID;
}


