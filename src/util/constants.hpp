#pragma once
#include <cstdint>

static constexpr uint32_t KWIDTH = 640*2;
static constexpr uint32_t KHEIGHT = 360*2;
static constexpr int64_t FPS = 60;
static constexpr int64_t USPF = 1'000'000/FPS; // Los MS que dura un FPS