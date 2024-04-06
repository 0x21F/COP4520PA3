#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <thread>

#define NUM_THREADS 8
#define NUM_MINUTES 120

// each thread gets their own slot
int min[NUM_MINUTES / 10][NUM_THREADS];
int max[NUM_MINUTES / 10][NUM_THREADS];
int t = 0;

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

	std::thread sensors[NUM_THREADS];

	for(int i=0;i < NUM_THREADS; ++i) {
		sensors[i] = std::thread(runner, i);
	}

	while(t<NUM_MINUTES) {
		usleep(200);
		std::cout << "time is: " << ++t;

		if(t % 10 == 0) {
			int greatest_diff = 0; 
			for(int i=0; i < NUM_THREADS; ++i) {
				for(int j=0; j < NUM_THREADS; ++j) {
					greatest_diff = (max[t/10][j], min[t/10][i])
				}

			}
		}
	}

	for(int i=0;i < NUM_THREADS; ++i) {
		sensors[i].join();
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
