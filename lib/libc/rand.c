#include <stdlib.h>

static unsigned long int _seed = 1;

int rand(void) {
	_seed = _seed * 1103515245 + 12345;
	return (int)(_seed / 65536) % 32768;
}

void srand(unsigned int seed) {
	_seed = seed;
}
