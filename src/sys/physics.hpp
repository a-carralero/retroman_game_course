#pragma once

#include <cstdint>

struct GameContext;

struct PhysicsSystem {

   bool update(GameContext& g) const;
};

