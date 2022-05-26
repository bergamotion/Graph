#include <stdlib.h>
#include "stack.h"

stack* stack_init()
{
	stack* result = stack_new_node(0);
	stack_pop(&result);
	return result;
}

stack* stack_new_node(int number)
{
	stack* node = (stack*)malloc(sizeof(stack));
	if (node) {
		node->num = number;
		node->next = nullptr;
	}
	return node;
}

stack* stack_push(stack* s, int number)
{
	stack* node = stack_new_node(number);
	if (s == nullptr) {
		s = node;
		return s;
	}
	node->next = s;
	return node;
}

int stack_pop(stack** s)
{
	if (*s == nullptr) return INT_MAX;
	int number = (*s)->num;
	stack* temp = *s;
	*s = (*s)->next;
	free(temp);
	return number;
}

void stack_free(stack* s)
{
	while (s) {
		stack* temp = s;
		s = s->next;
		free(temp);
	}
}
