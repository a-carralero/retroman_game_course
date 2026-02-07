#pragma once
#include <iostream>
#include <cstdint>
#include "sys/render.hpp"
#include "sys/physics.hpp"
#include "sys/collision.hpp"
#include "sys/input.hpp"
#include "sys/spawn.hpp"
#include "sys/health.hpp"
#include "sys/camera.hpp"
#include "man/entitymanager.hpp"
#include "util/factory.hpp"
#include "util/timer.hpp"
#include "man/state_base.hpp"
#include "util/constants.hpp"


class Game: public StateBase
{
   EntityManager EntityMan;
   Factory factory{EntityMan};
   RenderSys Render;
   PhysicsSys Physics;
   CollisionSys Collision;
   InputSys Input;
   SpawnSys Spawn;
   HealthSys Health;
   CameraSys Camera;

public:

   Game()
   {
      factory.createLevel1();
   }

   void update() override
   {
      Timer timer;
      timedCall("Camera", Camera.update(EntityMan));
      timedCall("Input", Input.update(EntityMan));
      timedCall("Physics", Physics.update(EntityMan));
      timedCall("Collision", Collision.update(EntityMan));
      timedCall("Health", Health.update(EntityMan));
      timedCall("Render", Render.update(EntityMan));
      timedCall("Spawn", Spawn.update(EntityMan));
      if (Input.isKeyPressed(XK_Escape)) alive = false;
      // std::cout << "[Towait] " << timer.waitUntil_us(USPF) << "\n";
      timer.waitUntil_us(USPF);
      timer.start();
   }
};