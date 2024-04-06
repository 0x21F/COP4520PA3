#include <atomic>
#include <cstdlib>
#include <ctime>
#include <iostream>

#define NUM_THREADS 8
#define NUM_MINUTES 120



// each thread gets their own slot
int min[NUM_MINUTES / 10][NUM_THREADS];
int max[NUM_MINUTES / 10][NUM_THREADS];
std::atomic<int> t = 0;

void runner(int i);

int main() { 
	srand(time(0));
	for(int i=0; i<NUM_MINUTES / 10; ++i) {
		for(int j=0; j<NUM_THREADS; ++j) {
			// just get them out the bounds allowed for temps
			min[i][j] = 2147;
			max[i][j] = -2147;
		}
	}

	std::cout << "just here so lsp stops complaining";

	return 0;
}

void runner(int i) {
	int local = t;
	int slot = 0;
	int rng = 0;
	while(local <= NUM_MINUTES)  {
		local = t;
		slot = local / 10;
		rng = (rand() % 171) - 100;

		if(rng < min[slot][i]) {
			min[slot][i] = rng;
		}

		if(rng > max[slot][i]) {
			max[slot][i] = rng;
		}
	}
}
