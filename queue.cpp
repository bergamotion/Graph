#include "queue.h"

queue* queue_init()
{
	queue* result = (queue*)malloc(sizeof(queue));
	if (result) {
		result->end = (queue_node**)malloc(sizeof(queue_node**));
		result->front = (queue_node**)malloc(sizeof(queue_node**));
		if (*(result->end) && *(result->front)) {
			*(result->end) = nullptr;
			*(result->front) = nullptr;
		}
	}
	return result;
}

queue_node* queue_new_node(int data)
{
	queue_node* node = (queue_node*)malloc(sizeof(queue_node));
	if (node) {
		node->data = data;
		node->next = nullptr;
	}
	return node;
}

void queue_push(queue* q, int data)
{
	if (*(q->end) == nullptr) {
		*(q->front) = *(q->end) = queue_new_node(data);
	}
	else {
		queue_node** queue_end = q->end;
		(*queue_end)->next = queue_new_node(data);
		(*queue_end) = (*queue_end)->next;
	}

}

int queue_pop(queue* q)
{
	if (*(q->front) == nullptr) return INT_NAN;
	int number = (*(q->front))->data;
	queue_node* temp = *(q->front);
	*(q->front) = (*(q->front))->next;
	free(temp);
	if (*(q->front) == nullptr) {
		*(q->front) = *(q->end) = nullptr;
	}
	return number;
}
