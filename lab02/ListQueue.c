// Implementation of the Queue ADT using a linked list

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

typedef struct node *Node;
struct node {
	Item item;
	Node next;
};

struct queue {
	Node head;
	Node tail;
	int  size;
};

/**
 * Creates a new empty queue
 */
Queue QueueNew(void) {
	Queue q = malloc(sizeof(*q));
	if (q == NULL) {
		fprintf(stderr, "couldn't allocate Queue\n");
		exit(EXIT_FAILURE);
	}

	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;
}

/**
 * Frees all resources associated with the given queue
 */
void QueueFree(Queue q) {
	Node curr = q->head;
	while (curr != NULL) {
		Node temp = curr;
		curr = curr->next; //O(logn)
		free(temp);
	}
	free(q);
}

/**
 * Adds an item to the end of the queue
 */
void QueueEnqueue(Queue q, Item it) {
	//make a new node (empty)
	struct node *new = malloc(sizeof(struct node));
	//assign values to new node
	new->item = it;
	new->next = NULL;
	//pre-req conditions are assigned
	if (q->head == NULL && q->tail == NULL) {
		q->head  = q->tail = new;
	} else {
		q->tail->next= new;
		q->tail = new;
	}
	//increase size by 1
	q->size ++;
	
}

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 */
Item QueueDequeue(Queue q) {
	// TODO
	struct node *temp;
	//integer value made for storing item value
	int value = 0;
	//if head = null
	if (q->head == NULL) {
		return 0;
	}
	else {
		temp = q->head;
		q->head = q->head->next;
		if (q->head == NULL) {
			q->tail = NULL; 
		}
		value = temp->item;
		free(temp);
	}
	q->size--; //decrease size by 1
	
	return value;
}

/**
 * Gets the item at the front of the queue without removing it
 * Assumes that the queue is not empty
 */
Item QueueFront(Queue q) {
	assert(q->size > 0);

	return q->head->item;
}

/**
 * Gets the size of the given queue
 */
int QueueSize(Queue q) {
	return q->size;
}

/**
 * Returns true if the queue is empty, and false otherwise
 */
bool QueueIsEmpty(Queue q) {
	return q->size == 0;
}

/**
 * Prints the items in the queue to the given file with items space-separated
 */
void QueueDump(Queue q, FILE *fp) {
	for (Node curr = q->head; curr != NULL; curr = curr->next) {
		fprintf(fp, "%d ", curr->item);
	}
	fprintf(fp, "\n");
}

/**
 * Prints out information for debugging
 */
void QueueDebugPrint(Queue q) {

}

