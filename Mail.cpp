#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <random>
#include <stdatomic.h>
#include <thread>
#include <iostream>
#include <mutex> 
#include <unistd.h>

#define NUM_CARDS 500000
#define NUM_THREADS 8

typedef struct l {
	std::mutex lock;
	int val;
	struct l *next;
} OptNode;

OptNode* allocNode(int val);

void runThisShit(OptNode head);
bool validate(OptNode *head, OptNode *pred, OptNode *curr);
int pop(OptNode *head);
bool add(OptNode *head, int val);
bool remove(OptNode *head, int val);
bool contains(OptNode *head, int val);

int cards[NUM_CARDS];
std::atomic<int> taken = 0;
std::atomic<int> delivered =  0;

int main () {
	std::random_device rd;
	std::mt19937 g(rd());
	srand(time(0));
	// don't have to worry about freeing this 
	OptNode head = {.val = -1, .next = NULL};

	for(int i=0; i < NUM_CARDS; ++i) {  
		cards[i] = i;
	}

	std::shuffle(std::begin(cards), std::end(cards), g);

	std::thread workers[NUM_THREADS];
	for(int i=0; i < NUM_THREADS; ++i) { 
		workers[i] = std::thread(runThisShit, head);
	}

	for(int i=0; i < NUM_THREADS; ++i) {  
		workers[i].join();
	}


	std::cout << "report: " << delivered << " notes\n";

	return 0;
}

void runThisShit(OptNode head) {
	int sel;
	while(delivered < NUM_CARDS) {
		usleep(rand() % 200);
		sel = cards[atomic_fetch_add(&taken, 1)];
		std::cout << "receiving message " << sel << "\n";
		add(&head, sel);
		
		usleep(rand() % 200);
		sel = pop(&head);
		std::cout << "sending message " << sel << "\n";
		usleep(rand() % 200);
	}
}


OptNode* allocNode(int val) {
	OptNode* ret = (OptNode*)malloc(sizeof(OptNode));
	if(ret) {
		return ret;
	}
	return NULL;
}

int pop(OptNode *head) {
	OptNode *curr = head->next;

	while(1) {
		head->lock.lock();
		curr->lock.lock();
		if(validate(head, head, curr)) {
			int val = curr->val;
			head->next = curr->next;	
			free(curr);
			return val;
		}

		head->lock.unlock();
		curr->lock.unlock();
	}
}

bool validate(OptNode* head, OptNode *pred, OptNode *curr){
	OptNode *node = head; 
	while(node->val <= pred->val){
		if(node == pred) {
			return pred->next == curr;
		}
		node = node->next;
	}
	return false;
}

bool add(OptNode* head, int val) {
	bool ret = false;
	while(1) {
		OptNode *pred = head;
		OptNode *curr = head->next; 
		while(curr->val < val) {
			pred = curr;
			curr = curr->next;
		}

		pred->lock.lock();
		curr->lock.lock();

		if(validate(head, pred, curr)) {
			if(curr->val == val) {
				ret = false;
			}
			else {
				OptNode *node = allocNode(val);
				pred->next = node;
				node->next = curr;

				ret = true;
			}

			pred->lock.unlock();
			curr->lock.unlock();

			return ret;
		}

		pred->lock.unlock();
		curr->lock.unlock();
	}
}

bool remove(OptNode *head, int val){
	bool ret = false;
	while(1) {
		OptNode *pred = head;
		OptNode *curr = head->next; 
		while(curr->val < val) {
			pred = curr;
			curr = curr->next;
		}

		pred->lock.lock();
		curr->lock.lock();

		if(validate(head, pred, curr)) {
			if(curr->val == val) {
				pred->next = curr->next;

				pred->lock.unlock();
				curr->lock.unlock();
				free(curr);

				curr = NULL;

				ret = true;
			}
			else {
				pred->lock.unlock();
				curr->lock.unlock();

				ret = false;
			}

			return ret;
		}

		pred->lock.unlock();
		curr->lock.unlock();
	}
}
	
bool contains(OptNode *head,int val){
	bool ret = false;
	while(1) {
		OptNode *pred = head;
		OptNode *curr = head->next; 
		while(curr->val < val) {
			pred = curr;
			curr = curr->next;
		}

		pred->lock.lock();
		curr->lock.lock();

		if(validate(head, pred, curr)) {
			ret = (curr->val == val);
			pred->lock.unlock();
			curr->lock.unlock();
			return ret;
		}

		pred->lock.unlock();
		curr->lock.unlock();
	}
}
