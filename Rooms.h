#pragma once

#ifndef ROOMS_H
#define ROOMS_H

#include <cstdint>
#include <algorithm>

#include "Constants.h"
#include "Random.h"
#include "Forest.h"
#include "Helpers.h"

typedef struct RoomTemplates RoomTemplates;
static struct RoomTemplates {
	int8_t id = -1;
	uint8_t index = 0;
	uint8_t zone[5] = {0, 0, 0, 0, 0};
	uint8_t shape = 0;
	RoomID name = ROOMEMPTY;
	uint8_t commonness = 0;
	uint8_t lights = 0;
	bool disableOverlapCheck = false;
};

typedef struct Rooms Rooms;
static struct Rooms {
	int16_t id = -1;
	uint8_t zone = 0;

	float x = 0, z = 0;

	int16_t angle = 0;
	RoomTemplates* rt = nullptr;

	float minX = 0.0, minZ = 0.0;
	float maxX = 0.0, maxZ = 0.0;

};

void CreateRoomTemplates(RoomTemplates* rt);
bool SetRoom(RoomID MapRoom[6][70], RoomID room_name, uint8_t room_type, uint8_t pos, uint8_t min_pos, uint8_t max_pos);
Rooms CreateRoom(uint8_t MapTemp[19][19], uint8_t& roomIdCounter, uint8_t* forest, float* e, RoomTemplates* rts, bbRandom* bb, rnd_state* rnd_state, uint8_t zone, uint8_t roomshape, float x, /*float y, */float z, RoomID name);
bool PreventRoomOverlap(Rooms* r, Rooms* rooms, float* e);
bool CheckRoomOverlap(Rooms* r, Rooms* r2);
void FillRoom(uint8_t MapTemp[19][19], bbRandom* bb, rnd_state* rnd_state, Rooms* r, uint8_t* forest);
constexpr void GetRoomExtents(Rooms* r, float* e);

#endif