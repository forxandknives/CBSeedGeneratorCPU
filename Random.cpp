#include "Random.h"

bbRandom::bbRandom() { }

float bbRandom::rnd(rnd_state* rnd_state) {
	rnd_state->rnd_state = RND_A * (rnd_state->rnd_state % RND_Q) - RND_R * (rnd_state->rnd_state / RND_Q);
	if (rnd_state->rnd_state < 0) rnd_state->rnd_state += RND_M;
	return (rnd_state->rnd_state & 65535) / 65536.0f + (.5f / 65536.0f);
}

float bbRandom::bbRnd(rnd_state* rnd_state, float from, float to) {
	return rnd(rnd_state) * (to - from) + from;
}

int32_t bbRandom::bbRand(rnd_state* rnd_state, int32_t from, int32_t to) {
	if (to < from) {
		int32_t temp = to;
		from = to;
		to = temp;
	}

	return int32_t(rnd(rnd_state) * (to - from + 1)) + from;
}

void bbRandom::bbSeedRnd(rnd_state* rnd_state, int32_t seed) {
	if (seed <= 0) {
		rnd_state->rnd_state = 1;
	}
	else {
		rnd_state->rnd_state = seed;
	}
}