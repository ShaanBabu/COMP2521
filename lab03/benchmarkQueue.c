
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "Queue.h"

int main(void) {
	Queue q = QueueNew();
// TODO
	// Write a benchmark test to demonstrate the difference in
	// efficiency between ArrayQueue and CircularArrayQueue
	// enqueue 1 to 1000 
	for (int i = 1; i <= 1000; i++) {
		QueueEnqueue(q, i);
	}
	assert(QueueSize(q) == 1000);

	// dequeue 1 to 500 
	for (int j = 1; j <= 500; j++) {
		assert(QueueFront(q) == j);
		assert(QueueDequeue(q) == j);
	}
	assert(QueueSize(q) == 500);


	QueueFree(q);
}

