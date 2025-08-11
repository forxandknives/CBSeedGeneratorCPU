#pragma once

#ifndef FOREST_H
#define FOREST_H

#include <cstdint>

#include "Random.h"
#include "Constants.h"
#include "Helpers.h"

void GenForestGrid(bbRandom* bb, rnd_state* rnd_state, uint8_t* forest);
int32_t TurnIfDeviating(int32_t maxDeviationDistance, int32_t pathx, int32_t center, int32_t dir);
bool TurnIfDeviatingBool(int32_t maxDeviationDistance, int32_t pathx, int32_t center, int32_t dir);
int32_t MoveForward(int32_t dir, int32_t pathx, int32_t pathy, int32_t rv);
bool Chance(bbRandom* bb, rnd_state* rnd_state, int32_t prob);
constexpr uint8_t GetForestData(uint8_t* f, int32_t tile_type, int32_t index);
void PopulateForestData(uint8_t* f);


#endif