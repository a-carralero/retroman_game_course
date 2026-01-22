#pragma once

#include <cstdint>

struct GameContext;

struct CollisionSystem {

   bool update(GameContext& g) const;
};

