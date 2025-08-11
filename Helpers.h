#pragma once

#ifndef HELPERS_H
#define HELPERS_H

#include <cstdint>
#include <string>

#include "Constants.h"
#include "Random.h"

uint8_t GetZone(const uint8_t y);
void CreateDoor(bbRandom* bb, rnd_state* rnd_state);
void CreateItem(bbRandom* bb, rnd_state* rnd_state);
std::string RoomIDToName(RoomID r);

#endif