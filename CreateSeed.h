#pragma once

#ifndef CREATESEED_H
#define CREATESEED_H

#include <cstdint>

#include "Rooms.h"
#include "Helpers.h"

uint8_t CreateMap(int32_t thread, RoomTemplates* rts, float* e, uint8_t* forest);

#endif
