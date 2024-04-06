#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <random>
#include <thread>
#include <iostream>
#include <mutex> 
#include <unistd.h>

#define NUM_CARDS 500000

typedef struct l {
	std::mutex lock;
	int val;
	struct l *next;
} OptNode;

OptNode* allocNode(int val);

bool validate(OptNode *head, OptNode *pred, OptNode *curr);
bool pop(OptNode *head);
bool add(OptNode *head, int val);
bool remove(OptNode *head, int val);
bool contains(OptNode *head, int val);

int cards[NUM_CARDS];

int main () {
	std::random_device rd;
	std::mt19937 g(rd());
	// don't have to worry about freeing this 
	OptNode head = {.val = -1, .next = NULL};

	for(int i=0; i < NUM_CARDS; ++i) {  
		cards[i] = i;
	}

	std::shuffle(std::begin(cards), std::end(cards), g);


	std::cout << "no\n";
	return 0;
}

OptNode* allocNode(int val) {
	OptNode* ret = (OptNode*)malloc(sizeof(OptNode));

	if(ret) {
		return ret;
	}

	return NULL;
}

bool pop(OptNode *head) {
	OptNode *curr = head->next;

	while(1) {
		head->lock.lock();
		curr->lock.lock();
		if(validate(head, head, curr)) {
			head->next = curr->next;	
			free(curr);
			return true;
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
