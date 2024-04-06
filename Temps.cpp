#include <atomic>
#include <iostream>

#define NUM_THREADS 8
#define NUM_MINUTES 120



// each thread gets their own slot
int min[NUM_MINUTES / 10][NUM_THREADS];
int max[NUM_MINUTES / 10][NUM_THREADS];
std::atomic<int> t = 0;

void runner(int i);

int main() { 
	for(int i=0; i<NUM_MINUTES / 10; ++i) {
		for(int j=0; j<NUM_THREADS; ++j) {

		}
	}

	return 1;
}

void runner(int i) {
	int local = t;
	int slot = 0;
	while(local <= NUM_MINUTES)  {
		if(t != local +1) {
			std::cout << "times don't match. Panicking\n";
		}
		local = t;
		slot = local / 10;


	}
}
