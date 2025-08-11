#pragma once

#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>

typedef struct rnd_state rnd_state;
static struct rnd_state {
	int32_t rnd_state;
};

class bbRandom {

public:	
	static const int32_t RND_A = 48271;
	static const int32_t RND_M = 2147483647;
	static const int32_t RND_Q = 44488;
	static const int32_t RND_R = 3399;

	bbRandom();

	static float rnd(rnd_state* rnd_state);

	float bbRnd(rnd_state* rnd_state, float from, float to);

	int32_t bbRand(rnd_state* rnd_state, int32_t from, int32_t to);

	void bbSeedRnd(rnd_state* rnd_state, int32_t seed);
};

#endif