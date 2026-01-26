#pragma once
#include <cstdint>
#include <chrono>
#include "cmp/component.hpp"

using namespace std::chrono_literals;

struct SpawnerCmp: Component<SpawnerCmp>
{
   explicit SpawnerCmp(uint32_t eid)
      : Component(eid) {}
   
   using clk = std::chrono::steady_clock;

   clk::time_point last_spawn_time = clk::now();
   std::chrono::duration<double> spawn_interval = 5s; 
   std::size_t to_be_spawned = 3;
};