#include <iostream>
#include <memory>
#include <cstdint>
#include <chrono>
#include <thread>
#include "sys/render.hpp"
#include "sys/physics.hpp"
#include "sys/collision.hpp"
#include "sys/input.hpp"
#include "sys/spawn.hpp"
#include "sys/health.hpp"
#include "sys/camera.hpp"
#include "man/entitymanager.hpp"
#include "util/factory.hpp"

constexpr uint32_t KWIDTH = 640;
constexpr uint32_t KHEIGHT = 360;
constexpr auto FPS = 60;
constexpr auto MSPF = 1000ms/FPS; // Los MS que dura un FPS

int main(){
   EntityManager EntityMan;
   Factory factory(EntityMan);
   Entity& player = factory.createPlyer(40, 40, "pngs/klipartz4.png");
   factory.createCamera(0,0, KWIDTH,KHEIGHT, player.getEntityID());
   factory.createLevel1();
   // factory.createBlade(40, 250, 1, -1);
   factory.createSpawner(500, 200, 
      [&factory](uint32_t x, uint32_t y, int32_t vx, int32_t vy){
         factory.createBlade(x,y,vx,vy);
   });
   RenderSys Render{KWIDTH, KHEIGHT};
   PhysicsSys Physics;
   CollisionSys Collision(KWIDTH, KHEIGHT);
   InputSys Input;
   SpawnSys Spawn;
   HealthSys Health;
   CameraSys Camera;
   Render.setDebugDraw(false);

   using clk = std::chrono::steady_clock;
   auto last_time = clk::now();
   while( !Input.isKeyPressed(XK_Escape))
   {
      Camera.update(EntityMan);
      Input.update(EntityMan);
      Physics.update(EntityMan);
      Collision.update(EntityMan);
      Health.update(EntityMan);
      Render.update(EntityMan);
      Spawn.update(EntityMan);
      auto interval = clk::now() - last_time;

      if (interval < MSPF){
         std::this_thread::sleep_for(MSPF - interval);
      }else{
         std::cout <<"Hemos pasado mucho tiempo bloqueados\n";
      }
      last_time = clk::now();
   }


   return 0;
}