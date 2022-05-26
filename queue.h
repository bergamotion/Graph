#pragma once

#include <stdlib.h>
#define INT_NAN 2147483647

struct queue_node {
	int data;
	struct queue_node* next;
};

struct queue {
	struct queue_node** front;
	struct queue_node** end;
};


queue* queue_init();
queue_node* queue_new_node(int data);
void queue_push(queue* q, int data);
int queue_pop(queue* q);