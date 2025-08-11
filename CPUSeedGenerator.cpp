
#include "CreateSeed.h"
#include "Rooms.h"
#include "Extents.h"
#include "Forest.h"

#include <cstdlib>

#include <chrono>

bool is_number(const std::string& s);

int main(int argc, char* argv[]) {

	if (argc != 2) {
		printf("No seed specified.\n");
		return 0;
	}

	std::string s = argv[1];

	if (!is_number(s)) {
		printf("Inputted seed is not a number.\n");
		return 0;
	}

	int64_t seed = std::stoi(s);

	//Seed cannot be 0 or below, set to 1.
	if (seed <= 0) seed = 1;

	//2147483647 will crash, anything above doesn't work.
	if (seed >= 2147483647) seed = 2147483646;


	//////////ROOMTEMPLATES////////////////////
	RoomTemplates* rts = (RoomTemplates*)malloc(roomTemplateAmount * sizeof(RoomTemplates));	
	for (int i = 0; i < roomTemplateAmount; i++) {
		new (rts + i) RoomTemplates();
	}
	CreateRoomTemplates(rts);
	//////////ROOMTEMPLATES////////////////////
	/*for (int i = 0; i < roomTemplateAmount; i++) {
		RoomTemplates* r = &rts[i];
		printf("ID: %d\n", r->id);
		printf("INDEX: %d\n", r->index);
		printf("ZONE: %d %d %d %d %d\n", r->zone[0], r->zone[1], r->zone[2], r->zone[3], r->zone[4]);
		printf("SHAPE: %d\n", r->shape);
		printf("NAME: %s\n", RoomIDToName(r->name).c_str());
		printf("COMMONNESS: %d\n", r->commonness);
		printf("LIGHTS: %d\n", r->lights);
		printf("OVERLAPCHECK: %d\n", r->disableOverlapCheck);
		printf("\n");
	}*/

	//////////FOREST///////////////////////////
	uint8_t* forest = (uint8_t*)malloc(3380 * sizeof(uint8_t));
	PopulateForestData(forest);
	//////////FOREST///////////////////////////

	/////////EXTENTS///////////////////////////
	float* extents = (float*)malloc(48960 * sizeof(float));
	PopulateRoomExtents(extents);
	/////////EXTENTS///////////////////////////

	//////////////////OUTPUT///////////////////
	uint32_t size = 2000;
	uint8_t* output = (uint8_t*)malloc(size * sizeof(uint8_t));
	//////////////////OUTPUT///////////////////	

	CreateMap(int32_t(seed), rts, extents, forest);

	free(rts);
	for (int i = 0; i < roomTemplateAmount; i++) {
		rts[i].~RoomTemplates();
	}
	free(forest);
	free(extents);
	free(output);

	return 0;
}

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}