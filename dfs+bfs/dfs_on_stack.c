#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


#define STACK_OVERFLOW  -228
#define SIZE 100

// для проверки 
int graph[6][6] = 
{
	{0, 0, 1, 1, 0, 0},
	{0, 0, 0, 0, 1, 1},
	{0, 0, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0}
};

typedef int data_t;
typedef struct { // структура для стека 
	int size; 
		data_t *data;
} stack_t;

stack_t stack = {}; 
int maxsize = 0;

data_t top(stack_t* stack) { // возвращащение элемента с вершины стека
return stack->data[stack->size-1];
}

void resize(stack_t *stack) {//изменение размера 
	maxsize *= 2;
	stack->data = realloc(stack->data, maxsize * sizeof(data_t));
	if(stack->data == NULL) {
		exit(STACK_OVERFLOW);
	}
	return;
} 

void push(stack_t* stack, data_t val) { // занесение элемента в стек
	if(stack->size >= maxsize) {
		resize(stack);
	}
	stack->data[stack->size++] = val;
	return;
}

data_t pop(stack_t *stack) { //снятие верхнего элемента 
	stack->size--;
	return stack->data[stack->size];
}

void stack_create(stack_t* stack, int maxs) {
	maxsize = maxs;
	assert(stack); 
	stack->data = (data_t*) calloc(maxsize, sizeof(stack->data[0])); 
	stack->size = 0;
return;
}
  
void stack_destroy(stack_t* stack) {
	free(stack->data); 
	stack->data = NULL;
  return;
}


int is_empty(stack_t* stack) { // возвращает 1, если стек пуст, и 0 - в противном случае
	if(stack->size == 0){ 
		return 1;}
	if(stack->size != 0) {
		return 0;}
	return 0;
}

int main(void){
	int i;
	data_t dir;
	stack_t s; 
	stack_create(&s, SIZE);
	push(&s, 0);
	while((is_empty(&s)) == 0) {
		dir = top(&s);
		printf("%d\n", dir + 1);
		pop(&s);
		for(i = 5; i >= 0; i--)
			if(graph[dir][i] == 1)
			push(&s, i);
	}
	stack_destroy(&s);
	return 0;
}
