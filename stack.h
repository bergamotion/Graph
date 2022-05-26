#pragma once
#define INT_MAX 2147483647
struct stack {
	int num;
	struct stack* next;
};

stack* stack_push(stack* s, int number);
stack* stack_new_node(int number);
int stack_pop(stack** s);
void stack_free(stack* s);
stack* stack_init();
