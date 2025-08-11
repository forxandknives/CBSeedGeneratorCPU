#include "CreateSeed.h"

#include <algorithm>

using namespace std;

uint8_t CreateMap(int32_t thread, RoomTemplates* rts, float* e, uint8_t* forest) {

	uint8_t MapTemp[MapWidth + 1][MapHeight + 1] = { {0, 0} };
	RoomID MapRoom[ROOM4 + 1][70] = { {RoomID::ROOMEMPTY, RoomID::ROOMEMPTY} };
	uint8_t roomIdCounter = 0;

	rnd_state rnd_state;

	bbRandom bb = bbRandom();
	bb.bbSeedRnd(&rnd_state, thread);

	int32_t x = 0, y = 0, temp = 0;
	int32_t i = 0, x2 = 0, y2 = 0;
	int32_t width = 0, height = 0;

	uint8_t zone = 0;

	RoomID MapName[MapWidth][MapHeight] = { {ROOMEMPTY, ROOMEMPTY} };
	uint8_t MapRoomID[ROOM4 + 1] = { 0 };

	x = floor(float(MapWidth / 2));
	y = MapHeight - 2;

	for (i = y; i <= MapHeight - 1; i++) {
		MapTemp[x][i] = 1;		
	}

	{
		//This scope is so we can just remove the varaibles we no longer use after this block.
		uint8_t tempheight = 0;
		uint8_t yhallways = 0;

		do {
			width = bb.bbRand(&rnd_state, 10, 15);

			if (x > MapWidth * 0.6) {
				width = -width;
			}
			else if (x > MapWidth * 0.4) {
				x = x - (width / 2);
			}

			if (x + width > MapWidth - 3) {
				width = MapWidth - 3 - x;
			}
			else if (x + width < 2) {
				width = -x + 2;
			}

			x = min(x, x + width);
			width = abs((int32_t)width);

			for (i = x; i <= x + width; i++) {
				MapTemp[min(uint8_t(i), MapWidth)][y] = 1;
			}

			height = bb.bbRand(&rnd_state, 3, 4);

			if (y - height < 1) {
				height = y - 1;
			}

			yhallways = bb.bbRand(&rnd_state, 4, 5);

			if (GetZone(y - height) != GetZone(y - height + 1)) {
				height = height - 1;
			}

			for (i = 1; i <= yhallways; i++) {
				x2 = max(min(bb.bbRand(&rnd_state, x, x + width - 1), MapWidth - 2), 2);

				while (MapTemp[x2][y - 1] || MapTemp[x2 - 1][y - 1] || MapTemp[x2 + 1][y - 1]) {
					x2++;
				}

				if (x2 < x + width) {
					if (i == 1) {
						tempheight = height;
						if (bb.bbRand(&rnd_state, 1, 2) == 1) {
							x2 = x;
						}
						else {
							x2 = x + width;
						}
					}
					else {
						tempheight = bb.bbRand(&rnd_state, 1, height);
					}

					for (y2 = y - tempheight; y2 <= y; y2++) {
						if (GetZone(y2) != GetZone(y2 + 1)) {
							MapTemp[x2][y2] = 255;
						}
						else {
							MapTemp[x2][y2] = 1;
						}
					}

					if (tempheight == height) {
						temp = x2;
					}
				}
			}
			x = temp;
			y = y - height;

		} while (y >= 2);
	}

	//printf("FIRST: %d\n", rnd_state.rnd_state);

	uint8_t ZoneAmount = 3;
	uint8_t Room1Amount[3] = { 0 };
	uint8_t Room2Amount[3] = { 0 };
	uint8_t Room2CAmount[3] = { 0 };
	uint8_t Room3Amount[3] = { 0 };
	uint8_t Room4Amount[3] = { 0 };

	//count the amount of rooms
	//might be problems with adding 1 to end number
	//or reducing starting number by 1.
	for (y = 1; y <= MapHeight - 1; y++) {
		uint8_t zone = GetZone(y);
		for (x = 1; x <= MapWidth - 1; x++) {
			if (MapTemp[x][y] > 0) {
				temp = min(MapTemp[x + 1][y], uint8_t(1)) + min(MapTemp[x - 1][y], uint8_t(1));
				temp = temp + min(MapTemp[x][y + 1], uint8_t(1)) + min(MapTemp[x][y - 1], uint8_t(1));
				if (MapTemp[x][y] < 255) {
					MapTemp[x][y] = temp;
				}
				switch (MapTemp[x][y]) {
				case 1:
					Room1Amount[zone] = Room1Amount[zone] + 1;
					break;
				case 2:
					if (min(MapTemp[x + 1][y], uint8_t(1)) + min(MapTemp[x - 1][y], uint8_t(1)) == 2) {
						Room2Amount[zone] = Room2Amount[zone] + 1;
					}				
					else if (min(MapTemp[x][y + 1], uint8_t(1)) + min(MapTemp[x][y - 1], uint8_t(1)) == 2) {
						Room2Amount[zone] = Room2Amount[zone] + 1;
					}
					else {
						Room2CAmount[zone] = Room2CAmount[zone] + 1;
					}
					break;
				case 3:
					Room3Amount[zone] = Room3Amount[zone] + 1;
					break;
				case 4:
					Room4Amount[zone] = Room4Amount[zone] + 1;
					break;
				}
			}
		}
	}				

	//Force more room1s.
	for (i = 0; i <= 2; i++) {
		temp = -Room1Amount[i] + 5;
		if (temp > 0) {
			for (y = (MapHeight / ZoneAmount) * (2 - i) + 1; y <= ((MapHeight / ZoneAmount) * ((2 - i) + 1.0)) - 2; y++) {
				for (x = 2; x <= MapWidth - 2; x++) {
					if (MapTemp[x][y] == 0) {
						if (min(MapTemp[x + 1][y], uint8_t(1)) + min(MapTemp[x - 1][y], uint8_t(1)) + min(MapTemp[x][y + 1], uint8_t(1)) + min(MapTemp[x][y - 1], uint8_t(1)) == 1) {
							if (MapTemp[x + 1][y]) {
								x2 = x + 1;
								y2 = y;
							}
							else if (MapTemp[x - 1][y]) {
								x2 = x - 1;
								y2 = y;
							}
							else if (MapTemp[x][y + 1]) {
								x2 = x;
								y2 = y + 1;
							}
							else if (MapTemp[x][y - 1]) {
								x2 = x;
								y2 = y - 1;
							}
							bool placed = false;
							if (MapTemp[x2][y2] > 1 && MapTemp[x2][y2] < 4) {
								switch (MapTemp[x2][y2]) {
								case 2:
									if (min(MapTemp[x2 + 1][y2], uint8_t(1)) + min(MapTemp[x2 - 1][y2], uint8_t(1)) == 2) {
										Room2Amount[i] = Room2Amount[i] - 1;
										Room3Amount[i] = Room3Amount[i] + 1;
										placed = true;
									}
									else if (min(MapTemp[x2][y2 + 1], uint8_t(1)) + min(MapTemp[x2][y2 - 1], uint8_t(1)) == 2) {
										Room2Amount[i] = Room2Amount[i] - 1;
										Room3Amount[i] = Room3Amount[i] + 1;
										placed = true;
									}
									break;
								case 3:
									Room3Amount[i] = Room3Amount[i] - 1;
									Room4Amount[i] = Room4Amount[i] + 1;
									placed = true;
									break;
								}
								if (placed) {
									MapTemp[x2][y2] = MapTemp[x2][y2] + 1;

									MapTemp[x][y] = 1;
									Room1Amount[i] = Room1Amount[i] + 1;
									temp = temp - 1;
								}
							}
						}
					}
					if (temp == 0) break;
				}
				if (temp == 0) break;
			}
		}
	}			

	uint8_t temp2 = 0;

	//printf("SECOND: %d\n", rnd_state.rnd_state);

	//force more room4s and room2cs
	for (i = 0; i <= 2; i++) {

		switch (i) {
		case 2:
			zone = 2;
			temp2 = MapHeight / 3;
			break;
		case 1:
			zone = MapHeight / 3 + 1;
			temp2 = MapHeight * (2.0 / 3.0) - 1;
			break;
		case 0:
			zone = MapHeight * (2.0 / 3.0) + 1;
			temp2 = MapHeight - 2;
			break;
		}

		if (Room4Amount[i] < 1) {
			temp = 0;

			for (y = zone; y <= temp2; y++) {
				for (x = 2; x <= MapWidth - 2; x++) {
					if (MapTemp[x][y] == 3) {
						//switch (0)
						//We have to invert each if
						if (!(MapTemp[x + 1][y] || MapTemp[x + 1][y + 1] || MapTemp[x + 1][y - 1] || MapTemp[x + 2][y])) {
							MapTemp[x + 1][y] = 1;
							temp = 1;						
						}
						else if (!(MapTemp[x - 1][y] || MapTemp[x - 1][y + 1] || MapTemp[x - 1][y - 1] || MapTemp[x - 2][y])) {
							MapTemp[x - 1][y] = 1;
							temp = 1;						
						}
						else if (!(MapTemp[x][y + 1] || MapTemp[x + 1][y + 1] || MapTemp[x - 1][y + 1] || MapTemp[x][y + 2])) {
							MapTemp[x][y + 1] = 1;
							temp = 1;
						}
						else if (!(MapTemp[x][y - 1] || MapTemp[x + 1][y - 1] || MapTemp[x - 1][y - 1] || MapTemp[x][y - 2])) {
							MapTemp[x][y - 1] = 1;
							temp = 1;
						}

						if (temp == 1) {
							MapTemp[x][y] = 4;
							Room4Amount[i] = Room4Amount[i] + 1;
							Room3Amount[i] = Room3Amount[i] - 1;
							Room1Amount[i] = Room1Amount[i] + 1;
						}
					}
					if (temp == 1) break;
				}
				if (temp == 1) break;
			}
		}

		if (Room2CAmount[i] < 1) {
			temp = 0;

			zone = zone + 1;
			temp2 = temp2 - 1;

			for (y = zone; y <= temp2; y++) {
				for (x = 3; x <= MapWidth - 3; x++) {
					if (MapTemp[x][y] == 1) {
						if (true) {
							if (MapTemp[x - 1][y] > 0) {
								if ((MapTemp[x][y - 1] + MapTemp[x][y + 1] + MapTemp[x + 2][y]) == 0) {
									if ((MapTemp[x + 1][y - 2] + MapTemp[x + 2][y - 1] + MapTemp[x + 1][y - 1]) == 0) {
										MapTemp[x][y] = 2;
										MapTemp[x + 1][y] = 2;
										MapTemp[x + 1][y - 1] = 1;
										temp = 1;
									}
									else if ((MapTemp[x + 1][y + 2] + MapTemp[x + 2][y + 1] + MapTemp[x + 1][y + 1]) == 0) {
										MapTemp[x][y] = 2;
										MapTemp[x + 1][y] = 2;
										MapTemp[x + 1][y + 1] = 1;
										temp = 1;
									}
								}
							}
							else if (MapTemp[x + 1][y] > 0) {
								if ((MapTemp[x][y - 1] + MapTemp[x][y + 1] + MapTemp[x - 2][y]) == 0) {
									if ((MapTemp[x - 1][y - 2] + MapTemp[x - 2][y - 1] + MapTemp[x - 1][y - 1]) == 0) {
										MapTemp[x][y] = 2;
										MapTemp[x - 1][y] = 2;
										MapTemp[x - 1][y - 1] = 1;
										temp = 1;
									}
									else if ((MapTemp[x - 1][y + 2] + MapTemp[x - 2][y + 1] + MapTemp[x - 1][y + 1]) == 0) {
										MapTemp[x][y] = 2;
										MapTemp[x - 1][y] = 2;
										MapTemp[x - 1][y + 1] = 1;
										temp = 1;
									}
								}
							}
							else if (MapTemp[x][y - 1] > 0) {
								if ((MapTemp[x - 1][y] + MapTemp[x + 1][y] + MapTemp[x][y + 2]) == 0) {
									if ((MapTemp[x - 2][y + 1] + MapTemp[x - 1][y + 2] + MapTemp[x - 1][y + 1]) == 0) {
										MapTemp[x][y] = 2;
										MapTemp[x][y + 1] = 2;
										MapTemp[x - 1][y + 1] = 1;
										temp = 1;
									}
									else if ((MapTemp[x + 2][y + 1] + MapTemp[x + 1][y + 2] + MapTemp[x + 1][y + 1]) == 0) {
										MapTemp[x][y] = 2;
										MapTemp[x][y + 1] = 2;
										MapTemp[x + 1][y + 1] = 1;
										temp = 1;

									}
								}
							}
							else if (MapTemp[x][y + 1] > 0) {
								if ((MapTemp[x - 1][y] + MapTemp[x + 1][y] + MapTemp[x][y - 2]) == 0) {
									if ((MapTemp[x - 2][y - 1] + MapTemp[x - 1][y - 2] + MapTemp[x - 1][y - 1]) == 0) {
										MapTemp[x][y] = 2;
										MapTemp[x][y - 1] = 2;
										MapTemp[x - 1][y - 1] = 1;
										temp = 1;
									}
									else if ((MapTemp[x + 2][y - 1] + MapTemp[x + 1][y - 2] + MapTemp[x + 1][y - 1]) == 0) {
										MapTemp[x][y] = 2;
										MapTemp[x][y - 1] = 2;
										MapTemp[x + 1][y - 1] = 1;
										temp = 1;
									}
								}
							}
						}
						if (temp == 1) {
							Room2CAmount[i] = Room2CAmount[i] + 1;
							Room2Amount[i] = Room2Amount[i] + 1;
						}
					}
					if (temp == 1) break;
				}
				if (temp == 1) break;
			}
		}
	}			

	//printf("THIRD: %d\n", rnd_state.rnd_state);

	int32_t MaxRooms = 55 * MapWidth / 20;
	MaxRooms = max(MaxRooms, Room1Amount[0] + Room1Amount[1] + Room1Amount[2] + 1);
	MaxRooms = max(MaxRooms, Room2Amount[0] + Room2Amount[1] + Room2Amount[2] + 1);
	MaxRooms = max(MaxRooms, Room2CAmount[0] + Room2CAmount[1] + Room2CAmount[2] + 1);
	MaxRooms = max(MaxRooms, Room3Amount[0] + Room3Amount[1] + Room3Amount[2] + 1);
	MaxRooms = max(MaxRooms, Room4Amount[0] + Room4Amount[1] + Room4Amount[2] + 1);

	//zone 1 --------------------------------------------------------------------------------------
	uint8_t min_pos = 1;
	uint8_t max_pos = Room1Amount[0] - 1;

	MapRoom[ROOM1][0] = START;
	SetRoom(MapRoom, ROOMPJ, ROOM1, floor(0.1 * float(Room1Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM914, ROOM1, floor(0.3 * float(Room1Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM1ARCHIVE, ROOM1, floor(0.5 * float(Room1Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM205, ROOM1, floor(0.6 * float(Room1Amount[0])), min_pos, max_pos);

	MapRoom[ROOM2C][0] = LOCKROOM;	

	min_pos = 1;
	max_pos = Room2Amount[0] - 1;

	MapRoom[ROOM2][0] = ROOM2CLOSETS;
	SetRoom(MapRoom, ROOM2TESTROOM2, ROOM2, floor(0.1 * float(Room2Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2SCPS, ROOM2, floor(0.2 * float(Room2Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2STORAGE, ROOM2, floor(0.3 * float(Room2Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2GW_B, ROOM2, floor(0.4 * float(Room2Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2SL, ROOM2, floor(0.5 * float(Room2Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM012, ROOM2, floor(0.55 * float(Room2Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2SCPS2, ROOM2, floor(0.6 * float(Room2Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM1123, ROOM2, floor(0.7 * float(Room2Amount[0])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2ELEVATOR, ROOM2, floor(0.85 * float(Room2Amount[0])), min_pos, max_pos);

	uint8_t tempIndex = floor(bb.bbRnd(&rnd_state, 0.2, 0.8) * float(Room3Amount[0]));
	MapRoom[ROOM3][tempIndex] = ROOM3STORAGE;

	tempIndex = floor(0.5 * float(Room2CAmount[0]));
	MapRoom[ROOM2C][tempIndex] = ROOM1162;

	tempIndex = floor(0.3 * float(Room4Amount[0]));
	MapRoom[ROOM4][tempIndex] = ROOM4INFO;

	//zone 2 --------------------------------------------------------------------------------------

	min_pos = Room1Amount[0];
	max_pos = Room1Amount[0] + Room1Amount[1] - 1;

	SetRoom(MapRoom, ROOM079, ROOM1, Room1Amount[0] + floor(0.15 * float(Room1Amount[1])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM106, ROOM1, Room1Amount[0] + floor(0.3 * float(Room1Amount[1])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM008, ROOM1, Room1Amount[0] + floor(0.4 * float(Room1Amount[1])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM035, ROOM1, Room1Amount[0] + floor(0.5 * float(Room1Amount[1])), min_pos, max_pos);
	SetRoom(MapRoom, COFFIN, ROOM1, Room1Amount[0] + floor(0.7 * float(Room1Amount[1])), min_pos, max_pos);

	min_pos = Room2Amount[0];
	max_pos = Room2Amount[0] + Room2Amount[1] - 1;


	tempIndex = Room2Amount[0] + floor(0.1 * float(Room2Amount[1]));
	MapRoom[ROOM2][tempIndex] = ROOM2NUKE;
	SetRoom(MapRoom, ROOM2TUNNEL, ROOM2, Room2Amount[0] + floor(0.25 * float(Room2Amount[1])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM049, ROOM2, Room2Amount[0] + floor(0.4 * float(Room2Amount[1])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2SHAFT, ROOM2, Room2Amount[0] + floor(0.6 * float(Room2Amount[1])), min_pos, max_pos);
	SetRoom(MapRoom, TESTROOM, ROOM2, Room2Amount[0] + floor(0.7 * float(Room2Amount[1])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2SERVERS, ROOM2, Room2Amount[0] + floor(0.9 * Room2Amount[1]), min_pos, max_pos);

	tempIndex = Room3Amount[0] + floor(0.3 * float(Room3Amount[1]));
	MapRoom[ROOM3][tempIndex] = ROOM513;
	tempIndex = Room3Amount[0] + floor(0.6 * float(Room3Amount[1]));
	MapRoom[ROOM3][tempIndex] = ROOM966;

	tempIndex = Room2CAmount[0] + floor(0.5 * float(Room2CAmount[1]));
	MapRoom[ROOM2C][tempIndex] = ROOM2CPIT;	

	//zone 3 --------------------------------------------------------------------------------------

	MapRoom[ROOM1][Room1Amount[0] + Room1Amount[1] + Room1Amount[2] - 2] = EXIT1;
	MapRoom[ROOM1][Room1Amount[0] + Room1Amount[1] + Room1Amount[2] - 1] = GATEAENTRANCE;
	MapRoom[ROOM1][Room1Amount[0] + Room1Amount[1]] = ROOM1LIFTS;

	min_pos = Room2Amount[0] + Room2Amount[1];
	max_pos = Room2Amount[0] + Room2Amount[1] + Room2Amount[2] - 1;

	tempIndex = min_pos + floor(0.1 * float(Room2Amount[2]));
	MapRoom[ROOM2][tempIndex] = ROOM2POFFICES;
	SetRoom(MapRoom, ROOM2CAFETERIA, ROOM2, min_pos + floor(0.2 * float(Room2Amount[2])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2SROOM, ROOM2, min_pos + floor(0.3 * float(Room2Amount[2])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2SERVERS2, ROOM2, min_pos + floor(0.4 * Room2Amount[2]), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2OFFICES, ROOM2, min_pos + floor(0.45 * Room2Amount[2]), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2OFFICES4, ROOM2, min_pos + floor(0.5 * Room2Amount[2]), min_pos, max_pos);
	SetRoom(MapRoom, ROOM860, ROOM2, min_pos + floor(0.6 * Room2Amount[2]), min_pos, max_pos);
	SetRoom(MapRoom, MEDIBAY, ROOM2, min_pos + floor(0.7 * float(Room2Amount[2])), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2POFFICES2, ROOM2, min_pos + floor(0.8 * Room2Amount[2]), min_pos, max_pos);
	SetRoom(MapRoom, ROOM2OFFICES2, ROOM2, min_pos + floor(0.9 * float(Room2Amount[2])), min_pos, max_pos);

	MapRoom[ROOM2C][Room2CAmount[0] + Room2CAmount[1]] = ROOM2CCONT;
	MapRoom[ROOM2C][Room2CAmount[0] + Room2CAmount[1] + 1] = LOCKROOM2;

	tempIndex = Room3Amount[0] + Room3Amount[1] + floor(0.3 * float(Room3Amount[2]));
	MapRoom[ROOM3][tempIndex] = ROOM3SERVERS;
	tempIndex = Room3Amount[0] + Room3Amount[1] + floor(0.7 * float(Room3Amount[2]));
	MapRoom[ROOM3][tempIndex] = ROOM3SERVERS2;
	tempIndex = Room3Amount[0] + Room3Amount[1] + floor(0.5 * float(Room3Amount[2]));
	MapRoom[ROOM3][tempIndex] = ROOM3OFFICES;	

	//-----------------------------------------------------------------------------------------

	//printf("FOURTH: %d\n", rnd_state.rnd_state);

	temp = 0;
	//Rooms r;

	//we are going to attempt to use an array of Rooms to store the created rooms.
	//Guessing that we never generate more than 150 rooms.
	Rooms rooms[150] = { NULL };
	uint8_t roomsIndex = 0; //we must increment this after creating each room;		

	for (y = MapHeight-1; y >= 1; y--) {
		if (y < MapHeight / 3 + 1) {
			zone = 3;
		}
		else if (y < MapHeight * (2.0 / 3.0)) {
			zone = 2;
		}
		else {
			zone = 1;
		}

		for (x = 1; x <= MapWidth - 2; x++) {
			if (MapTemp[x][y] == 255) {
				if (y > MapHeight / 2) {
					rooms[roomsIndex++] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, zone, ROOM2, x * 8, y * 8, CHECKPOINT1);
				}
				else {
					rooms[roomsIndex++] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, zone, ROOM2, x * 8, y * 8, CHECKPOINT2);
				}
			}
			else if (MapTemp[x][y] > 0) {
				temp = min(MapTemp[x + 1][y], uint8_t(1)) + min(MapTemp[x - 1][y], uint8_t(1)) + min(MapTemp[x][y + 1], uint8_t(1)) + min(MapTemp[x][y - 1], uint8_t(1));

				switch (temp) {
				case 1:
					if (MapRoomID[ROOM1] < MaxRooms && MapName[x][y] == ROOMEMPTY) {
						if (MapRoom[ROOM1][MapRoomID[ROOM1]] != ROOMEMPTY) {
							MapName[x][y] = MapRoom[ROOM1][MapRoomID[ROOM1]];
						}
					}

					//we do not increment roomsIndex here because we need to edit the room at this index
					rooms[roomsIndex] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, zone, ROOM1, x * 8, y * 8, MapName[x][y]);
					if (MapTemp[x][y + 1]) {
						rooms[roomsIndex].angle = 180;
					}
					else if (MapTemp[x - 1][y]) {
						rooms[roomsIndex].angle = 270;
					}
					else if (MapTemp[x + 1][y]) {
						rooms[roomsIndex].angle = 90;
					}
					else {
						rooms[roomsIndex].angle = 0;
					}
					//Increment here since we didnt do it earlier.
					roomsIndex++;
					MapRoomID[ROOM1] = MapRoomID[ROOM1] + 1;
					break;

				case 2:
					//we gotta do more not incrementing roomsIndex here so might be wrong.
					if (MapTemp[x - 1][y] > 0 && MapTemp[x + 1][y] > 0) {
						if (MapRoomID[ROOM2] < MaxRooms && MapName[x][y] == ROOMEMPTY) {
							if (MapRoom[ROOM2][MapRoomID[ROOM2]] != ROOMEMPTY) {
								MapName[x][y] = MapRoom[ROOM2][MapRoomID[ROOM2]];
							}
						}
						rooms[roomsIndex] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, zone, ROOM2, x * 8, y * 8, MapName[x][y]);
						if (bb.bbRand(&rnd_state, 1, 2) == 1) {
							rooms[roomsIndex].angle = 90;
						}
						else {
							rooms[roomsIndex].angle = 270;
						}
						roomsIndex++;
						MapRoomID[ROOM2] = MapRoomID[ROOM2] + 1;

					}
					else if (MapTemp[x][y - 1] > 0 && MapTemp[x][y + 1] > 0) {
						if (MapRoomID[ROOM2] < MaxRooms && MapName[x][y] == ROOMEMPTY) {
							if (MapRoom[ROOM2][MapRoomID[ROOM2]] != ROOMEMPTY) {
								MapName[x][y] = MapRoom[ROOM2][MapRoomID[ROOM2]];
							}
						}
						rooms[roomsIndex] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state,zone, ROOM2, x * 8, y * 8, MapName[x][y]);
						if (bb.bbRand(&rnd_state, 1, 2) == 1) {
							rooms[roomsIndex].angle = 180;
						}
						else {
							rooms[roomsIndex].angle = 0;
						}
						roomsIndex++;
						MapRoomID[ROOM2] = MapRoomID[ROOM2] + 1;

					}
					else {
						if (MapRoomID[ROOM2C] < MaxRooms && MapName[x][y] == ROOMEMPTY) {
							if (MapRoom[ROOM2C][MapRoomID[ROOM2C]] != ROOMEMPTY) {
								MapName[x][y] = MapRoom[ROOM2C][MapRoomID[ROOM2C]];
							}
						}

						if (MapTemp[x - 1][y] > 0 && MapTemp[x][y + 1] > 0) {
							rooms[roomsIndex] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, zone, ROOM2C, x * 8, y * 8, MapName[x][y]);
							rooms[roomsIndex].angle = 180;
							roomsIndex++;
						}
						else if (MapTemp[x + 1][y] > 0 && MapTemp[x][y + 1] > 0) {
							rooms[roomsIndex] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, zone, ROOM2C, x * 8, y * 8, MapName[x][y]);
							rooms[roomsIndex].angle = 90;
							roomsIndex++;
						}
						else if (MapTemp[x - 1][y] > 0 && MapTemp[x][y - 1] > 0) {
							rooms[roomsIndex] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, zone, ROOM2C, x * 8, y * 8, MapName[x][y]);
							rooms[roomsIndex].angle = 270;
							roomsIndex++;
						}
						else {
							rooms[roomsIndex] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, zone, ROOM2C, x * 8, y * 8, MapName[x][y]);
							roomsIndex++;
						}						
						MapRoomID[ROOM2C] = MapRoomID[ROOM2C] + 1;
					}
					break;
				case 3:
					if (MapRoomID[ROOM3] < MaxRooms && MapName[x][y] == ROOMEMPTY) {
						if (MapRoom[ROOM3][MapRoomID[ROOM3]] != ROOMEMPTY) {
							MapName[x][y] = MapRoom[ROOM3][MapRoomID[ROOM3]];
						}
					}
					rooms[roomsIndex] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, zone, ROOM3, x * 8, y * 8, MapName[x][y]);
					if (!MapTemp[x][y - 1]) {
						rooms[roomsIndex].angle = 180;
						roomsIndex++;
					}
					else if (!MapTemp[x - 1][y]) {
						rooms[roomsIndex].angle = 90;
						roomsIndex++;
					}
					else if (!MapTemp[x + 1][y]) {
						rooms[roomsIndex].angle = 270;
						roomsIndex++;
					}
					else {
						roomsIndex++;
					}
					MapRoomID[ROOM3] = MapRoomID[ROOM3] + 1;
					break;

				case 4:
					if (MapRoomID[ROOM4] < MaxRooms && MapName[x][y] == ROOMEMPTY) {
						if (MapRoom[ROOM4][MapRoomID[ROOM4]] != ROOMEMPTY) {
							MapName[x][y] = MapRoom[ROOM4][MapRoomID[ROOM4]];
						}
					}
					rooms[roomsIndex++] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, zone, ROOM4, x * 8, y * 8, MapName[x][y]);
					MapRoomID[ROOM4] = MapRoomID[ROOM4] + 1;
					break;
				}
			}
		}
	}

	//printf("FIFTH: %d\n", rnd_state.rnd_state);

	rooms[roomsIndex++] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, 0, ROOM1, (MapWidth - 1) * 8, 8, GATEA);
	MapRoomID[ROOM1] = MapRoomID[ROOM1] + 1;

	rooms[roomsIndex++] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, 0, ROOM1, (MapWidth - 1) * 8, (MapHeight - 1) * 8, POCKETDIMENSION);
	MapRoomID[ROOM1] = MapRoomID[ROOM1] + 1;

	//If introenabled
	//dont need bc intro never on

	rooms[roomsIndex++] = CreateRoom(MapTemp, roomIdCounter, forest, e, rts, &bb, &rnd_state, 0, ROOM1, 8, 0, DIMENSION1499);
	MapRoomID[ROOM1] = MapRoomID[ROOM1] + 1;	

	//150 because that is length of rooms array that we guessed.
	//In the future, if this program works, we can check the room amount of 
	//every seed in the game, take the maximun number, and set the
	//rooms array length to that number.
	for (i = 0; i < 150; i++) {	
		//If id is -1 that means we reached end of actual rooms in array.
		if (rooms[i].id == -1) break;				
		
		PreventRoomOverlap(&rooms[i], rooms, e);	
	}

	for (i = 0; i < 150; i++) {
		Rooms* rr = &rooms[i];

		if (rr->id == -1) {
			//printf("\nFound %d rooms.\n", i);
			break;
		}

		printf("%d %d %d %d ", static_cast<int32_t>(rr->rt->name), rr->angle, int32_t(rr->x), int32_t(rr->z));
	}

	/*if (true) {
		for (int16_t i = 0; i < roomIdCounter; i++) {			

			Rooms* r = &rooms[i];

			printf("NAME: %s ANGLE: %d X: %d Z: %d\n", RoomIDToName(r->rt->name).c_str(), r->angle, int(r->x), int(r->z));
		}
		printf("RND_STATE: %d\n", rnd_state.rnd_state);	
		printf("SEED: %d\n", thread);
	}*/

	return 0;

	//Rooms* rooms2d[19][19];
	//for (y = 0; y < MapWidth + 1; y++) {
	//	for (x = 0; x < MapHeight + 1; x++) {
	//		rooms2d[y][x] = nullptr;
	//	}
	//}

	//for (i = 0; i < 150; i++) {
	//	if (rooms[i].id == -1) break;

	//	rooms2d[uint8_t(rooms[i].z / 8.0)][-uint8_t(rooms[i].x / 8.0) + 18] = &rooms[i];
	//}

	////temp = 0;

	////FINDING SPECIFIC SEED FROM MAP LAYOUT
	//for (y = 0; y < MapWidth + 1; y++) {			
	//	for (x = 0; x < MapHeight + 1; x++) {
	//		if (rooms2d[y][x] == nullptr) continue;

	//		if (rooms2d[y][x]->rt->name == START) {
	//			if (rooms2d[y][x - 2] != nullptr && rooms2d[y][x-2]->rt->name == ROOMPJ) {
	//				//printf("FOUND OVERLAP IN THREAD %d SEED %d\n", threadIdx.x, thread);
	//				return 1;
	//			}
	//			else {
	//				return 0;
	//			}						
	//			/*if (rooms2d[y-2][x-1] != nullptr && rooms2d[y - 2][x-1]->rt->name == ROOM914) {
	//				if (rooms2d[y - 2][x - 3] != nullptr && rooms2d[y - 2][x - 3]->rt->name == ROOM2SL) {
	//					if (rooms2d[y - 3][x - 3] != nullptr && rooms2d[y - 3][x - 3]->rt->name == ROOM2SCPS2) {
	//						printf("FOUND SEED: %d\n", thread);
	//						return 1;
	//					}
	//				}
	//			}*/
	//			return 0;
	//		}
	//	}
	//}

	///*if (thread == 201) {
	//	for (x = 0; x < MapWidth + 1; x++) {
	//		printf("\n");
	//		for (y = 0; y < MapHeight + 1; y++) {
	//			if (rooms2d[x][y] == nullptr) {
	//				printf("   ");
	//				continue;
	//			}
	//			if (rooms2d[x][y]->rt->name < 10) {
	//				printf(" %d ", rooms2d[x][y]->rt->name);
	//				continue;
	//			}
	//			printf("%d ", rooms2d[x][y]->rt->name);
	//		}
	//	}
	//	printf("\n");
	//}*/

	//return 0;

}